#include "CommandLineParser.h"
CommandLineParser::CommandLineParser()
    :QCommandLineParser()
{
    this->addOption(rowsOption);
    this->addOption(colsOption);
    this->addOption(stepsOption);
    this->addOption(updateOption);
    this->addOption(portOption);
    this->addOption(dataOption);
}

bool CommandLineParser::isThereAnError()
{
    int n;
    return !(rows(n) && columns(n) && updateSteps(n) && steps(n) && port(n));
}

bool CommandLineParser::rows(int &n) const
{
    bool ok;
    n = this->value(rowsOption).toInt(&ok);
    return n > 0 && ok;
}

bool CommandLineParser::columns(int &n) const
{
    bool ok;
     n = this->value(colsOption).toInt(&ok);
    return n > 0 && ok;
}

bool CommandLineParser::updateSteps(int &n) const
{
    bool ok;
    n = this->value(updateOption).toInt(&ok);
    return n > 0 && ok;
}

bool CommandLineParser::steps(int &n) const
{
    bool ok;
    n = value(stepsOption).toInt(&ok);
    return n > 0 && ok;

}

bool CommandLineParser::port(int &n) const
{
    bool ok;
    n = value(portOption).toInt(&ok);
    return n > 0 && ok;
}

bool CommandLineParser::data(QString &str) const
{
    bool ok=true;
    str = value(dataOption);
    return ok;
}

void CommandLineParser::data(CellularAutomata<float> &automata) const
{
    QStringList values = value(dataOption).split(',');
     values[0].remove(0,1);
     values[values.size()-1].remove(values[values.size()-1].size()-1,1);
     for(int i = 0; i < values.size(); i++){
         automata.state(i) = values[i].toFloat();
     }
}

