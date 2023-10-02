#ifndef SHUTDOWN_H
#define SHUTDOWN_H

#include <QQmlEngine>

class Shutdown : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit Shutdown(QObject *parent = nullptr);
    Q_INVOKABLE void shutdown();

signals:

};

#endif // SHUTDOWN_H
