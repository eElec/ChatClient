#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    server = nullptr;
    prevMsg = "";
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_sendButton_clicked()
{
    QString msg = ui->msgEdit->text();
    msg = "<div align=\"right\">" + msg + "</div>";

    QString currentTime = QTime::currentTime().toString();
    currentTime = "&nbsp;&nbsp;<font color=\"red\">" + currentTime + "</font>";

    ui->showText->setHtml(prevMsg + msg + currentTime);
    this->prevMsg += msg + currentTime;
}

void MainWindow::on_pushButton_clicked()
{

    if(server != nullptr){
        delete server;
    }
    QString IP = ui->ipEdit->text();
    ushort port = ui->portEdit->text().toUShort();

    this->server = new QTcpServer(this);
    connect(server,  SIGNAL(newConnection()), this, SLOT(newConnection()));

    if(!server->listen(QHostAddress::LocalHost, port)){
        this->prevMsg = "<font color=\"red\">Connection Failed</font>";
        ui->showText->setHtml(prevMsg);
    }
    else{
        this->prevMsg = "<font color=\"green\">Connected To 127.0.0.1:" + QString::number(port) +  "</font>";
        ui->showText->setHtml(prevMsg);
    }
}

void MainWindow::newConnection(){

    this->socket = new QTcpSocket(this);
    socket = server->nextPendingConnection();
    const char *welcome = "Welcome\n";
    socket->write(welcome, qstrlen(welcome));
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::recieveData);
}

void MainWindow::recieveData(){
    QString msg_Data;

    if (socket->canReadLine()) {
        msg_Data = socket->readLine();
        qDebug() << "Got Data: " << msg_Data;
        QString userName = "";
        QString msg = "";
        int i=0;
        for(; msg_Data[i]!=':' && msg_Data[i]!="\n"; i++){
            userName.append(msg_Data[i]);
            if(msg_Data[i+1] == "\n"){
                return;
            }
        }
        i += 2;
        for(; msg_Data[i]!="\n"; i++){
            msg.append(msg_Data[i]);
        }

        qDebug() << "UserName: " << userName << "\nMsg: " << msg << "\n";

        QString currentTime = QTime::currentTime().toString();
        currentTime = "<font color=\"blue\">" + currentTime + "</font>&nbsp;&nbsp;";
        msg = "<div align=\"left\">" + currentTime +  msg + "</div>";

        ui->showText->setHtml(prevMsg + msg);
        this->prevMsg += msg;
    }
}
