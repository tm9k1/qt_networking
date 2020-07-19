#include "myserver.h"
#include <QMessageBox>

MyServer::MyServer(QObject *parent) : QObject(parent)
{
    m_server = new QTcpServer(this);
}


void MyServer::newConnection()
{
    m_socket = m_server->nextPendingConnection();
}

MyServer::~MyServer()
{
    m_server->close();
    delete m_server;
}

bool MyServer::startServing()
{
    m_server->listen(QHostAddress::Any, 1234);
    connect(m_server,SIGNAL(newConnection()), this, SLOT(newConnection()));

    if(m_server->isListening()) {
        qDebug() << "Server started.";
        m_ipAddress = m_server->serverAddress().toString();
        m_portNumber = QString::number(m_server->serverPort());
        return true;
    } else {
        return false;
    }
}

QString MyServer::getIpAddress()
{
    return m_ipAddress;
}

QString MyServer::getPortNumber()
{
    return m_portNumber;
}

bool MyServer::writeMessage(QString &message)
{
    m_socket->write(message.toStdString().c_str());
    return m_socket->waitForBytesWritten(30000);
}
