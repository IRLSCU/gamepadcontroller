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
#include <QLatin1Char>                                                               ^
#include "tinycar.h"
#include <QDebug>
#include <iostream>
#include <QList>
using namespace std;

GamepadMonitor::GamepadMonitor(QObject *parent): QObject(parent), m_gamepad(0)
{
    auto gamepads = QGamepadManager::instance()->connectedGamepads();
    if (gamepads.isEmpty()) {
        return;
    }

    m_gamepad = new QGamepad(*gamepads.begin(), this);
    car = new Tinycar();//car duixiang
//    conn = new serialconnect();//chuankoulianjieduixiang fasongmingling
//    conn->openPort();//dakaichuankoucaozuo
    send = new socketsend();
    send->connect();
//    QByteArray head;
//    for (int i=0;i<16;i++)
//    {
//         head[i] = 0x01;
//    }
     head.resize(2);
     head[0] = 0xff;
     head[1] = 0xfe;
     a_speed.resize(1);//A车轮速度
     a_speed[0] = 0x12;
     b_speed.resize(1);//B车轮速度
     b_speed[0] = 0x12;
     a_orientation.resize(1);//A车轮方向
     a_orientation[0] = 0x00;
     b_orientation.resize(1);//B车轮方向
     b_orientation[0] = 0x00;

     tail.resize(4);//A车轮方向
     tail[0] = tail[1] = tail[2] = tail[3] = 0x00;

     order.resize(10);
     order[0] = head[0];
     order[1] = head[1];
     order[2] = a_speed[0];
     order[3] = b_speed[0];
     order[4] = a_orientation[0];
     order[5] = b_orientation[0];
     order[6] = tail[0];
     order[7] = tail[1];
     order[8] = tail[2];
     order[9] = tail[3];

}
void GamepadMonitor::setCar(Tinycar *car){
    this->car = car;
}
GamepadMonitor::~GamepadMonitor()
{
//    conn->~serialconnect();
    send->disconnect();
    send->~socketsend();
    delete m_gamepad;
}

void GamepadMonitor::showLabel(double value){
    qDebug()<<"showlabel";
    cout<<"value is : "<<value<<endl;

}

void GamepadMonitor::angle2speed(double v, int* v_r ,int* v_l, double angle){
    double L =0.6;// che chang
    double W =0.6;// che kuan
    double R =0;// ZHUANXIANGBANJING
    double theta = angle/180.0*3.14159265; //zhuanxiangjiaodu
    double speed_l = 0.0, speed_r = 0.0;

    speed_l = v*(L/tan(theta)-(W/2))/(L/(tan(theta)));
    speed_r = v*(L/tan(theta)+(W/2))/(L/(tan(theta)));
    *v_l = (int)speed_l;
    *v_r = (int)speed_r;
    qDebug()<<v_l<<", "<<v_r;
}

void GamepadMonitor::formatString(QString& org, int n, const QChar& ch){
    int size = org.size();
    int space = qRound(size*1.0/n+0.5)-1;
    if(space<=0){
        return;
    }
    for(int i=0,pos=n;i<space;++i,pos+=(n+1)){
        org.insert(pos,ch);
    }
}

QByteArray GamepadMonitor::hexStringtoByteArray(QString hex){
    QByteArray ret;
    hex = hex.trimmed();
    formatString(hex,2,' ');
    QStringList sl = hex.split(" ");
    foreach (QString s, sl) {
        if(!s.isEmpty())
            ret.append((char)s.toInt(0,16)&0xFF);
    }
    return ret;
}

QString GamepadMonitor::ByteArrayToHexString(QByteArray &ba)
 {
     QDataStream out(&ba,QIODevice::ReadWrite);   //将str的数据 读到out里面去
     QString buf;
     while(!out.atEnd())
     {
         qint8 outChar = 0;
         out >> outChar;   //每次一个字节的填充到 outchar
         QString str =QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0')).toUpper() + QString(" ");   //2 字符宽度
         buf += str;
     }
     return buf;
 }

