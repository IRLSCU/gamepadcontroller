/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Gamepad module
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "gamepadmonitor.h"
#include <QtGamepad/QGamepad>
#include <QString>
#include <QDebug>
using namespace std;

GamepadMonitor::GamepadMonitor(QObject *parent): QObject(parent), m_gamepad(0)
{
    auto gamepads = QGamepadManager::instance()->connectedGamepads();

    if (gamepads.isEmpty()) {
        qDebug()<<"do not have to gamepads,please check the usb";
        return;
    }
    m_gamepad = new QGamepad(*gamepads.begin(), this);
#ifdef SENDTOROS
    controlOrderSendThread=new ControlOrderSendToRosThread(NULL);
    connect(this,&GamepadMonitor::setOrder,controlOrderSendThread,&ControlOrderSendToRosThread::setOrder);
    controlOrderSendThread->start();
#else
    serialport= new TinyCarCommunication();
    serialport->connect();
    temp[0]=0xFF;
    temp[1]=0xFE;
#endif
    controlOrder.init();
    sendOrder(controlOrder);
}

GamepadMonitor::~GamepadMonitor()
{
    controlOrderSendThread->stopImmediately();
    controlOrderSendThread->wait();
#ifdef SENDTOROS
    delete controlOrderSendThread;
#else
    if(serialport!=NULL){
        serialport->close();
        delete serialport;
    }
#endif
    delete m_gamepad;
}
void GamepadMonitor::turn(double value){
    if(abs(value)<0.05){
        controlOrder.setTurnRange(0);
    }else{
        controlOrder.setTurnRange(CONTROLORDER_MAX_TURN_RANGE*value);
    }
    sendOrder(controlOrder);
}

void GamepadMonitor::speed(double value){
    if(abs(value)<0.05){
        controlOrder.setSpeed(0);
    }else{
        controlOrder.setSpeed(CONTROLORDER_MAX_SPEED*value*(-1)/2);
    }
    sendOrder(controlOrder);
}

void GamepadMonitor::front(bool select){
    if(select){
        cout<<"car go front"<<endl;
        controlOrder.setGear(CONTROLORDER_GEAR_FORWARD);
        sendOrder(controlOrder);
    }
}
void GamepadMonitor::back(bool select){
    if(select){
        cout<<"car go back"<<endl;
        controlOrder.setGear(CONTROLORDER_GEAR_BACKWARD);
        sendOrder(controlOrder);
    }
}

void GamepadMonitor::init(bool start){
    if(start){
        controlOrder.init();
        sendOrder(controlOrder);
        qDebug()<<"init!!!!!!!!!!!!!";
    }
}

void GamepadMonitor::showInfo(bool select){
    if(select == true){
        qDebug()<<"left speed: "<<tinyCarCO.getLeftSpeed();
        qDebug()<<"right speed: "<<tinyCarCO.getRightSpeed();
        qDebug()<<"left orientation: "<<tinyCarCO.getLeftOrientation();
        qDebug()<<"right orientation: "<<tinyCarCO.getRightOrientation();
    }
}

void GamepadMonitor::start(){
        connect(m_gamepad, SIGNAL(axisLeftYChanged(double)), this, SLOT(speed(double)));//left axis  y for speed

        connect(m_gamepad, SIGNAL(axisRightXChanged(double)), this, SLOT(turn(double)));//right axis x for turn range

        connect(m_gamepad, &QGamepad::axisRightXChanged, this, [](double value){
            qDebug() << "Right X" << value;
        });
        connect(m_gamepad, &QGamepad::axisLeftYChanged, this, [](double value){
            qDebug() << "Left Y" << value;
        });

        connect(m_gamepad, SIGNAL(buttonUpChanged(bool)),this, SLOT(front(bool)));

        connect(m_gamepad, SIGNAL(buttonDownChanged(bool)),this, SLOT(back(bool)));

//        connect(m_gamepad, SIGNAL(buttonAChanged(bool)), this, SLOT(showCarCurrentInfo(bool)));

        connect(m_gamepad, SIGNAL(buttonStartChanged(bool)), this, SLOT(init(bool)));
        connect(m_gamepad, SIGNAL(buttonGuideChanged(bool)), this, SLOT(showInfo(bool)));
}
void GamepadMonitor::sendOrder(ControlOrder controlOrder){
    ControlOrder::NormalCO2TinyCarCO(controlOrder,tinyCarCO);
#ifdef SENDTOROS
    emit setOrder(tinyCarCO.getRightSpeed(),tinyCarCO.getLeftSpeed(),tinyCarCO.getRightOrientation(),tinyCarCO.getLeftOrientation());
#else
    temp[2]=tinyCarCO.getRightSpeed();
    temp[3]=tinyCarCO.getLeftSpeed();
    temp[4]=tinyCarCO.getRightOrientation();
    temp[5]=tinyCarCO.getLeftOrientation();
    serialport->sendMessage(temp);
#endif
}
