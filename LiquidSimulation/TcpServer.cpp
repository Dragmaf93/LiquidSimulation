#include "TcpServer.h"

TcpServer::TcpServer()
{
    server = new QTcpServer(this);

    // whenever a user connects, it will emit signal
    connect(server, SIGNAL(newConnection()),this, SLOT(newConnection()));
}

bool TcpServer::openServer(int port)
{
    if(!server->listen(QHostAddress::Any, port))
    {
        qDebug() << "Server could not start. "<<server->errorString();
        return false;
    }
    else
    {
        qDebug() << "Simulation started!";
    }

    server->waitForNewConnection(-1);
    return true;

}

void TcpServer::sendCellullarAutomata(CellularAutomata<float>& automata)
{
    //qDebug() << automata.toString();
    currentSocket->write(QByteArray(automata.toString().toStdString().c_str())+"|");
    currentSocket->flush();
//    currentSocket->waitForBytesWritten(10000);
}

void TcpServer::closeConnection()
{
    currentSocket->write("close");

    currentSocket->waitForBytesWritten(5000);
    currentSocket->close();
}

void TcpServer::newConnection()
{
    qDebug() << "New Connectionnnnnnnnnnnnnnnnnnnnn";
    currentSocket = server->nextPendingConnection();
//    currentSocket->write("ciaoooooo");

//    currentSocket->waitForBytesWritten(3000);
}
