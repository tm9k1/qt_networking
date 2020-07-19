#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QKeyEvent>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), m_server(nullptr), m_clientSocket(nullptr), m_receivedMessages(new QQueue<QString>)
{
    ui->setupUi(this);
    this->show();
}

MainWindow::~MainWindow()
{
    delete ui;
    destroyServer();
    if (m_clientSocket) {
        delete m_clientSocket;
    }
    delete m_receivedMessages;
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
        //        qDebug() << "Creating Server";
        m_server = new MyServer();

        if(m_server->startServing()){
            //        qDebug() << "Server started.";
            ui->startServerPushButton->setText("Server Created\nClick to Stop");
            ui->serverIpLabel->setText(m_server->getIpAddress());
            ui->serverPortNumberLabel->setText(m_server->getPortNumber());
            ui->sendTcpPushButton->setEnabled(true);
        } else {
            QMessageBox::critical(this, "Error", "Could not start the server.\nPlease check if some other program is using that IP-port combination.");
            destroyServer();
        }
    } else {
        if(m_server) {
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
            m_clientSocket = new QUdpSocket(this);
            m_clientSocket->bind(QHostAddress(ui->clientIpLineEdit->text()),
                                 ui->clientPortLineEdit->text().toInt());
            ui->sendUdpPushButton->setEnabled(true);
        } else {
            m_clientSocket = new QTcpSocket(this);
        }

        connect(m_clientSocket, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
        connect(m_clientSocket, SIGNAL(connected()), this, SLOT(clientConnected()));

        m_clientSocket->connectToHost(QHostAddress(ui->clientIpLineEdit->text()),
                               ui->clientPortLineEdit->text().toInt());

        if(!m_clientSocket->waitForConnected(3000)) {
            QMessageBox::critical(this, "Could not connect", "Could not connect to the given IP-port combination.");

            if(m_clientSocket) {
                delete m_clientSocket;
                m_clientSocket = nullptr;
                ui->sendUdpPushButton->setEnabled(false);
            }

            ui->connectPushButton->setText("Click to\nConnect");
            ui->connectPushButton->setChecked(false);

        } else {
            connect(m_clientSocket, SIGNAL(readyRead()), this, SLOT(readMessages()));
            qDebug() << "Looks like Socket connected to the port.";
        }
        ui->connectPushButton->setEnabled(true);

    } else {
        if(m_clientSocket) {
            m_clientSocket->close();
            delete m_clientSocket;
            m_clientSocket = nullptr;
            ui->sendUdpPushButton->setEnabled(false);

        }
    }
}

void MainWindow::destroyServer()
{
    if(!m_server) {
        return;
    }
    //    qDebug() << "Deleting Server";
    m_server->disconnect();
    delete m_server;
    m_server = nullptr;

    ui->startServerPushButton->setText("Create\nServer");
    ui->startServerPushButton->setChecked(false);
    ui->serverIpLabel->setText("");
    ui->serverPortNumberLabel->setText("");
    ui->sendTcpPushButton->setEnabled(false);
}

void MainWindow::clientDisconnected()
{
    //    qDebug() << "Client Disconnected.";
    ui->connectPushButton->setText("Click to\nConnect");
    ui->connectPushButton->setChecked(false);

    ui->clientIpLineEdit->setEnabled(true);
    ui->clientPortLineEdit->setEnabled(true);
    ui->udpModeRadioButton->setEnabled(true);
    ui->tcpModeRadioButton->setEnabled(true);
}

void MainWindow::clientConnected()
{
    //    qDebug() << "Client Connected.";
    ui->connectPushButton->setText("Click to\nDisconnect");
    ui->connectPushButton->setChecked(true);

    ui->clientIpLineEdit->setEnabled(false);
    ui->clientPortLineEdit->setEnabled(false);
    ui->udpModeRadioButton->setEnabled(false);
    ui->tcpModeRadioButton->setEnabled(false);
}

void MainWindow::readMessages()
{
    qDebug() << "readyRead was fired!!";
    qDebug() << "Attempting to read now...";
    if(m_clientSocket->isReadable()) {
        if(m_receivedMessages->count() == messagesLimit) {
            m_receivedMessages->dequeue();
        }

        m_receivedMessages->enqueue(m_clientSocket->readAll());

    } else {
        //        qDebug() << "Could not read the message from socket.";
    }
}

void MainWindow::on_showMessagesPushButton_clicked()
{

    QString string = "";
        for(QString &temp: *m_receivedMessages) {
            string.append(temp);
            string.append("\n");
        }

    if(string.isEmpty()) {
        string = "No messages yet.";
    }

    QMessageBox(QMessageBox::Information,
                QString("Last %1 messages").arg(qMin(m_receivedMessages->count(),messagesLimit)),
                string,
                QMessageBox::Ok,
                this).exec();
}

void MainWindow::on_sendTcpPushButton_clicked()
{
    QString message = ui->messageTextEdit->toPlainText();
    if(message.isEmpty()) {
        return;
    }

    if(m_server->writeTCPMessage(message)) {
                qDebug() << "Message written to socket.";
    } else {
        QMessageBox::critical(this,"Error", "Could not send the message");
    }
}

void MainWindow::on_sendUdpPushButton_clicked()
{
    QString message = ui->messageTextEdit->toPlainText();
    if(message.isEmpty()) {
        return;
    }
    QByteArray data;
    data.append(message);
    if(m_clientSocket->write(data) != -1) {
                qDebug() << "Message sent";
    } else {
        QMessageBox::critical(this,"Error", "Could not send the message");
    }

}

