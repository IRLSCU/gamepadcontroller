#include <QCoreApplication>


#include "gamepadmonitor.h"

int main(int argc, char **argv)
{
    QCoreApplication application(argc, argv);

    GamepadMonitor monitor;
    monitor.start();
    return application.exec();
}
