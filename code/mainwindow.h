#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    void destroyServer();
    void on_sendMessagePushButton_clicked();

protected:
    void keyPressEvent(QKeyEvent *e);

private:
    Ui::MainWindow *ui;
    MyServer *mServer;
    QAbstractSocket * mSocket;
};

#endif // MAINWINDOW_H
