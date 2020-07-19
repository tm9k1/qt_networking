#ifndef MYSERVER_H
#define MYSERVER_H

#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>

class MyServer : public QObject
{
    Q_OBJECT
public:
    explicit MyServer(QObject *parent = nullptr);
    QString getIpAddress();
    QString getPortNumber();

signals:

public slots:
    void newConnection();


private:
    QTcpServer *m_server;
    QString m_ipAddress;
    QString m_portNumber;
};

#endif // MYSERVER_H
