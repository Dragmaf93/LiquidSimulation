#include "LiquidSimulatorGUI.h"
#include <QApplication>
#include <QScreen>

#include "LiquidSimulation.h"
#include "TcpClient.h"
#include "SetParametersForm.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    unsigned long dim1=200,
//            dim2=200;
//    CellularAutomata<float> automata(dim1,dim2);

//    for(unsigned long i = 0;i<dim1;i++)
//        for(unsigned long j = 0;j<dim2;j++)
//            automata.state(i,j)=0.0f;

//    //Set borderr
//    for(unsigned long i = 0;i<dim1;i++)
    //    {
    //        automata.state(i,0)=LiquidSimulation::SOLID;
    //        automata.state(i,dim2-1)=LiquidSimulation::SOLID;
    //    }

    //    for(unsigned long j = 0;j<dim2;j++)
    //    {
    //        automata.state(0,j)=LiquidSimulation::SOLID;
    //        automata.state(dim1-1,j)=LiquidSimulation::SOLID;
    //    }

//    LiquidSimulation::AddWater=5.0f;
//    LiquidSimulation::FlowSpeed=1.0f;
//    LiquidSimulation::initSimulator(automata);

//    LiquidSimulatorGUI w;
//    int row = 400, col = 400;
//    w.setNumProcesses(5);

//    w.setNumColumns(col);
//    w.setNumRows(row);

//    w.setNumSteps(80000);
//    w.setUpdateSteps(10);
//    w.setCellSize(2);
//    if(!w.validateParamenter())
//        a.exit(-1);

////    if(!w.startSimulation())
//        a.exit(-1);

//    CellularAutomata<float> automata(row,col);

//    w.initScene(automata);
//    w.setFixedSize(QSize(QGuiApplication::screens()[0]->size()*0.8));
//    w.show();
    SetParameterForm mainForm;
    mainForm.show();



    return a.exec();
}
