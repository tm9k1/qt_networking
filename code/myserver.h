#ifndef MYSERVER_H
#define MYSERVER_H

#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>

class MyServer : public QObject
{
    Q_OBJECT
public:
    explicit MyServer(QObject *parent = nullptr);
    ~MyServer();

    bool startServing();

    QString getIpAddress();
    QString getPortNumber();
    bool writeTCPMessage(QString &message);

public slots:
    void newConnection();

private:
    QTcpServer *m_server;
    QTcpSocket *m_tcpSocket;
    QUdpSocket *m_udpSocket;
    QString m_ipAddress;
    QString m_portNumber;
};

#endif // MYSERVER_H
