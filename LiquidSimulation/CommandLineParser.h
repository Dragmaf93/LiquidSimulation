#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H

#include <QCommandLineOption>
#include <QCommandLineParser>
#include "CellularAutomata.h"
class CommandLineParser: public QCommandLineParser
{
public:
    CommandLineParser();
    bool isThereAnError();

    bool rows(int &n)const;
    bool columns(int &n)const;
    bool updateSteps(int &n)const;
    bool steps(int &n)const;
    bool port(int &n)const;
    bool data(QString &str)const;

    void data(CellularAutomata<float> &automata)const;
private:
    QCommandLineOption rowsOption=QCommandLineOption(QStringList() << "r" << "rows","number of rows","10");
    QCommandLineOption colsOption=QCommandLineOption(QStringList() << "c" << "columns","number of columns","10");
    QCommandLineOption stepsOption=QCommandLineOption(QStringList() << "s" << "steps","number of steps","20");
    QCommandLineOption updateOption=QCommandLineOption(QStringList() << "u" << "update-steps","steps to update","5");
    QCommandLineOption portOption=QCommandLineOption(QStringList() << "p" << "port","number of port","55555");
    QCommandLineOption dataOption=QCommandLineOption(QStringList() << "d" << "data","init automata","0");
};

#endif // COMMANDLINEPARSER_H