QString GamepadMonitor::int2hexstring(int num){
    return QString("%1").arg(num,2,16,QLatin1Char('0'));//k为int型或char型都可

}

int GamepadMonitor::hexString2Int(QString s)
{
    int i;
    int n = 0;
    int len =2;
    if(s.length()<len) return 0; //解决崩毁，指针溢出问题

    for (i=0;i<len;++i)
        {
            if (s.at(i) > '9')
            {
                n = 16 * n + (10 + s.at(i).toLatin1() - 'A');
            }
            else
            {
                n = 16 * n +( s.at(i).toLatin1() - '0');
            }
        }
        return n;
}

Tinycar& GamepadMonitor::goSpeedUp(double value){

}
Tinycar& GamepadMonitor::backSpeedUp(double value){

}
Tinycar& GamepadMonitor::turn(double value){

    double L ;// che chang
    double W ;// che kuan
    double R ;// ZHUANXIANGBANJING
    double theta; //zhuanxiangjiaodu
    double v_l;
    double v_r;
    double v = car->left_c_speed;
    v_l = v*(L/tan(theta)-(W/2))/(L/(tan(theta)));
    v_r = v*(L/tan(theta)+(W/2))/(L/(tan(theta)));



    int mini_speed = 32;//(car->left_c_speed+car->right_c_speed) / 2;
    int left_speed,right_speed;

    if(value<0){//turn left // right speed > left speed
        value = -value;
        if(value<0.2){
            left_speed = right_speed = (car->left_c_speed+car->right_c_speed)/2;//left right speed are same
        }
        else if(value<0.4&&value>0.21){

//            left_speed = mini_speed;
//            right_speed = mini_speed + (0.36*mini_speed);
            angle2speed(mini_speed,&left_speed,&right_speed,30.0);
            qDebug()<<left_speed<<", "<<right_speed;

        }
        else if(value<0.6&&value>0.41){
//            left_speed = mini_speed;
//            right_speed = mini_speed + (0.56*mini_speed);
            angle2speed(mini_speed,&left_speed,&right_speed,45.0);
            qDebug()<<left_speed<<", "<<right_speed;
        }
        else if(value<0.8&&value>0.61){
//            left_speed = mini_speed;
//            right_speed = mini_speed + (0.76*mini_speed);
            angle2speed(mini_speed,&left_speed,&right_speed,60.0);
            qDebug()<<left_speed<<", "<<right_speed;
        }
        else if(value<1.0&&value>0.81){
//            left_speed = mini_speed;
//            right_speed = mini_speed + (0.9*mini_speed);
            angle2speed(mini_speed,&left_speed,&right_speed,60.0);
            qDebug()<<left_speed<<", "<<right_speed;
        }

        QString l_sp_hex_str = int2hexstring(left_speed);
        QString r_sp_hex_str = int2hexstring(right_speed);
        QString order_str = "FFFE"+l_sp_hex_str+r_sp_hex_str+car->leftOrientation+car->rightOrientation+"00000000";
        qDebug()<<car->leftOrientation+car->rightOrientation;
        qDebug()<<order_str;
        qDebug()<<"turn left  left wheel speed :"<<left_speed;
        qDebug()<<"turn left  right wheel speed :"<<right_speed;
        car->left_c_speed =left_speed;
        car->right_c_speed = right_speed;
        car->left_speed_str = l_sp_hex_str;
        car->right_speed_str = r_sp_hex_str;
        order = hexStringtoByteArray(order_str);
        send->sendinfo(order_str);
//        conn->send(order);
    }
    else if(value>0){//turn right //left speed > right speed

        if(value<0.2){
            left_speed = right_speed = (car->left_c_speed+car->right_c_speed)/2;//left right speed are same
        }

        else if(value<0.4&&value>0.21){

//            left_speed = mini_speed;
//            right_speed = mini_speed + (0.36*mini_speed);
            angle2speed(mini_speed,&left_speed,&right_speed,-30.0);
            qDebug()<<left_speed<<", "<<right_speed;

        }
        else if(value<0.6&&value>0.41){
//            left_speed = mini_speed;
//            right_speed = mini_speed + (0.56*mini_speed);
            angle2speed(mini_speed,&left_speed,&right_speed,-45.0);

        }
        else if(value<0.8&&value>0.61){
//            left_speed = mini_speed;
//            right_speed = mini_speed + (0.76*mini_speed);
            angle2speed(mini_speed,&left_speed,&right_speed,-60.0);

        }
        else if(value<1.0&&value>0.81){
//            left_speed = mini_speed;
//            right_speed = mini_speed + (0.9*mini_speed);
            angle2speed(mini_speed,&left_speed,&right_speed,-60.0);

        }

        QString l_sp_hex_str = int2hexstring(left_speed);
        QString r_sp_hex_str = int2hexstring(right_speed);
        QString order_str = "FFFE"+l_sp_hex_str+r_sp_hex_str+car->leftOrientation+car->rightOrientation+"00000000";
        qDebug()<<car->leftOrientation+car->rightOrientation;
        qDebug()<<order_str;
        qDebug()<<"turn left  left wheel speed :"<<left_speed;
        qDebug()<<"turn left  right wheel speed :"<<right_speed;
        car->left_c_speed =left_speed;
        car->right_c_speed = right_speed;
        car->left_speed_str = l_sp_hex_str;
        car->right_speed_str = r_sp_hex_str;
        order = hexStringtoByteArray(order_str);
        send->sendinfo(order_str);
//        conn->send(order);
    }

}

