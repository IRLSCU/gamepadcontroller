#include "serialconnect.h"

#include <QByteArray>

#include <QDebug>

serialconnect::serialconnect(QObject *parent):
    QObject(parent)
{
    m_serialPort = new QSerialPort();



    m_portNameList = getPortNameList();
}

serialconnect::~serialconnect(){
    closePort();
    delete m_serialPort;
}



QStringList serialconnect::getPortNameList()
{
    QStringList m_serialPortName;
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        m_serialPortName << info.portName();
        qDebug()<<"serialPortName:"<<info.portName();
    }
    qDebug()<<"add list finished";
    return m_serialPortName;
}

void serialconnect::openPort()
{
    if(m_serialPort->isOpen())//如果串口已经打开了 先给他关闭了
    {
        m_serialPort->clear();
        m_serialPort->close();
    }


    m_serialPort->setPortName("/dev/ttyUSB0");//当前选择的串口名字
//    m_serialPort->setPortName("/dev/pts/24");//当前选择的串口名字
    if(!m_serialPort->open(QIODevice::ReadWrite))//用ReadWrite 的模式尝试打开串口
    {
        qDebug()<<"打开失败!";
        return;
    }
    qDebug()<<"串口打开成功!";

    m_serialPort->setBaudRate(QSerialPort::Baud115200,QSerialPort::AllDirections);//设置波特率和读写方向
    m_serialPort->setDataBits(QSerialPort::Data8);      //数据位为8位
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);//无流控制
    m_serialPort->setParity(QSerialPort::NoParity); //无校验位
    m_serialPort->setStopBits(QSerialPort::OneStop); //一位停止位

//    connect(m_serialPort,SIGNAL(readyRead()),this,SLOT(receiveInfo()));
}

void serialconnect:: closePort(){
    if (m_serialPort->isOpen())
    {
        m_serialPort->close();
        qDebug()<<"the serial port has been closed!";
    }
}

bool serialconnect:: is_port_open(){
    return m_serialPort->isOpen();
}

//接收到单片机发送的数据进行解析
QByteArray serialconnect::receiveInfo()
{
    QByteArray state = m_serialPort->readLine(14);//小车现在车轮速度转向数据
//     qDebug()<<"receive info 40:"<<m_serialPort->readLine(14);
//    QByteArray a_current_speed;
//    a_current_speed.resize(1);//A车轮速度
//    a_current_speed[0] = state[2];
//    QByteArray b_current_speed;
//    b_current_speed.resize(1);//B车轮速度
//    b_current_speed[0] = 0x12;
//    QByteArray a_current_orientation;
//    a_current_orientation.resize(1);//A车轮方向
//    a_current_orientation[0] = 0x01;
//    QByteArray b_current_orientation;
//    b_current_orientation.resize(1);//B车轮方向
//    b_current_orientation[0] = 0x01;

//    qDebug()<<"receive info:"<<a_current_speed.toInt();
//    qDebug()<<"receive info:"<<state.toHex();
//    int s  = state[0];
    qDebug()<<"receive info:"<<state;
    return state;

}

void serialconnect::send(QByteArray order)
{
    m_serialPort->write(order);
}

