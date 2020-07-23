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
    void on_showMessagesPushButton_clicked();

    void on_sendMessagePushButton_clicked();

    void on_tcpModeRadioButton_toggled(bool checked);

protected:
    void keyPressEvent(QKeyEvent *e);

private:
    Ui::MainWindow *ui;
    MyServer *m_server;
    QAbstractSocket *m_clientSocket;
    int maxMessages = 10;

};

#endif // MAINWINDOW_H
