//mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include "serialconnect.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    serialconnect* s;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QByteArray hexStringtoByteArray(QString hex);
    void formatString(QString& org, int n = 2, const QChar& ch = QChar(' '));
private slots:
    void on_pushButton_Listen_clicked();

    void on_pushButton_Send_clicked();

    void server_New_Connect();

    void socket_Read_Data();

    void socket_Disconnected();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QTcpServer* server;
    QTcpSocket* socket;
};

#endif // MAINWINDOW_H
