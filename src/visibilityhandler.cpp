#include "visibilityhandler.h"
#include <loglibrary.h>

VisibilityHandler::VisibilityHandler(QObject *parent): ProxyInterfaces(DBUS_DESTINATION, DBUS_OBJECTPATH), QObject{parent}
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
    LOG("VisibilityHandler: lock screen request: {}", slock);
    screenLocked = slock;
    if (screenLocked)
        hide();
}

