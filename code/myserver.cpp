#include "myserver.h"
#include <QMessageBox>

MyServer::MyServer(QObject *parent) : QObject(parent), m_tcpSocket(nullptr), m_udpSocket(new QUdpSocket(this))
{
    m_server = new QTcpServer(this);

    connect(m_server,SIGNAL(newConnection()), this, SLOT(newConnection()));
}


void MyServer::newConnection()
{
    m_tcpSocket = m_server->nextPendingConnection();
}

MyServer::~MyServer()
{
    m_server->close();
    delete m_server;
    delete m_udpSocket;
}

bool MyServer::startServing()
{
    m_server->listen(QHostAddress::Any, 1234);

    if(m_server->isListening()) {

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

bool MyServer::writeTCPMessage(QString &message)
{
    if(m_tcpSocket) {
        m_tcpSocket->write(message.toStdString().c_str());
        return m_tcpSocket->waitForBytesWritten(30000);
    } else {
        return false;
    }
}

