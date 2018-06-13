#include <QCoreApplication>
#include<string.h>
#include<stdio.h>
#include<iostream>

#include "TcpServer.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    TcpServer server;

    return a.exec();
}
