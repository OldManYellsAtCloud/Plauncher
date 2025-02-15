#include "visibilityhandler.h"
#include <loglib/loglib.h>

VisibilityHandler::VisibilityHandler(QObject *parent): ProxyInterfaces(sdbus::ServiceName{DBUS_DESTINATION}, sdbus::ObjectPath{DBUS_OBJECTPATH}), QObject{parent}
{
    registerProxy();
}

VisibilityHandler::~VisibilityHandler()
{
    unregisterProxy();
}

void VisibilityHandler::hide()
{
    visible = false;
    emit visibleChanged();
}

void VisibilityHandler::unhide()
{
    if (!screenLocked) {
        visible = true;
        emit visibleChanged();
    }
}

bool VisibilityHandler::isVisible()
{
    return visible;
}

void VisibilityHandler::onTouchEvent()
{
    unhide();
}

void VisibilityHandler::lockScreen(bool slock)
{
    LOG_INFO_F("VisibilityHandler: lock screen request: {}", slock);
    screenLocked = slock;
    if (screenLocked)
        hide();
}

