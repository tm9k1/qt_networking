#include "myserver.h"
#include <QMessageBox>

MyServer::MyServer(QObject *parent) : QObject(parent),
    m_server(new QTcpServer(this)), m_receivedMessages(new QList<Message>),
    m_tcpSocket(nullptr), m_udpSocket(new QUdpSocket(this))
{
    // Set up listening for TCP connections
    m_server->listen(m_ipAddress, m_portNumber);
    if(!m_server->isListening()) {
        return;
    }
    connect(m_server,&QTcpServer::newConnection, this, &MyServer::newConnection);

    // Set up listening for UDP packets
    m_udpSocket->bind(m_ipAddress, m_portNumber);
    connect(m_udpSocket, &QUdpSocket::readyRead, this, &MyServer::readMessage);
}

MyServer::~MyServer()
{
    m_server->close();
    if(m_server) {
        delete m_server;
    }
    if(m_udpSocket) {
        delete m_udpSocket;
    }
}

bool MyServer::isListening()
{
    return m_server->isListening();
}

void MyServer::newConnection()
{
    m_tcpSocket = m_server->nextPendingConnection();
    connect(m_tcpSocket, &QTcpSocket::readyRead, this, &MyServer::readMessage);
}

const QHostAddress MyServer::getIpAddress()
{
    return m_ipAddress;
}

quint16 MyServer::getPortNumber()
{
    return m_portNumber;
}

void MyServer::readMessage()
{
    while (m_tcpSocket->bytesAvailable()) {
        m_receivedMessages->append(Message(m_tcpSocket->readAll(),
                                           m_tcpSocket->localAddress().toString(),
                                           QString::number(m_tcpSocket->localPort())));
    }

    while (m_udpSocket->hasPendingDatagrams()) {
        QByteArray buf;
        QHostAddress *srcHost = new QHostAddress;
        quint16 *srcPort = new quint16;
        buf.resize(m_udpSocket->pendingDatagramSize());

        m_udpSocket->readDatagram(
                    buf.data(),
                    buf.size(),
                    srcHost,
                    srcPort
                    );

        m_receivedMessages->append(Message(buf,
                                       srcHost->toString(),
                                       QString::number(*srcPort)));
    }
}

QPair<QString, int> MyServer::getMessages(int reqCount)
{
    int actualCount = qMin(m_receivedMessages->count(), reqCount);
    QPair<QString, int> messages("",actualCount);

    QList<Message>::reverse_iterator it;
    it = m_receivedMessages->rbegin();
    for(int i = 0; i < actualCount; ++i,*it++) {
        QString message = "";
        message.append(QString("<p align = \"center\"><b>From: </b> %1:%2<br/>"
                               "<b>Message Contents</b></p>%3\n")
                       .arg((*it).srcHost)
                       .arg((*it).srcPort)
                       .arg((*it).message));

        if(i != actualCount-1) {
            message.append("<hr>");
        }
        messages.first.append(message);
    }
    if(messages.second == 0) {
        messages.first = "No messages yet.";
    }

    return messages;
}
