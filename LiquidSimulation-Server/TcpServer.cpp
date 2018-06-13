#include "TcpServer.h"
int TcpServer::PORT = 5555;
TcpServer::TcpServer(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);

    // whenever a user connects, it will emit signal
    connect(server, SIGNAL(newConnection()),this, SLOT(newConnection()));

    if(!server->listen(QHostAddress::Any, 9999))
    {
        qDebug() << "Server could not start";
    }
    else
    {
        qDebug() << "Server started!";
    }
}

void TcpServer::newConnection()
{
    QTcpSocket *socket = server->nextPendingConnection();

    socket->waitForReadyRead(3000);

    QByteArray ba = socket->readAll();
//    qDebug() << ba;
    QString command ="mpirun",  port=QString::number(PORT);
    PORT++;


    QXmlStreamReader reader(ba);
    if(reader.readNextStartElement()){
        command +=" -np "+reader.attributes().value("processes");
        command +=" /home/mfdrago/Sviluppo/Parallel/LiquidSimulation/build-LiquidSimulation-Desktop_Qt_5_11_0_GCC_64bit2-Debug/LiquidSimulation";
        command +=" --steps "+ reader.attributes().value("steps");
        command +=" --rows "+reader.attributes().value("rows");
        command +=" --columns "+reader.attributes().value("columns");
        command +=" --update-steps "+reader.attributes().value("update-steps");
        command +=" --port "+port;
        reader.readNextStartElement();
       command +=" --data "+reader.readElementText();
    }
    socket->write(QByteArray(port.toStdString().c_str()));
    socket->waitForBytesWritten(3000);

    int numProc;
//    QString command("mpirun -np 4 /home/mfdrago/Sviluppo/Parallel/LiquidSimulation/build-LiquidSimulation-Desktop_Qt_5_11_0_GCC_64bit2-Debug/LiquidSimulation --steps 50 --rows 8 --columns 8 --update-steps 5 --port 5545");

    QProcess::execute(command);



    socket->close();
}
