#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QProcess>
#include <QXmlStreamReader>
#include <QSerialPortInfo>
#include <QDebug>

class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);

public slots:
    void newConnection();
private:
    QTcpServer* server;

    const char* PATH_EXE="";
    static int PORT;
};

#endif // TCPSERVER_H
