#include <functional>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include <loglibrary.h>

#include "taskhandler.h"
#include "sway_utils.h"
#include "settings.h"

bool isProtectedWindow(std::string win){
    auto *res = std::find(std::begin(PROTECTED_APPIDS), std::end(PROTECTED_APPIDS), win);
    return res != std::end(PROTECTED_APPIDS);
}

std::vector<app> filterWindowList(QJsonArray windowList){
    std::vector<app> windows;
    for (const auto& win: windowList){
        std::string windowName = win.toObject()["name"].toString().toStdString();
        if (!isProtectedWindow(windowName)){
            app window {
                .name = windowName,
                .appId = win.toObject()["app_id"].toString().toStdString(),
                .pid = win.toObject()["pid"].toInt(),
                .visible = win.toObject()["visible"].toBool()
            };
            windows.push_back(window);
        }
    }
    return windows;
}

std::vector<app> filterWorkspaces(QJsonArray workspaceList){
    std::vector<app> windows;
    for (const auto& ws: workspaceList){
        QJsonArray windowList = ws.toObject()["nodes"].toArray();
        auto newWindows = filterWindowList(windowList);
        windows.insert(std::end(windows), std::begin(newWindows), std::end(newWindows));

        // scratchpad stores containers as floating nodes
        windowList = ws.toObject()["floating_nodes"].toArray();
        newWindows = filterWindowList(windowList);
        windows.insert(std::end(windows), std::begin(newWindows), std::end(newWindows));
    }
    return windows;
}

std::vector<app> filterOutputs(QJsonArray outputList){
    std::vector<app> windows;
    for (const auto& output: outputList){
        QJsonArray workspaceList = output.toObject()["nodes"].toArray();
        auto newWindows = filterWorkspaces(workspaceList);
        windows.insert(std::end(windows), std::begin(newWindows), std::end(newWindows));
    }
    return windows;
}

TaskHandler::TaskHandler(QObject *parent)
    : QAbstractListModel{parent},
      ProxyInterfaces(DISPLAY_DBUS_DESTINATION, DISPLAY_DBUS_OBJECTPATH),
      screenLocked_{false}
{
    m_roleNames[RoleNames::PidRole] = "pid";
    m_roleNames[RoleNames::PictureRole] = "picture";
    cb = [this](std::string s){this->taskCallback(s);};

    swayMonitor = new std::thread(subscribe_sway_message, "[\"window\"]", cb);
    initWindowList();
    registerProxy();
}

TaskHandler::~TaskHandler()
{
    delete(swayMonitor);
    unregisterProxy();
}

void TaskHandler::initWindowList()
{
    auto initialWindowList = getWindowList();
    std::sort(initialWindowList.begin(), initialWindowList.end(), appCompare);
    int i = 0;
    for (auto& win: initialWindowList){
        if (!isProtectedWindow(win.name)){
            ++i;
        }
    }

    auto predicate = [](app a){return !isProtectedWindow(a.name);};

    beginInsertRows(QModelIndex(), 0, i);
    std::copy_if(initialWindowList.begin(), initialWindowList.end(), std::back_inserter(windowList), predicate);
    endInsertRows();
}

void TaskHandler::onScreenLocked(const bool &screenLocked)
{
    LOG("Screen locked signal: {}", screenLocked);
    screenLocked_ = screenLocked;
    if (screenLocked){
        lastActivePid = hideActiveWindow();
    } else if (lastActivePid > 0) {
        bringWindowToForeground(QString::number(lastActivePid));
        lastActivePid = 0;
    }
    emit screenLockChanged();
}

std::vector<app> TaskHandler::getWindowList()
{
    std::vector<app> windowList;
    std::string swayTree = send_sway_message("", message_type::GET_TREE);
    QJsonDocument jsonDocument = QJsonDocument::fromJson(swayTree.c_str());
    QJsonArray outputArray = jsonDocument.object()["nodes"].toArray();

    return filterOutputs(outputArray);
}

void TaskHandler::addTask(int pid){
    std::vector<app> newWindowList = getWindowList();

    auto comparator = [pid](app a){return a.pid == pid;};
    auto it = std::find_if(newWindowList.begin(), newWindowList.end(), comparator);

    if (it == newWindowList.end()){
        ERROR("Could not find pid {} in Sway tree!", pid);
        return;
    }

    if (isProtectedWindow((*it).name)){
        LOG("{} window is protected", (*it).name);
        return;
    }

    int indexToHandle;
    if (windowList.size() == 0 || pid < windowList.at(0).pid){
        indexToHandle = 0;
    } else if (pid > windowList.at(windowList.size() - 1).pid){
        indexToHandle = windowList.size();
    } else {
        for (int i = 0; i < windowList.size() - 2; ++i){
            // the list is sorted, so it's enough to check
            // which app is the first to be smaller
            if (pid > windowList.at(i).pid) {
                indexToHandle = i + 1;
                break;
            }
        }
    }

    beginInsertRows(QModelIndex(), indexToHandle, indexToHandle);
    windowList.insert(windowList.begin() + indexToHandle, *it);
    endInsertRows();
}

void TaskHandler::removeTask(int pid)
{
    auto comparator = [pid](app a){return a.pid == pid;};
    auto it = std::find_if(windowList.begin(), windowList.end(), comparator);

    if (it == windowList.end()){
        ERROR("Could not find app to close! PID: {}", pid);
        return;
    }

    beginRemoveRows(QModelIndex(), int(it - windowList.begin()), int(it - windowList.begin()));
    windowList.erase(it);
    endRemoveRows();
}

void TaskHandler::taskCallback(std::string response){
    QJsonDocument jsonDocument = QJsonDocument::fromJson(response.c_str());
    QString change = jsonDocument.object()["change"].toString();

    if (change != "new" && change != "close") {
        return;
    }

    QJsonObject containerObject = jsonDocument.object()["container"].toObject();
    QString app_id = containerObject["app_id"].toString();
    if (app_id == "pine.sgy.appLauncher") {
        return;
    }

    int pidToHandle = containerObject["pid"].toInt();

    if (change == "close") {
        removeTask(pidToHandle);
    } else {
        addTask(pidToHandle);
    }

}

int TaskHandler::hideActiveWindow()
{
    std::vector<app> windowList = getWindowList();
    LOG("Received {} windows", windowList.size());
    for (const auto& window: windowList){
        if (window.visible){
            send_sway_message("[pid=" + std::to_string(window.pid) + "] focus", message_type::RUN_COMMAND);
            send_sway_message("move container to scratchpad", message_type::RUN_COMMAND);
            return window.pid;
        }
    }
    return 0;
}

void TaskHandler::bringWindowToForeground(QString pid)
{
    hideActiveWindow();
    send_sway_message("[pid=" + pid.toStdString() + "] focus", message_type::RUN_COMMAND);
    send_sway_message("move container to workspace current", message_type::RUN_COMMAND);
    send_sway_message("floating disable", message_type::RUN_COMMAND);
}

int TaskHandler::rowCount(const QModelIndex &parent) const
{
    return windowList.size();
}

QVariant TaskHandler::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= windowList.size())
        return QVariant();
    if (role == RoleNames::PidRole) {
        return windowList.at(index.row()).pid;
    } else if (role == RoleNames::PictureRole){
        std::string appId = windowList.at(index.row()).appId;
        for (const auto& app: Settings::getSettings().getLauncherSettings()){
            if (app.appId.toStdString() == appId)
                return app.iconPath;
        }
    }
    return QVariant();
}

QHash<int, QByteArray> TaskHandler::roleNames() const
{
    return m_roleNames;
}
