#include "socketsend.h"

socketsend::socketsend()
{
    socket = new QTcpSocket();

    //连接信号槽
//    QObject::connect(socket, &QTcpSocket::readyRead, this, &socketsend::socket_Read_Data);
//    QObject::connect(socket, &QTcpSocket::disconnected, this, &socketsend::disconnect);
}
socketsend::~socketsend(){
    delete this->socket;
}
void socketsend::connect(){
    QString IP;
    int port;

    //获取IP地址
    IP = "192.168.12.31";
    //获取端口号
    port = 8888;

    //取消已有的连接
    socket->abort();
    //连接服务器
    socket->connectToHost(IP, port);

    //等待连接成功
    if(!socket->waitForConnected(30000))
    {
        qDebug() << "Connection failed!";
        return;
    }
    qDebug() << "Connect successfully!";
    return;
}
void socketsend::disconnect(){
    socket->disconnectFromHost();
}
void socketsend::sendinfo(QString info){
    qDebug() << "Send: " << info;
    QByteArray a ;

    socket->write(info.toLatin1());
    socket->flush();
}
