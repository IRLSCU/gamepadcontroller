#ifndef CONTROLORDERSENDTOROSTHREAD_H
#define CONTROLORDERSENDTOROSTHREAD_H
#define CONTROLORDERSENDTOROSTHREAD_BOLCKTIME 200

#include<QThread>
#include<QMutex>
#include<QList>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/QuaternionStamped.h"
/**
 * @brief 将开启一个线程，将汽车控制指令送往对应ROS
 */
class ControlOrderSendToRosThread : public QThread
{
    Q_OBJECT
private:
    QMutex m_threadRunLock;
    QMutex m_enableLock;
    QMutex m_controlOrderLock;
    bool m_isCanRun;//控制线程开闭
    int leftSpeed;
    int rightSpeed;
    int leftOrientation;
    int rightOrientation;
public:
    ControlOrderSendToRosThread(QObject *parent);
    ~ControlOrderSendToRosThread();
    void stopImmediately();
    void setOrder(int rightSpeed,int leftSpeed,int rightOrientation,int leftOrientation);
    void run();
signals:
};

#endif // CONTROLORDERSENDTOROSTHREAD_H
