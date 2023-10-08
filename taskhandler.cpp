#include <functional>
#include "taskhandler.h"
#include "sway_utils.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

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

std::vector<app> TaskHandler::getWindowList()
{
    std::vector<app> windowList;
    std::string swayTree = send_sway_message("", message_type::GET_TREE);
    QJsonDocument jsonDocument = QJsonDocument::fromJson(swayTree.c_str());
    QJsonArray outputArray = jsonDocument.object()["nodes"].toArray();

    return filterOutputs(outputArray);
}

void TaskHandler::taskCallback(std::string response){
    QJsonDocument jsonDocument = QJsonDocument::fromJson(response.c_str());
    QString change = jsonDocument.object()["change"].toString();

    if (change != "new" && change != "close")
        return;

    QJsonObject containerObject = jsonDocument.object()["container"].toObject();

    std::vector<app> newWindowList = getWindowList();
    std::sort(newWindowList.begin(), newWindowList.end(), appCompare);

    int pidToHandle = containerObject["pid"].toInt();
    int indexToHandle = -1;

    if (change == "close") {
        for (int i = 0; i < windowList.size(); ++i){
            if (windowList.at(i).pid == pidToHandle){
                indexToHandle = i;
                break;
            }
        }

        if (indexToHandle < 0)
            return;

        emit beginRemoveRows(QModelIndex(), indexToHandle, 1);
        windowList.erase(windowList.begin() + indexToHandle);
        emit endRemoveRows();
    } else {
        if (pidToHandle < windowList.at(0).pid){
            indexToHandle = 0;
        } else if (pidToHandle > windowList.at(windowList.size() - 1).pid){
            indexToHandle = windowList.size();
        } else {
            for (int i = 0; i < windowList.size() - 2; ++i){
                if (pidToHandle > windowList.at(i).pid && pidToHandle < windowList.at(i + 1).pid) {
                    indexToHandle = i + 1;
                    break;
                }
            }
        }

        app newApp = {
            .name = containerObject["name"].toString().toStdString(),
            .appId = containerObject["app_id"].toString().toStdString(),
            .pid = containerObject["pid"].toInt()
        };

        emit beginInsertRows(QModelIndex(), indexToHandle, 1);
        windowList.insert(windowList.begin() + indexToHandle, newApp);
        emit endInsertRows();

    }

}

TaskHandler::TaskHandler(QObject *parent)
    : QAbstractListModel{parent}
{
    swayMonitor = new std::thread(send_sway_message, "[\"window\"]", message_type::SUBSCRIBE, 0, &TaskHandler::taskCallback);
}

TaskHandler::~TaskHandler()
{
    delete(swayMonitor);
}

void TaskHandler::hideActiveWindow()
{
    std::vector<app> windowList = getWindowList();
    qDebug() << "Received " << windowList.size() << " windows.";
    for (const auto& window: windowList){
        if (window.visible){
            send_sway_message("[pid=" + std::to_string(window.pid) + "] focus", message_type::RUN_COMMAND);
            send_sway_message("move container to scratchpad", message_type::RUN_COMMAND);
        }
    }
}

void TaskHandler::bringWindowToForeground()
{

}

int TaskHandler::rowCount(const QModelIndex &parent) const
{
    return windowList.size();
}

QVariant TaskHandler::data(const QModelIndex &index, int role) const
{
    return QVariant();
}
