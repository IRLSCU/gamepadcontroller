#ifndef SOCKETSEND_H
#define SOCKETSEND_H

#include <QString>
#include <QTcpSocket>
class socketsend
{
public:
    QTcpSocket *socket;
    socketsend();
    ~socketsend();
    void connect();
    void disconnect();
    void sendinfo(QString info);
};

#endif // SOCKETSEND_H
