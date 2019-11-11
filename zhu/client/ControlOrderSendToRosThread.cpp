#include "ControlOrderSendToRosThread.h"
#include <QObject>
#include<QDebug>
ControlOrderSendToRosThread::ControlOrderSendToRosThread(QObject *parent):QThread(parent){
    m_isCanRun=true;
}

ControlOrderSendToRosThread::~ControlOrderSendToRosThread(){
    qDebug()<<"ControlOrderSendToRosThread for sending car's control orders has been destoried";
}

void ControlOrderSendToRosThread::stopImmediately(){

    QMutexLocker locker(&m_threadRunLock);
    m_isCanRun = false;
    locker.unlock();
}

void ControlOrderSendToRosThread::run(){
    int argc=0;
    char **argv={};
    ros::init(argc, argv, "order_send_node");
    ros::NodeHandle n("~");
    ros::Publisher order_simulate_pub = n.advertise<geometry_msgs::QuaternionStamped>("/tiny_car_order", 1000);
    ros::Rate loop_rate(1000);
    while(ros::ok()){
//            unsigned char tt[10]={0xFF,0xFE,1,2,3,4,5,6,7,8};
            //communication->sendMessage(tinyCarCO.getCharOrder());
        geometry_msgs::QuaternionStamped order;
        //ros::Time currentTime = ros::Time::now();
        order.quaternion.x=leftOrientation;
        order.quaternion.w=rightOrientation;
        order.quaternion.y=leftSpeed;
        order.quaternion.z=rightSpeed;
        order_simulate_pub.publish(order);

        QMutexLocker locker2(&m_threadRunLock);
        if(!m_isCanRun)//在每次循环判断是否可以运行，如果不行就退出循环
        {
            locker2.unlock();
            break;
        }
        locker2.unlock();
        ros::spinOnce();
        loop_rate.sleep();
    }
}

void ControlOrderSendToRosThread::setOrder(int rightSpeed,int leftSpeed,int rightOrientation,int leftOrientation){
    this->leftSpeed=leftSpeed;
    this->leftOrientation=leftOrientation;
    this->rightSpeed=rightSpeed;
    this->rightOrientation=rightOrientation;
    qDebug()<<"important!!!!!!!"<<"rightOrientation:"<<rightOrientation<<" leftOrientation:"<<leftOrientation<<" leftSpeed:"<<leftSpeed<<" rightSpeed:"<<rightSpeed;
}
