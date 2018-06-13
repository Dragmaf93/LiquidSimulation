#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#include <QDebug>

#include "CellularAutomata.h"
class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer();
    bool openServer(int port);
    void sendCellullarAutomata(CellularAutomata<float> &automata);
    void closeConnection();
public slots:
    void newConnection();
private:
    QTcpServer* server;
    QTcpSocket* currentSocket;
};

#endif // TCPSERVER_H
