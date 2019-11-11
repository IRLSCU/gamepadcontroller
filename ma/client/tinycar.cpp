#include "tinycar.h"
#include <QDebug>
Tinycar::Tinycar(){
    init();
}

Tinycar& Tinycar::setLeftOrientation(QString leftOrientation){

    this->leftOrientation=leftOrientation;

    return *this;
}

Tinycar& Tinycar::setRightOrientation(QString rightOrientation){

    this->rightOrientation=rightOrientation;
    return *this;
}

void Tinycar::init(){
    this->left_c_speed=0;
    this->right_c_speed=0;
    this->left_speed_str = "00";
    this->right_speed_str = "00";
    this->leftOrientation="00";
    this->rightOrientation="00";
}

void Tinycar::printInfo(){
    qDebug("Tiny Car Control Order: leftSpeed:%d,rightSpeed:%d,leftOrientation:%d,rightOrientation:%d",
           left_c_speed,right_c_speed,leftOrientation,rightOrientation);
}


Tinycar& Tinycar::goSpeedUp(quint8){
    //TODO
}
Tinycar& Tinycar::backSpeedUp(quint8){
    //TODO
}
Tinycar& Tinycar::turnleft(quint8){
    //TODO
}
Tinycar& Tinycar::turnright(quint8){
    //TODO
}
Tinycar& Tinycar::start(quint8){
    //TODO
}
Tinycar& Tinycar::stop(quint8){
    //TODO
}
