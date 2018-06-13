#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QTcpSocket>
#include <QQueue>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "CellularAutomata.h"

class TcpClient : public QObject
{
    Q_OBJECT
public:
    TcpClient(QObject *parent = nullptr);

    bool connectoToServer(QByteArray &simParameter);
    QByteArray* getNextMessage();

     QByteArray& setSimulationParameter(int nProces, int  rows, int columns, int steps, int stepsToUpdate, CellularAutomata<float> & automata);
private:
    QTcpSocket* socket;

    QQueue<QByteArray> queue;

signals:
    void newMessageArrived(QByteArray& ba);

public slots:
    void handleMessage();
};

#endif // TCPCLIENT_H
