#ifndef LIQUIDSIMULATOR_H
#define LIQUIDSIMULATOR_H

#include "CellularAutomata.h"
#include<cmath>
#include<QDebug>
#include<QList>
class LiquidSimulation
{
public:
    static CellularAutomata<float>* tmpAutomata;
    static float MaxValue;
    static float MinValue;
    static float MaxCompression;
    static float MinFlow;
    static float MaxFlow;
    static float FlowSpeed;

    static float AddWater;
    static QList<CellularAutomata<float>::Cell> SourcesWater;
    enum TypeCell{BLANK=0,SOLID=9999,SOURCE_WATER=5555};
    static void initSimulator(CellularAutomata<float>& automata);
    static void transitionFunction(CellularAutomata<float>& automata, unsigned long i, unsigned long j);
    static void applyTransitionFunctionToAutomata(CellularAutomata<float> &automata);

};

#endif // LIQUIDSIMULATOR_H
