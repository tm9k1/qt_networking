#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QQueue>

#include <myserver.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_startServerPushButton_clicked(bool checked);
    void clientConnected();
    void clientDisconnected();
    void on_connectPushButton_clicked(bool checked);
    void on_sendMessagePushButton_clicked();
    void readMessages();

    void on_showMessagesPushButton_clicked();

protected:
    void keyPressEvent(QKeyEvent *e);

private:
    Ui::MainWindow *ui;
    MyServer *mServer;
    QAbstractSocket *mSocket;
    QQueue<QString> *mMessages;
    int messagesLimit = 10;

    void destroyServer();

};

#endif // MAINWINDOW_H
