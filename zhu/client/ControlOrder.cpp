#include "ControlOrder.h"
#include "QDebug"
#include<QString>
void ControlOrder::init(){
    this->speed=0;
    this->lastTurnRange=0;
    this->turnRange=0;
    this->gear=0;
    this->lightSignal=0;
    this->horn=0;
}

ControlOrder& ControlOrder::setSpeed(int speed){
    if(speed>CONTROLORDER_MAX_SPEED){
        speed=CONTROLORDER_MAX_SPEED;
    }else if(speed<CONTROLORDER_MIN_SPEED){
        speed=CONTROLORDER_MIN_SPEED;
    }
    this->speed=speed;
    return *this;
}

ControlOrder& ControlOrder::setTurnRange(int turnRange){
    if(turnRange>CONTROLORDER_MAX_TURN_RANGE){
        turnRange=CONTROLORDER_MAX_TURN_RANGE;
    }else if(turnRange<CONTROLORDER_MIN_TURN_RANGE){
        turnRange=CONTROLORDER_MIN_TURN_RANGE;
    }
    this->lastTurnRange=this->turnRange;
    this->turnRange=turnRange;
    return *this;
}
ControlOrder& ControlOrder::setLastTurnRange(int lastTurnRange){
    this->lastTurnRange=lastTurnRange;
    return *this;
}

ControlOrder& ControlOrder::setGear(qint8 gear){
    if(gear==CONTROLORDER_GEAR_ZERO||gear==CONTROLORDER_GEAR_FORWARD||gear==CONTROLORDER_GEAR_BACKWARD){
        this->gear=gear;
    }else{
        qDebug("geal=%d,setting error",gear);
    }
    return *this;
}

ControlOrder& ControlOrder::setLightSignal(qint8 lightSignal){
    if(lightSignal==CONTROLORDER_LIGHT_ALL_OFF||lightSignal==CONTROLORDER_LIGHT_LEFT_ON||lightSignal==CONTROLORDER_LIGHT_RIGHT_ON){
        this->lightSignal=lightSignal;
    }else{
        qDebug("lightSignal=%d,setting error",lightSignal);
    }
    return *this;
}

ControlOrder& ControlOrder::setHorn(qint8 horn){
    if(horn==CONTROLORDER_HORN_OFF||horn==CONTROLORDER_HORN_ON){
        this->horn=horn;
    }else{
        qDebug("horn=%d,setting error",horn);
    }
    return *this;
}
/**
    Normal       ---->       Tiny
    speed[-32768,-1]    leftSpeed&&rightSpeed=0//刹车
    speed[0，32767]      leftSpeed&&rightSpeed[0,255]
    turnRange[0,32767]  rightSpeed=(1-k1)*leftSpeed,k1[0,1]//右转
    turnRange[-1,-32768] leftSpeed=(1-k2)*rightSpeed,k2[0,1]//左转
    if(lastTurnRange*curTurnRange<0)  leftSpeed=rightSpeed=max(leftSpeed,rightSpeed)
    gear==2             leftOrientation=rightOrientation=1
    gear!=2             leftOrientation=rightOrientation=1
 */
void ControlOrder::NormalCO2TinyCarCO(ControlOrder& normalCO,TinyCarCO& tinyCarCO){
    int speed=normalCO.getSpeed();
    if(speed<=0){
        tinyCarCO.setLeftSpeed(0);
        tinyCarCO.setRightSpeed(0);
    }else{
        speed=speed/32767.0*255;
        tinyCarCO.setLeftSpeed(speed);
        tinyCarCO.setRightSpeed(speed);
    }
    if(normalCO.getTurnRange()*normalCO.getLastTurnRange()<0){
        int leftSpeed=tinyCarCO.getLeftSpeed();
        int rightSpeed=tinyCarCO.getRightSpeed();
        if(leftSpeed>rightSpeed){
            tinyCarCO.setLeftSpeed(leftSpeed);
            tinyCarCO.setRightSpeed(leftSpeed);
        }
    }
    int turnRange=normalCO.getTurnRange();
    if(turnRange>=0){
        tinyCarCO.setRightSpeed((1-turnRange/32767.0)*tinyCarCO.getLeftSpeed());
    }
    if(turnRange<0){
        tinyCarCO.setLeftSpeed((1-(turnRange+1)/(-32767.0))*tinyCarCO.getRightSpeed());
    }
    if(normalCO.getGear()==CONTROLORDER_GEAR_BACKWARD){
        tinyCarCO.setLeftOrientation(TINYCARCO_ORIENTATION_BACKWARD);
        tinyCarCO.setRightOrientation(TINYCARCO_ORIENTATION_BACKWARD);
    }else{
        tinyCarCO.setLeftOrientation(TINYCARCO_ORIENTATION_FORWARD);
        tinyCarCO.setRightOrientation(TINYCARCO_ORIENTATION_FORWARD);
    }

}
/**
    Normal        <----      Tiny
    speed[-32768] leftSpeed&&rightSpeed=0//刹车
    speed[0，32767]      max(leftSpeed,rightSpeed) [0,255]
    turnRange[0,32767]   leftSpeed>rightSpeed,k1=rightSpeed/leftSpeed,k1[0,1]//右转
    turnRange[-1,-32768] rightSpeed>leftSpeed,k2=leftSpeed/rightSpeed,k2[0,1]//左转
    gear==2             leftOrientation=rightOrientation=1
    gear!=2             leftOrientation=rightOrientation=1
 */
void ControlOrder::TinyCarCO2NormalCO(TinyCarCO& tinyCarCO,ControlOrder& normalCO){
    int leftSpeed=tinyCarCO.getLeftSpeed();
    int rightSpeed=tinyCarCO.getRightSpeed();
    if(leftSpeed==0&&rightSpeed==0){
        normalCO.setSpeed(CONTROLORDER_MIN_SPEED);
    }else{
        int maxSpeed=leftSpeed>rightSpeed?leftSpeed:rightSpeed;
        normalCO.setSpeed(CONTROLORDER_MAX_SPEED*maxSpeed*1.0/TINYCARCO_MAX_SPEED);

        if(rightSpeed<leftSpeed){//右转
            normalCO.setTurnRange(CONTROLORDER_MAX_TURN_RANGE*(1-rightSpeed*1.0/leftSpeed));
        }else if(rightSpeed>leftSpeed){//左转
            normalCO.setTurnRange(-1+(CONTROLORDER_MIN_TURN_RANGE+1)*(1-leftSpeed*1.0/rightSpeed));
        }
    }
    int leftOrientation=tinyCarCO.getLeftOrientation();
    int rightOrientation=tinyCarCO.getRightOrientation();
    if(leftOrientation==rightOrientation){
        if(leftOrientation==1){
            normalCO.setGear(2);
        }else{
            normalCO.setGear(1);
        }
    }else{
        qDebug()<<"error:tiny car leftOrientation!= rightOrientation";
    }

}
void ControlOrder::printInfo(){
    qDebug("Control Order: speed:%d,turnRange:%d,gear:%d %s,lightSignal:%d %s,hore:%d %s",
           speed,turnRange,
           gear,gear==0?"neutral":gear==1?("forward"):("backward"),
           lightSignal,lightSignal==0?("all light turn off"):lightSignal==1?"right light on":"left light on",
           horn,horn==0?"mute":"whistle"
           );
}
