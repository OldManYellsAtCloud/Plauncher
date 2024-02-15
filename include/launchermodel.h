#ifndef LAUNCHERMODEL_H
#define LAUNCHERMODEL_H

#include <QAbstractListModel>

class LauncherModel : public QAbstractListModel
{
    Q_OBJECT
private:
    QHash<int, QByteArray> m_roleNames;
public:
    explicit LauncherModel(QObject *parent = nullptr);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void runCommand(int i);
    Q_INVOKABLE void runCommand(QString s);

    enum RoleNames {
        launcherNameRole = Qt::UserRole,
        iconRole = Qt::UserRole + 2,
        commandRole = Qt::UserRole + 3
    };
};

#endif // LAUNCHERMODEL_H
