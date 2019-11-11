#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->lineEdit_Port->setText("7654");
    ui->pushButton_Send->setEnabled(false);

    server = new QTcpServer();
    s = new serialconnect();
    s->openPort();
    //连接信号槽
    connect(server,&QTcpServer::newConnection,this,&MainWindow::server_New_Connect);
}

MainWindow::~MainWindow()
{
    server->close();
    server->deleteLater();
    delete ui;
}

void MainWindow::on_pushButton_Listen_clicked()
{
    if(ui->pushButton_Listen->text() == tr("listen"))
    {
        //从输入框获取端口号
        int port = ui->lineEdit_Port->text().toInt();

        //监听指定的端口
        if(!server->listen(QHostAddress::Any, port))
        {
            //若出错，则输出错误信息
            qDebug()<<server->errorString();
            return;
        }
        //修改按键文字
        ui->pushButton_Listen->setText("not listen");
        qDebug()<< "Listen succeessfully!";
    }
    else
    {
/*
        //如果正在连接（点击侦听后立即取消侦听，若socket没有指定对象会有异常，应修正——2017.9.30）
        if(socket->state() == QAbstractSocket::ConnectedState)
        {
            //关闭连接
            socket->disconnectFromHost();
        }
*/
        socket->abort();
        //取消侦听
        server->close();
        //修改按键文字
        ui->pushButton_Listen->setText("listen");
        //发送按键失能
        ui->pushButton_Send->setEnabled(false);
    }

}

void MainWindow::on_pushButton_Send_clicked()
{
    qDebug() << "Send: " << ui->textEdit_Send->toPlainText();
    //获取文本框内容并以ASCII码形式发送
    socket->write(ui->textEdit_Send->toPlainText().toLatin1());
    socket->flush();
}

void MainWindow::server_New_Connect()
{
    //获取客户端连接
    socket = server->nextPendingConnection();
    //连接QTcpSocket的信号槽，以读取新数据
    QObject::connect(socket, &QTcpSocket::readyRead, this, &MainWindow::socket_Read_Data);
    QObject::connect(socket, &QTcpSocket::disconnected, this, &MainWindow::socket_Disconnected);
    //发送按键使能
    ui->pushButton_Send->setEnabled(true);

    qDebug() << "A Client connect!";
}

void MainWindow::socket_Read_Data()
{
    QByteArray buffer;
    //读取缓冲区数据
    buffer = socket->readAll();
    if(!buffer.isEmpty())
    {
        QString str = ui->textEdit_Recv->toPlainText();
        QString order_str = QObject::tr(buffer);
        str+=order_str+"\n";
        QByteArray order = hexStringtoByteArray(order_str);
        s->send(order);
        //刷新显示
        ui->textEdit_Recv->setText(str);
    }
}

void MainWindow::socket_Disconnected()
{
    //发送按键失能
    ui->pushButton_Send->setEnabled(false);
    qDebug() << "Disconnected!";
}


QByteArray MainWindow::hexStringtoByteArray(QString hex){
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
void MainWindow::formatString(QString& org, int n, const QChar& ch){
    int size = org.size();
    int space = qRound(size*1.0/n+0.5)-1;
    if(space<=0){
        return;
    }
    for(int i=0,pos=n;i<space;++i,pos+=(n+1)){
        org.insert(pos,ch);
    }
}

void MainWindow::on_pushButton_clicked()
{

    if(s->is_port_open()){
        s->closePort();
        ui->pushButton->setText("open port");
    }
    else{
        s->openPort();
        ui->pushButton->setText("close port");
    }
}
