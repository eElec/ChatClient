#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QtDebug>

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:  //Weird Preprocessor Stuff
    void on_sendButton_clicked();   //All Events are Stored Here
    void on_pushButton_clicked();
    void recieveData();

public slots:
    void newConnection();

private:
    //Some Variables
    QString prevMsg;
    QTcpServer *server;
    QTcpSocket *socket;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
