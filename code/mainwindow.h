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

public slots:
protected:
    void keyPressEvent(QKeyEvent *e);

private:
    Ui::MainWindow *ui;
    MyServer *mServer;
};

#endif // MAINWINDOW_H