Tinycar& GamepadMonitor::speed(double value){
/*
    QByteArray state = conn->receiveInfo();
    qDebug()<<"state"<<state;


    QString str_state = ByteArrayToHexString(state);

    qDebug()<<"str_state"<<str_state;
    QList<QString> state_list = str_state.split(' ');

     qDebug()<<"state_list"<<state_list;
     qDebug()<<"state_list size "<<state_list.size();
     if(state_list.size()>=12){
         QString left_c_speed_str =  state_list[2];
         QString left_c_speed_orientation_str =  state_list[3];
         QString right_c_speed_str =  state_list[4];
         QString right_c_speed_orientation_str =  state_list[5];
         //chushihua che de dangqian zhuangtai canshu
         int left_c_speed = hexString2Int(left_c_speed_str);
         int right_c_speed = hexString2Int(right_c_speed_str);
         int left_c_speed_orientation = hexString2Int(left_c_speed_orientation_str);
         int right_c_speed_orientation = hexString2Int(right_c_speed_orientation_str);

         qDebug()<<"left_c_speed"<<left_c_speed;
         qDebug()<<"right_c_speed"<<right_c_speed;
         qDebug()<<"left_c_speed_orientation"<<left_c_speed_orientation;
         qDebug()<<"right_c_speed_orientation"<<right_c_speed_orientation;

         int left_speed = left_c_speed + (left_c_speed * -value);
         int right_speed = right_c_speed + (right_c_speed * -value);
         qDebug()<<"left speed right speed :"<< left_speed<<","<<right_speed;

         QString left_speed_str = int2hexstring(left_speed);
         QString right_speed_str = int2hexstring(right_speed);
         QString left_speed_orientation_str;
         QString right_speed_orientation_str;
         if(left_c_speed_orientation==2){
             left_speed_orientation_str = int2hexstring(0);
             car->setLeftOrientation("00");
         }else if(left_c_speed_orientation==0){
             left_speed_orientation_str = int2hexstring(1);
             car->setLeftOrientation("01");
         }
         if(right_c_speed_orientation==2){
             right_speed_orientation_str = int2hexstring(0);
             car->setRightOrientation("00");
         }else if(right_c_speed_orientation==0){
             right_speed_orientation_str = int2hexstring(1);
             car->setRightOrientation("01");
         }
         QString order_str = "fffe"+left_speed_str+right_speed_str+left_speed_orientation_str+right_speed_orientation_str+"00000000";
         order = hexStringtoByteArray(order_str);
         qDebug()<<"orders:"<<order;
         cout<<"go speed up or down"<<value<<endl;
         car->goSpeedUp(value);
         conn->receiveInfo();
         conn->send(order);
         car->setLeftSpeed(left_speed);
         car->setRightSpeed(right_speed);

     }
     */
    value = -value;
    QString sp_hex_str;
     QString order_str;
     if(value<-0.05){
//         conn->send(hexStringtoByteArray("FFFE0000000000000000"));
         send->sendinfo("FFFE0000000000000000");
         car->left_c_speed = 0;
         car->right_c_speed = 0;
         car->left_speed_str = "00";
         car->right_speed_str = "00";
         qDebug()<<"stop!!!!!!!!!!!!!";
     }
     else{
         if(value<0.125&&value>0.09){
             int sp = 32;
             sp_hex_str = int2hexstring(sp);
              order_str = "FFFE"+sp_hex_str+sp_hex_str+car->leftOrientation+car->rightOrientation+"00000000";
              qDebug()<<car->leftOrientation+car->rightOrientation;
              qDebug()<<order_str;
              car->left_c_speed = sp;
              car->right_c_speed = sp;
              car->left_speed_str = sp_hex_str;
              car->right_speed_str = sp_hex_str;
              order = hexStringtoByteArray(order_str);
//              conn->send(order);
              send->sendinfo(order_str);
         }
         if(value<0.25&&value>0.135){
             int sp = 64;
             sp_hex_str = int2hexstring(sp);
              order_str = "FFFE"+sp_hex_str+sp_hex_str+car->leftOrientation+car->rightOrientation+"00000000";
              qDebug()<<car->leftOrientation+car->rightOrientation;
              qDebug()<<order_str;
              car->left_c_speed = sp;
              car->right_c_speed = sp;
              car->left_speed_str = sp_hex_str;
              car->right_speed_str = sp_hex_str;
              order = hexStringtoByteArray(order_str);
//              conn->send(order);
              send->sendinfo(order_str);
         }
         if(value<0.375&&value>0.26){
             int sp = 96;
             sp_hex_str = int2hexstring(sp);
              order_str = "FFFE"+sp_hex_str+sp_hex_str+car->leftOrientation+car->rightOrientation+"00000000";
              qDebug()<<car->leftOrientation+car->rightOrientation;
              qDebug()<<order_str;
              car->left_c_speed = sp;
              car->right_c_speed = sp;
              car->left_speed_str = sp_hex_str;
              car->right_speed_str = sp_hex_str;
              order = hexStringtoByteArray(order_str);
//              conn->send(order);
              send->sendinfo(order_str);
         }
         if(value<0.5&&value>0.385){
             int sp = 128;
             sp_hex_str = int2hexstring(sp);
              order_str = "FFFE"+sp_hex_str+sp_hex_str+car->leftOrientation+car->rightOrientation+"00000000";
              qDebug()<<car->leftOrientation+car->rightOrientation;
              qDebug()<<order_str;
              car->left_c_speed = sp;
              car->right_c_speed = sp;
              car->left_speed_str = sp_hex_str;
              car->right_speed_str = sp_hex_str;
              order = hexStringtoByteArray(order_str);
//              conn->send(order);
              send->sendinfo(order_str);
         }
         if(value<0.625&&value>0.51){
             int sp = 160;
             sp_hex_str = int2hexstring(sp);
              order_str = "FFFE"+sp_hex_str+sp_hex_str+car->leftOrientation+car->rightOrientation+"00000000";
              qDebug()<<car->leftOrientation+car->rightOrientation;
              qDebug()<<order_str;
              car->left_c_speed = sp;
              car->right_c_speed = sp;
              car->left_speed_str = sp_hex_str;
              car->right_speed_str = sp_hex_str;
              order = hexStringtoByteArray(order_str);
//              conn->send(order);
              send->sendinfo(order_str);
         }
         if(value<0.75&&value>0.635){
             int sp = 192;
             sp_hex_str = int2hexstring(sp);
              order_str = "FFFE"+sp_hex_str+sp_hex_str+car->leftOrientation+car->rightOrientation+"00000000";
              qDebug()<<car->leftOrientation+car->rightOrientation;
              qDebug()<<order_str;
              car->left_c_speed = sp;
              car->right_c_speed = sp;
              car->left_speed_str = sp_hex_str;
              car->right_speed_str = sp_hex_str;
              order = hexStringtoByteArray(order_str);
//              conn->send(order);
              send->sendinfo(order_str);
         }
         if(value<0.875&&value>0.76){
             int sp = 223;
             sp_hex_str = int2hexstring(sp);
              order_str = "FFFE"+sp_hex_str+sp_hex_str+car->leftOrientation+car->rightOrientation+"00000000";
              qDebug()<<car->leftOrientation+car->rightOrientation;
              qDebug()<<order_str;
              car->left_c_speed = sp;
              car->right_c_speed = sp;
              car->left_speed_str = sp_hex_str;
              car->right_speed_str = sp_hex_str;
              order = hexStringtoByteArray(order_str);
//              conn->send(order);
              send->sendinfo(order_str);
         }
         if(value<1.0&&value>0.9){
             int sp = 255;
             sp_hex_str = int2hexstring(sp);
              order_str = "FFFE"+sp_hex_str+sp_hex_str+car->leftOrientation+car->rightOrientation+"00000000";
              qDebug()<<car->leftOrientation+car->rightOrientation;
              qDebug()<<order_str;
              car->left_c_speed = sp;
              car->right_c_speed = sp;
              car->left_speed_str = sp_hex_str;
              car->right_speed_str = sp_hex_str;
              order = hexStringtoByteArray(order_str);
//              conn->send(order);
              send->sendinfo(order_str);
         }
     }




//    if(value<0){
//        int speed = -value/1*0xff;
//        QString hex = QString("%1").arg(speed, 2, 16, QLatin1Char('0'));
//        qDebug()<<speed;
//        qDebug()<< hex.toLatin1()[0];
//        qDebug()<<'1'<< hex.toLatin1()[1];

//         a_speed.resize(1);//A车轮速度
////         QByteArray b =  hex.toLocal8Bit();
////         a_speed[0] = b;
//         b_speed.resize(1);//B车轮速度
//         b_speed[0] = 0x12;
//         order[2] = a_speed[0];
//         order[3] = b_speed[0];
//         a_orientation.resize(1);//A车轮方向
//         a_orientation[0] = 0x01;
//         b_orientation.resize(1);//B车轮方向
//         b_orientation[0] = 0x01;
//         order[4] = a_orientation[0];
//         order[5] = b_orientation[0];
//         cout<<"go speed up"<<value<<endl;
//         car->goSpeedUp(value);
//         conn->receiveInfo();
//         conn->send(order);
//    }
//    else if(value>0){
//        a_speed.resize(1);//A车轮速度
//        a_speed[0] = 0x12;
//        b_speed.resize(1);//B车轮速度
//        b_speed[0] = 0x12;
//        order[2] = a_speed[0];
//        order[3] = b_speed[0];
//        a_orientation.resize(1);//A车轮方向
//        a_orientation[0] = 0x00;
//        b_orientation.resize(1);//B车轮方向
//        b_orientation[0] = 0x00;
//        order[4] = a_orientation[0];
//        order[5] = b_orientation[0];
//        cout<<"go speed down"<<value<<endl;
//        car->goSpeedUp(value);
//        conn->send(order);
//    }
}

