#include "TcpClient.h"
#include <QTimer>
#include <QEventLoop>

#define MAX_ATTEMPT 10
TcpClient::TcpClient(QObject *parent) : QObject(parent)
{

}

bool TcpClient::connectoToServer(QByteArray& simParameter)
{
    socket = new QTcpSocket(this);

    socket->connectToHost("localhost", 9999);

    if(socket->waitForConnected(5000))
    {
        qDebug() << "Connected!";

        // send
        socket->write(simParameter);
        socket->waitForBytesWritten(1000);
        socket->waitForReadyRead(3000);

        // get the data
        QByteArray newPort = socket->readAll();
        socket->close();
        delete socket;

        socket = new QTcpSocket(this);

        qDebug() << "Try to connect to port: " <<newPort << newPort.toInt();

        int attemptConnection =  0;
        while(attemptConnection<MAX_ATTEMPT){

            QEventLoop loop;
            QTimer::singleShot(3000, &loop, SLOT(quit()));
            loop.exec();
            socket->connectToHost("localhost", newPort.toInt());
            qDebug()<<"Attempt :"<<attemptConnection;
            if(socket->waitForConnected()){
                qDebug() << "Connected";
                connect(socket,SIGNAL(readyRead()),this,SLOT(handleMessage()));
                return true;
            }
            attemptConnection++;
        }

        qDebug() << "Error "<<socket->error();
        return false;
    }else
    {
        qDebug() << "Not connected!";
        return false;
    }

    return true;
}

QByteArray* TcpClient::getNextMessage()
{
    if(queue.size()<=0) return nullptr;

    QByteArray* ba = new QByteArray(queue.dequeue());

    return ba;

}

QByteArray& TcpClient::setSimulationParameter(int nProces, int rows, int columns, int steps, int stepsToUpdate, CellularAutomata<float> & automata)
{
    QByteArray* byteArray = new QByteArray;
    QXmlStreamWriter xmlWriter (byteArray);

    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    xmlWriter.writeStartElement("simulation-parameter");
    xmlWriter.writeAttribute("processes",QString::number(nProces));
    xmlWriter.writeAttribute("rows",QString::number(rows));
    xmlWriter.writeAttribute("columns",QString::number(columns));
    xmlWriter.writeAttribute("steps",QString::number(steps));
    xmlWriter.writeAttribute("update-steps",QString::number(stepsToUpdate));
    xmlWriter.writeTextElement("data",automata.toString().toStdString().c_str());
    xmlWriter.writeEndElement();

    xmlWriter.writeEndDocument();

    return *byteArray;
}

void TcpClient::handleMessage()
{
    qDebug() << "Message arrived";
    QByteArray receivedMsgs = socket->readAll();
    //    qDebug() << receivedMsgs;

    QList<QByteArray> msgList = receivedMsgs.split('|');
    foreach (QByteArray msg, msgList) {
        if(msg == "close"){
            socket->close();
            qDebug() << "Close connection";
        }else if(msg != "")
            emit newMessageArrived(msg);
    }

}
