#ifndef TASKHANDLER_H
#define TASKHANDLER_H

#include <QObject>
#include <QAbstractListModel>
#include <QQmlEngine>

#include <string>
#include <vector>

// these app IDs shouldn't be hidden/handled on any way
static std::string PROTECTED_APPIDS[] = {"appLauncher", "launcher"};

struct app {
    std::string name;
    std::string appId;
    int pid;
    bool visible = false;
};

class TaskHandler : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
private:
    std::vector<app> getWindowList();
public:
    explicit TaskHandler(QObject *parent = nullptr);
    Q_INVOKABLE void hideActiveWindow();
    Q_INVOKABLE void bringWindowToForeground();

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

signals:

};

#endif // TASKHANDLER_H
