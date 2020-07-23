#ifndef MYSERVER_H
#define MYSERVER_H

#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QPair>

struct Message {
    QString message;
    QString srcHost;
    QString srcPort;
    QString protocol;
    Message(QString msg, QString sourceHost, QString sourcePort) :
        message(msg), srcHost(sourceHost), srcPort(sourcePort) {}
};

class MyServer : public QObject
{
    Q_OBJECT
public:
    explicit MyServer(QObject *parent = nullptr);
    ~MyServer();
    bool isListening();
    const QHostAddress getIpAddress();
    quint16 getPortNumber();
    QPair<QString, int> getMessages(int count);

public slots:
    void newConnection();
    void readMessage();

private:
    QTcpServer *m_server;
    QList<Message> *m_receivedMessages;
    QTcpSocket *m_tcpSocket;
    QUdpSocket *m_udpSocket;

    QHostAddress m_ipAddress = QHostAddress::Any;
    int m_portNumber = 1234;

};

#endif // MYSERVER_H
