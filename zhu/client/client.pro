QT -= gui
QT += serialport
CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
QT       += core gui
QT       += network
QT += gamepad
SOURCES += \
        main.cpp \
    gamepadmonitor.cpp \
    ControlOrderSendToRosThread.cpp \
    ControlOrder.cpp \
    TinyCarCO.cpp \
    TinyCarCommunication.cpp

HEADERS += \
    gamepadmonitor.h \
    ControlOrderSendToRosThread.h \
    ControlOrder.h \
    TinyCarCO.h \
    TinyCarCommunication.h
linux-g++*{
    #--add ros include
    INCLUDEPATH += -I /opt/ros/kinetic/include
    DEPENDPATH +=  /opt/ros/kinetic/include

    #--add ros libs

    LIBS += -L /opt/ros/kinetic/lib/ -lroscpp
    LIBS += -L /opt/ros/kinetic/lib/ -lroslib
    LIBS += -L /opt/ros/kinetic/lib/ -lpthread
    LIBS += -L /opt/ros/kinetic/lib/ -lroscpp_serialization
    LIBS += -L /opt/ros/kinetic/lib/ -lrostime
    LIBS += -L /opt/ros/kinetic/lib/ -lrosconsole
    LIBS += -L /opt/ros/kinetic/lib/ -lrosconsole_log4cxx
    LIBS += -L /opt/ros/kinetic/lib/ -lrosconsole_backend_interface
    LIBS += -L /opt/ros/kinetic/lib/ -lxmlrpcpp

    LIBS += -L$$PWD/../../../../opt/ros/kinetic/lib/ -lcpp_common

    INCLUDEPATH += $$PWD/../../../../opt/ros/kinetic/include
    DEPENDPATH += $$PWD/../../../../opt/ros/kinetic/include
}
