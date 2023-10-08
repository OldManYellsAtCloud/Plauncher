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

TaskHandler::TaskHandler(QObject *parent)
    : QAbstractListModel{parent}
{

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
    return 0;
}

QVariant TaskHandler::data(const QModelIndex &index, int role) const
{
    return QVariant();
}
