#ifndef TASKHANDLER_H
#define TASKHANDLER_H

#include <QObject>
#include <QAbstractListModel>
#include <QQmlEngine>

#include <string>
#include <vector>
#include <thread>
#include <functional>

#include "screenlock-client-glue.h"

#define DISPLAY_DBUS_DESTINATION "org.gspine.display"
#define DISPLAY_DBUS_OBJECTPATH  "/org/gspine/display"

// these app IDs shouldn't be hidden/handled on any way - they supposed to handle themselves
static std::string PROTECTED_APPIDS[] = {"appLauncher", "launcher", "screenlock", "appscreenlock"};

struct app {
    std::string name;
    std::string appId;
    int pid = -1;
    bool visible = false;
};

static bool appCompare(app a, app b){
    return a.pid < b.pid;
}



class TaskHandler : public QAbstractListModel, public sdbus::ProxyInterfaces<org::gspine::screenLock_proxy>
{
    Q_OBJECT
    QML_ELEMENT
private:
    std::vector<app> getWindowList();
    std::thread *swayMonitor;
    std::function<void(std::string)> cb;
    std::vector<app> windowList;

    int lastActivePid;

    QHash<int, QByteArray> m_roleNames;
    void addTask(int pid);
    void removeTask(int pid);
    void initWindowList();

protected:
    void onScreenLocked(const bool& screenLocked) override;

public:
    explicit TaskHandler(QObject *parent = nullptr);
    ~TaskHandler();
    Q_INVOKABLE int hideActiveWindow();
    Q_INVOKABLE void bringWindowToForeground(QString pid);
    void taskCallback(std::string response);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    enum RoleNames {
        PidRole = Qt::UserRole,
        PictureRole = Qt::UserRole + 1,
    };

signals:

};

#endif // TASKHANDLER_H
