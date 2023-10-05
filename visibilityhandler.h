#ifndef VISIBILITYHANDLER_H
#define VISIBILITYHANDLER_H

#include <QQmlEngine>

#include "direction-client-glue.h"

#define DBUS_DESTINATION "org.gspine.gesture"
#define DBUS_OBJECTPATH  "/org/gspine/gesture"


class VisibilityHandler : public QObject, public sdbus::ProxyInterfaces<org::gspine::Gesture_proxy>
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(bool visible READ isVisible NOTIFY visibleChanged)

private:
#ifdef DEBUG
    bool visible = true;
#else
    bool visible = false;
#endif
public:
    explicit VisibilityHandler(QObject *parent = nullptr);
    ~VisibilityHandler();
    Q_INVOKABLE void hide();
    Q_INVOKABLE void unhide();
    bool isVisible();

protected:
    // handle dbus event generated upon a directional touch event
    void onTouchEvent(const std::string& direction) override;

signals:
    void visibleChanged();

};


#endif // VISIBILITYHANDLER_H
