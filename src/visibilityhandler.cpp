#include "visibilityhandler.h"

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
    visible = true;
    emit visibleChanged();
}

bool VisibilityHandler::isVisible()
{
    return visible;
}

void VisibilityHandler::onTouchEvent()
{
    unhide();
}