Tinycar& GamepadMonitor::front(bool select){
    //current order = conn->receiveInfo();
    cout<<"car go front"<<endl;

    car->setLeftOrientation("00");
    car->setRightOrientation("00");
    QString order_str = "FFFE"+car->left_speed_str+car->right_speed_str+car->leftOrientation+car->rightOrientation+"00000000";
    order = hexStringtoByteArray(order_str);
//    conn->send(order);
    send->sendinfo(order_str);
}
Tinycar& GamepadMonitor::back(bool select){
    //current order = conn->receiveInfo();
    cout<<"car go back"<<endl;
    car->setLeftOrientation("01");
    car->setRightOrientation("01");
    QString order_str = "FFFE"+car->left_speed_str+car->right_speed_str+car->leftOrientation+car->rightOrientation+"00000000";
    order = hexStringtoByteArray(order_str);
//    conn->send(order);
    send->sendinfo(order_str);
}

Tinycar& GamepadMonitor::init(bool start){
    cout<<"car init"<<start<<endl;
    car->init();
//    if(!conn->is_port_open()){
//        conn->openPort();
//    }
}
Tinycar& GamepadMonitor::stop(double value){
    order = hexStringtoByteArray("FFFE0000000000000000");
//    conn->send(order);
    send->sendinfo("FFFE0000000000000000");
    car->left_c_speed = 0;
    car->right_c_speed = 0;
    car->left_speed_str = "00";
    car->right_speed_str = "00";
    qDebug()<<"stop!!!!!!!!!!!!!";

}

