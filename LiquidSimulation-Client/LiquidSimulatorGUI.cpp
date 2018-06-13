#include "LiquidSimulatorGUI.h"
#include<QDebug>
#define SIZE 150
#define CELL_SIZE 5
LiquidSimulatorGUI::LiquidSimulatorGUI(QWidget *parent)
    : QGraphicsView(parent)
{
    connect(&client,SIGNAL(newMessageArrived(QByteArray&)),this,SLOT(updateCellularAutomata(QByteArray&)));
}


LiquidSimulatorGUI::~LiquidSimulatorGUI()
{

}
bool LiquidSimulatorGUI::validateParamenter()
{
    if(nProcesses != 3 && nProcesses != 5 && nProcesses !=9)
        return false;
    if(nSteps <= 0 || nSteps < nStepsUpdate )
        return false;
    if(nStepsUpdate <= 0)
        return false;
    if(nRows <= 0)
        return false;
    if(nColumns <= 0)
        return false;

    return true;
}

bool LiquidSimulatorGUI::startSimulation()
{
    QByteArray ba =  client.setSimulationParameter(nProcesses,nRows,nColumns,nSteps,nStepsUpdate,*automata);

//    qDebug() << "Simulation parameter" << ba;
    return client.connectoToServer(ba);
}



void LiquidSimulatorGUI::initScene(CellularAutomata<float> &automata)
{
    this->automata = &automata;
    this->liquidScene = new LiquidScene(automata,cellSize);
    this->setScene(this->liquidScene);
    this->liquidScene->drawGrid(automata);
}

void LiquidSimulatorGUI::keyReleaseEvent(QKeyEvent *event)
{
    switch( event->key() )
    {
    case Qt::Key_Space:
    {
        LiquidSimulation::applyTransitionFunctionToAutomata(*automata);
        this->liquidScene->drawGrid(*automata);
    }
        break;
    }
}

void LiquidSimulatorGUI::updateCellularAutomata(QByteArray& ba)
{
    QList<QByteArray> values = ba.split(',');
    values[0].remove(0,1);
    values[values.size()-1].remove(values[values.size()-1].size()-1,1);
    qDebug() << "Values "<<values.size() << values[0];

    for(int i = 0; i < values.size(); i++){
        automata->state(i) = values[i].toFloat();
    }
//    automata->print();
    this->liquidScene->drawGrid(*automata);

}

void LiquidSimulatorGUI::setNumProcesses(int num)
{
    nProcesses = num;
}

void LiquidSimulatorGUI::setNumColumns(int num)
{
    nColumns = num;
}

void LiquidSimulatorGUI::setNumRows(int num)
{
    nRows = num;
}

void LiquidSimulatorGUI::setNumSteps(int num)
{
    nSteps = num;
}

void LiquidSimulatorGUI::setUpdateSteps(int num)
{
    nStepsUpdate = num;
}

void LiquidSimulatorGUI::setCellSize(int num)
{
    cellSize = num;
}
