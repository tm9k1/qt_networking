#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QKeyEvent>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->show();
    mServer = nullptr;
}

MainWindow::~MainWindow()
{
    delete ui;
    destroyServer();
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
        qDebug() << "Creating Server";
        mServer = new MyServer();
        if(mServer->startServing()){
            ui->startServerPushButton->setText("Server Created\nClick to Stop");
            ui->serverIpLabel->setText(mServer->getIpAddress());
            ui->serverPortNumberLabel->setText(mServer->getPortNumber());
            ui->sendMessagePushButton->setEnabled(true);
            ui->messageTextEdit->setEnabled(true);
        } else {
            QMessageBox::critical(this, "Error", "Could not start the server.\nPlease check if some other program is using that IP-port combination.");
            destroyServer();
        }
    } else {
        if(mServer) {
            destroyServer();
        }
    }
}

void MainWindow::on_connectPushButton_clicked(bool checked)
{
    if (checked) {
        ui->connectPushButton->setText("Connecting...");
        ui->connectPushButton->setEnabled(false);

        if (ui->udpModeRadioButton->isChecked()) {
            mSocket = new QUdpSocket(this);
        } else {
            mSocket = new QTcpSocket(this);
        }

        connect(mSocket, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
        connect(mSocket, SIGNAL(connected()), this, SLOT(clientConnected()));

        mSocket->connectToHost(QHostAddress(ui->clientIpLineEdit->text()),
                               ui->clientPortLineEdit->text().toInt());

        if(!mSocket->waitForConnected(3000)) {
            QMessageBox::critical(this, "Could not connect", "Could not connect to the given IP-port combination.");
            if(mSocket) {
                delete mSocket;
                mSocket = nullptr;
            }

            ui->connectPushButton->setText("Click to\nConnect");
            ui->connectPushButton->setChecked(false);
        } else {
            ui->showMessagesPushButton->setEnabled(true);
        }
        ui->connectPushButton->setEnabled(true);

    } else {
        if(mSocket) {
            mSocket->close();
            delete mSocket;
        }
    }
}

void MainWindow::destroyServer()
{
    if(!mServer) {
        return;
    }
    qDebug() << "Deleting Server";
    mServer->disconnect();
    delete mServer;
    mServer = nullptr;
    ui->startServerPushButton->setText("Create\nServer");
    ui->startServerPushButton->setChecked(false);
    ui->serverIpLabel->setText("");
    ui->serverPortNumberLabel->setText("");
    ui->sendMessagePushButton->setEnabled(false);
    ui->messageTextEdit->setEnabled(false);
}

void MainWindow::clientDisconnected()
{
    qDebug() << "Client Disconnected";
    ui->connectPushButton->setText("Click to\nConnect");
    ui->connectPushButton->setChecked(false);
}

void MainWindow::clientConnected()
{
    qDebug() << "Client Connected! Yayyyy!!!";
    ui->connectPushButton->setText("Click to\nDisconnect");
    ui->connectPushButton->setChecked(true);

}

void MainWindow::on_sendMessagePushButton_clicked()
{
    QString message = ui->messageTextEdit->toPlainText();
    if(mServer->writeMessage(message)) {
        qDebug() << "Message written to socket.";
    } else {
        QMessageBox::critical(this,"Error", "Could not send the message to client.");
    }
}
