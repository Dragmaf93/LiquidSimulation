#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsView>
#include <QKeyEvent>
#include "LiquidScene.h"
#include "TcpClient.h"

class LiquidSimulatorGUI : public QGraphicsView
{
    Q_OBJECT
public:
    LiquidSimulatorGUI(QWidget *parent = 0);

    void setNumProcesses(int num);
    void setNumColumns(int num);
    void setNumRows(int num);
    void setNumSteps(int num);
    void setUpdateSteps(int num);

    void setCellSize(int num);
    bool validateParamenter();

    bool startSimulation();
    void initScene(CellularAutomata<float> &automata);
    ~LiquidSimulatorGUI();
    void keyReleaseEvent(QKeyEvent *event);

public slots:
    void updateCellularAutomata(QByteArray &ba);


private:
    LiquidScene* liquidScene;
    CellularAutomata<float>* automata;
    TcpClient client;

    int nProcesses;
    int nColumns;
    int nRows;
    int nSteps;
    int nStepsUpdate;
    int cellSize;
};

#endif // MAINWINDOW_H