Tinycar& GamepadMonitor::showInfo(bool select){
    if(select == true){
        qDebug()<<"car left speed: "<<car->left_c_speed;
        qDebug()<<"car right speed: "<<car->right_c_speed;
        qDebug()<<"car left orita speed: "<<car->leftOrientation;
        qDebug()<<"car right orit speed: "<<car->rightOrientation;
    }
}

//Tinycar& GamepadMonitor::showCarCurrentInfo(bool select){
//    if(select == true){
//        QByteArray state = conn->receiveInfo();
//        qDebug()<<state;


//        QString str_state = ByteArrayToHexString(state);

//        qDebug()<<"str_state"<<str_state;
//        QList<QString> state_list = str_state.split(' ');

//         qDebug()<<"state_list"<<state_list;
//         qDebug()<<"state_list size "<<state_list.size();
//         if(state_list.size()>=12){
//             QString left_c_speed_str =  state_list[2];
//             QString left_c_speed_orientation_str =  state_list[3];
//             QString right_c_speed_str =  state_list[4];
//             QString right_c_speed_orientation_str =  state_list[5];
//             //chushihua che de dangqian zhuangtai canshu
//             int left_c_speed = hexString2Int(left_c_speed_str);
//             int right_c_speed = hexString2Int(right_c_speed_str);
//             int left_c_speed_orientation = hexString2Int(left_c_speed_orientation_str);
//             int right_c_speed_orientation = hexString2Int(right_c_speed_orientation_str);

