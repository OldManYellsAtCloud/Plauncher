#include "launchermodel.h"
#include "settings.h"

#include <stdlib.h>

LauncherModel::LauncherModel(QObject *parent)
    : QAbstractListModel{parent}
{
    m_roleNames[LauncherModel::launcherNameRole] = "launcherName";
    m_roleNames[LauncherModel::commandRole] = "command";
    m_roleNames[LauncherModel::iconRole] = "icon";
}

int LauncherModel::rowCount(const QModelIndex &parent) const
{
    return Settings::getSettings().getLauncherSettings().size();
}

QVariant LauncherModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= Settings::getSettings().getLauncherSettings().size()){
        return QVariant();
    }

    switch(role) {
        case (launcherNameRole):
            return Settings::getSettings().getLauncherSettings().at(index.row()).name;
        case (commandRole):
            return Settings::getSettings().getLauncherSettings().at(index.row()).command;
        case (iconRole):
            return Settings::getSettings().getLauncherSettings().at(index.row()).iconPath;
    }
    return QVariant();
}

QHash<int, QByteArray> LauncherModel::roleNames() const
{
    return m_roleNames;
}

void LauncherModel::runCommand(int i)
{
    std::string cmd = Settings::getSettings().getLauncherSettings().at(i).command.toStdString();
    cmd.append(" &");
    system(cmd.c_str());
}
