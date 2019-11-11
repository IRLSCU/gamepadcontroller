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

#ifndef GAMEPADMONITOR_H
#define GAMEPADMONITOR_H

#include <QtCore/QObject>
#include <QtCore/QTimer>

#include "tinycar.h"

#include "socketsend.h"
QT_BEGIN_NAMESPACE
class QGamepad;
QT_END_NAMESPACE

class GamepadMonitor : public QObject
{
    Q_OBJECT
public:
    explicit GamepadMonitor(QObject *parent = 0);
    ~GamepadMonitor();
    void setCar(Tinycar *car);
    void start();
    void formatString(QString& org, int n = 2, const QChar& ch = QChar(' '));
    QByteArray hexStringtoByteArray(QString hex);
    QString ByteArrayToHexString(QByteArray &ba);
    QString int2hexstring(int num);
    int hexString2Int(QString s);
    void angle2speed(double v, int* v_l ,int* v_r, double angle);
    QByteArray head;
//    for (int i=0;i<16;i++)
//    {
//         head[i] = 0x01;
//    }

     QByteArray a_speed;

     QByteArray b_speed;

     QByteArray a_orientation;

     QByteArray b_orientation;


     QByteArray tail;

     QByteArray order;

public:
    QGamepad *m_gamepad;
    Tinycar * car;
    //serialconnect* conn;
    socketsend* send;
private slots:
void showLabel(double value);

Tinycar& goSpeedUp(double value);
Tinycar& backSpeedUp(double value);
Tinycar& speed(double value);
Tinycar& turn(double value);
Tinycar& init(bool start);
Tinycar& stop(double value);
//Tinycar& disconnect(bool select);
Tinycar& front(bool select);
Tinycar& back(bool select);
Tinycar& showInfo(bool select);
//Tinycar& showCarCurrentInfo(bool select);
};

#endif // GAMEPADMONITOR_H