//             qDebug()<<"left_c_speed"<<left_c_speed;
//             qDebug()<<"right_c_speed"<<right_c_speed;
//             qDebug()<<"left_c_speed_orientation"<<left_c_speed_orientation;
//             qDebug()<<"right_c_speed_orientation"<<right_c_speed_orientation;
//    }
//}
//}

//Tinycar& GamepadMonitor::disconnect(bool select){
//    conn->closePort();
//}

void GamepadMonitor::start(){

    //    Tinycar* car  = new Tinycar();


        connect(m_gamepad, SIGNAL(axisLeftXChanged(double)), this, SLOT(turn(double)));
        connect(m_gamepad, SIGNAL(axisLeftYChanged(double)), this, SLOT(speed(double)));


        connect(m_gamepad, &QGamepad::axisRightXChanged, this, [](double value){
            qDebug() << "Right X" << value;
        });
        connect(m_gamepad, &QGamepad::axisRightYChanged, this, [](double value){
            qDebug() << "Right Y" << value;
        });

        connect(m_gamepad, &QGamepad::buttonLeftChanged,this, [](double value){
            qDebug()<<"buttonLeftChanged"<<value;
        });

        connect(m_gamepad, &QGamepad::buttonRightChanged,this, [](double value){
            qDebug()<<"buttonRightChanged"<<value;
        });

        connect(m_gamepad, SIGNAL(buttonUpChanged(bool)),this, SLOT(front(bool)));

        connect(m_gamepad, SIGNAL(buttonDownChanged(bool)),this, SLOT(back(bool)));

//        connect(m_gamepad, SIGNAL(buttonAChanged(bool)), this, SLOT(showCarCurrentInfo(bool)));

        connect(m_gamepad, &QGamepad::buttonBChanged, this, [](bool pressed){
            qDebug() << "Button B" << pressed;
        });
        connect(m_gamepad, &QGamepad::buttonXChanged, this, [](bool pressed){
            qDebug() << "Button X" << pressed;
        });
        connect(m_gamepad, &QGamepad::buttonYChanged, this, [](bool pressed){
            qDebug() << "Button Y" << pressed;
        });
        connect(m_gamepad, &QGamepad::buttonL1Changed, this, [](bool pressed){
            qDebug() << "Button L1" << pressed;
        });
        connect(m_gamepad, &QGamepad::buttonR1Changed, this, [](bool pressed){
            qDebug() << "Button R1" << pressed;
        });
        connect(m_gamepad, &QGamepad::buttonL2Changed, this, [](double value){
            qDebug() << "Button L2: " << value;
        });
        connect(m_gamepad, &QGamepad::buttonR2Changed, this, [](double value){
            qDebug() << "Button R2: " << value;
        });
//        connect(m_gamepad, SIGNAL(buttonSelectChanged(bool)), this, SLOT(disconnect(bool)));
        connect(m_gamepad, SIGNAL(buttonStartChanged(bool)), this, SLOT(init(bool)));
        connect(m_gamepad, SIGNAL(buttonGuideChanged(bool)), this, SLOT(showInfo(bool)));



        /*
        connect(m_gamepad, &QGamepad::axisRightXChanged, this, [](double value){
            qDebug() << "Right X" << value;
        });
        connect(m_gamepad, &QGamepad::axisRightYChanged, this, [](double value){
            qDebug() << "Right Y" << value;
        });

        connect(m_gamepad, &QGamepad::buttonLeftChanged,this, [](double value){
            qDebug()<<"buttonLeftChanged"<<value;
        });

        connect(m_gamepad, &QGamepad::buttonRightChanged,this, [](double value){
            qDebug()<<"buttonRightChanged"<<value;
        });

        connect(m_gamepad, &QGamepad::buttonUpChanged,this, [](double value){
            qDebug()<<"buttonUpChanged"<<value;
        });

        connect(m_gamepad, &QGamepad::buttonDownChanged,this, [](double value){
            qDebug()<<"buttonDownChanged"<<value;
        });

        connect(m_gamepad, &QGamepad::buttonAChanged, this, [](bool pressed){
            qDebug() << "Button A" << pressed;
            qDebug()<<"this is a test sentence!";
        });
        connect(m_gamepad, &QGamepad::buttonBChanged, this, [](bool pressed){
            qDebug() << "Button B" << pressed;
        });
        connect(m_gamepad, &QGamepad::buttonXChanged, this, [](bool pressed){
            qDebug() << "Button X" << pressed;
        });
        connect(m_gamepad, &QGamepad::buttonYChanged, this, [](bool pressed){
            qDebug() << "Button Y" << pressed;
        });
        connect(m_gamepad, &QGamepad::buttonL1Changed, this, [](bool pressed){
            qDebug() << "Button L1" << pressed;
        });
        connect(m_gamepad, &QGamepad::buttonR1Changed, this, [](bool pressed){
            qDebug() << "Button R1" << pressed;
        });
        connect(m_gamepad, &QGamepad::buttonL2Changed, this, [](double value){
            qDebug() << "Button L2: " << value;
        });
        connect(m_gamepad, &QGamepad::buttonR2Changed, this, [](double value){
            qDebug() << "Button R2: " << value;
        });
        connect(m_gamepad, &QGamepad::buttonSelectChanged, this, [](bool pressed){
            qDebug() << "Button Select" << pressed;
        });
        connect(m_gamepad, &QGamepad::buttonStartChanged, this, [](bool pressed){
            qDebug() << "Button Start" << pressed;
        });
        connect(m_gamepad, &QGamepad::buttonGuideChanged, this, [](bool pressed){
            qDebug() << "Button Guide" << pressed;
        });
        */
}
