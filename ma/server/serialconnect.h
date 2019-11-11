#ifndef SERIALCONNECT_H
#define SERIALCONNECT_H
#include <QSerialPort>
#include <QSerialPortInfo>

class serialconnect : public QObject
{
    Q_OBJECT

public:
    serialconnect(QObject *parent = nullptr);
    ~serialconnect();
    QStringList getPortNameList();//获取所有可用的串口列表
    void openPort();//打开串口
    void closePort();
    bool is_port_open();
public slots:
    QByteArray receiveInfo();
public slots:
    void send(QByteArray order);
private:
    QSerialPort* m_serialPort; //串口类
    QStringList m_portNameList;

//    QComboBox* m_PortNameComboBox;
//    QPushButton* m_OpenPortButton;
};

#endif // SERIALCONNECT_H


