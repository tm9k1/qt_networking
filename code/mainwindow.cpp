#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QKeyEvent>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), m_server(nullptr), m_clientSocket(nullptr)
{
    ui->setupUi(this);
    this->show();
}

MainWindow::~MainWindow()
{
    delete ui;
    if (m_clientSocket) {
        delete m_clientSocket;
    }
    if (m_server) {
        delete m_server;
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
        //        qDebug() << "Creating Server";
        m_server = new MyServer();

        if (!m_server->isListening()) {
            QMessageBox::critical(this, "Error", "Could not start the server.\nPlease check if some other program is using that IP-port combination.");
            delete m_server;
            m_server = nullptr;
            ui->startServerPushButton->setChecked(false);
        }
    } else {
        if(m_server) {
            //            qDebug() << "Destroying server";
            m_server->disconnect();
            delete m_server;
            m_server = nullptr;
        }
    }
    ui->startServerPushButton->setText((checked) ? ("Server Created\nClick to Stop") : ("Create\nServer"));
    ui->serverIpLabel->setText((checked) ? (m_server->getIpAddress().toString()) : (""));
    ui->serverPortLabel->setText((checked) ? (QString::number(m_server->getPortNumber())) : (""));
    ui->showMessagesPushButton->setEnabled(checked);
    ui->portNumberLabel->setEnabled(checked);
    ui->ipAddressLabel->setEnabled(checked);

}

void MainWindow::on_connectPushButton_clicked(bool checked)
{
    if (checked) {
        // Disable the button once clicked to avoid spamming by user
        ui->connectPushButton->setText("Connecting...");
        ui->connectPushButton->setEnabled(false);

        // TCP Mode
        if (ui->tcpModeRadioButton->isChecked()) {
            m_clientSocket = new QTcpSocket(this);
        }

        connect(m_clientSocket, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
        connect(m_clientSocket, SIGNAL(connected()), this, SLOT(clientConnected()));

        m_clientSocket->connectToHost(QHostAddress(ui->clientIpLineEdit->text()),
                                      ui->clientPortLineEdit->text().toInt());

        // if could not connect
        if(!m_clientSocket->waitForConnected(3000)) {
            QMessageBox::critical(this, "Could not connect", "Could not connect to the given IP-port combination.");

            if(m_clientSocket) {
                delete m_clientSocket;
                m_clientSocket = nullptr;
            }

            ui->connectPushButton->setText("Click to\nConnect");
            ui->connectPushButton->setChecked(false);

        } else {
            //            qDebug() << "Connected successfully to the host.";
        }

        // ok we have processed the connect request. Allow user to make another request now.
        ui->connectPushButton->setEnabled(true);

    } else {
        if(m_clientSocket) {
            m_clientSocket->close();
            delete m_clientSocket;
            m_clientSocket = nullptr;
        }
    }
}

void MainWindow::on_sendMessagePushButton_clicked()
{
    QString message = ui->messageTextEdit->toPlainText();
    if(message.isEmpty()) {
        return;
    }
    //    qDebug() << "Attempting to send message now...";
    int res;
    if (ui->udpModeRadioButton->isChecked()) {  // UDP Mode
        m_clientSocket = new QUdpSocket();
        res = reinterpret_cast<QUdpSocket *>(m_clientSocket)->writeDatagram(
                    message.toStdString().c_str(),
                    QHostAddress(ui->clientIpLineEdit->text()),
                    ui->clientPortLineEdit->text().toInt());
        delete m_clientSocket;
        m_clientSocket = nullptr;
    } else {    // TCP Mode
        res = reinterpret_cast<QTcpSocket *>(m_clientSocket)->write(message.toStdString().c_str());
    }
    if (res != -1) {
        //        qDebug() << "Message sent successfully!";
    } else {
        QMessageBox::critical(this,"Please try again","Could not send message");
    }
}

void MainWindow::on_showMessagesPushButton_clicked()
{
    QPair<QString, int> messages;
    messages = m_server->getMessages(10);

    QMessageBox(QMessageBox::NoIcon,
                QString("Last %1 messages").arg(messages.second),
                messages.first,
                QMessageBox::Ok,
                this).exec();
}

void MainWindow::clientDisconnected()
{
    //    qDebug() << "Client disconnected.";
    ui->connectPushButton->setText("Click to\nConnect");
    ui->connectPushButton->setChecked(false);

    // enable client configs once disconnected
    ui->clientIpLineEdit->setEnabled(true);
    ui->clientPortLineEdit->setEnabled(true);
    ui->udpModeRadioButton->setEnabled(true);
    ui->tcpModeRadioButton->setEnabled(true);

    // disable message box
    ui->sendMessagePushButton->setEnabled(true);
    ui->messageTextEdit->setEnabled(true);
}

void MainWindow::clientConnected()
{
    qDebug() << "Client Connected.";
    ui->connectPushButton->setText("Click to\nDisconnect");
    ui->connectPushButton->setChecked(true);

    // disable client configs once connected
    ui->clientIpLineEdit->setEnabled(false);
    ui->clientPortLineEdit->setEnabled(false);
    ui->udpModeRadioButton->setEnabled(false);
    ui->tcpModeRadioButton->setEnabled(false);

    // allow sending messages
    ui->sendMessagePushButton->setEnabled(true);
    ui->messageTextEdit->setEnabled(true);
}

void MainWindow::on_tcpModeRadioButton_toggled(bool checked)
{
    ui->messageTextEdit->setEnabled(!checked);
    ui->sendMessagePushButton->setEnabled(!checked);
    ui->connectPushButton->setEnabled(checked);
}
