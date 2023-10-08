#ifndef TASKHANDLER_H
#define TASKHANDLER_H

#include <QObject>
#include <QAbstractListModel>
#include <QQmlEngine>

#include <string>
#include <vector>
#include <thread>

// these app IDs shouldn't be hidden/handled on any way
static std::string PROTECTED_APPIDS[] = {"appLauncher", "launcher"};

struct app {
    std::string name;
    std::string appId;
    int pid;
    bool visible = false;
};

bool appCompare(app a, app b){
    return a.pid < b.pid;
}

class TaskHandler : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
private:
    std::vector<app> getWindowList();
    std::thread *swayMonitor;
    std::vector<app> windowList;


public:
    explicit TaskHandler(QObject *parent = nullptr);
    ~TaskHandler();
    Q_INVOKABLE void hideActiveWindow();
    Q_INVOKABLE void bringWindowToForeground();
    void taskCallback(std::string response);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

signals:

};

#endif // TASKHANDLER_H
