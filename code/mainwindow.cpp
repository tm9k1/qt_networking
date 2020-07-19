#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->show();

}

MainWindow::~MainWindow()
{
    delete ui;
    if(mServer) {
        mServer->disconnect();
        delete mServer;
    }

}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Escape) {
        QCoreApplication::quit();
    }
}

void MainWindow::on_startServerPushButton_clicked(bool checked)
{
    if (checked) {
        mServer = new MyServer();
        ui->startServerPushButton->setText("Server Created\nClick to Stop");
        ui->serverIpLabel->setText(mServer->getIpAddress());
        ui->serverPortNumberLabel->setText(mServer->getPortNumber());
    }
    else {
        if(mServer) {
            mServer->disconnect();
            delete mServer;
            qDebug() << "Server Stopped";
            ui->startServerPushButton->setText("Create\nServer");
            ui->serverIpLabel->setText("");
            ui->serverPortNumberLabel->setText("");

        }
    }
}
