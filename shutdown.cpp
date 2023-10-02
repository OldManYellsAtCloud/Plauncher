#include "shutdown.h"
#include <unistd.h>

Shutdown::Shutdown(QObject *parent)
    : QObject{parent}
{

}

void Shutdown::shutdown()
{
    execl("/sbin/shutdown", "shutdown", "-P", "now", (char *)0);
}
