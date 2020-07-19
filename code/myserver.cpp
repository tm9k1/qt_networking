#include "myserver.h"
#include <QMessageBox>

MyServer::MyServer(QObject *parent) : QObject(parent)
{
    m_server = new QTcpServer(this);
    connect(m_server,SIGNAL(newConnection()), this, SLOT(newConnection()));
    m_server->listen(QHostAddress::Any, 1234);
    if(m_server->isListening()) {
        qDebug() << "Server started.";
        m_ipAddress = m_server->serverAddress().toString();
        m_portNumber = QString::number(m_server->serverPort());
    } else {
        qDebug() << "Server could not start with the given IP Address and port number.";
    }
}


void MyServer::newConnection()
{
    QTcpSocket *socket = m_server->nextPendingConnection();

    socket->write("Hello Client\r\n");
    socket->flush();

    socket->waitForBytesWritten(3000);

    socket->close();
}

QString MyServer::getIpAddress()
{
    return m_ipAddress;
}

QString MyServer::getPortNumber()
{
    return m_portNumber;
}

