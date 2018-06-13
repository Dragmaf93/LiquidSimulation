#include "LiquidSimulation.h"

CellularAutomata<float>* LiquidSimulation::tmpAutomata = NULL;
float LiquidSimulation::MinValue = 0.0005f;
float LiquidSimulation::MaxValue = 1.0f;
float LiquidSimulation::MaxCompression = 0.25f;
float LiquidSimulation::MinFlow = 0.005f;
float LiquidSimulation::MaxFlow = 4.0f;
float LiquidSimulation::FlowSpeed = 1.0f;
float LiquidSimulation::AddWater = 1.0f;
QList<CellularAutomata<float>::Cell> LiquidSimulation::SourcesWater = QList<CellularAutomata<float>::Cell>();

void LiquidSimulation::initSimulator(CellularAutomata<float> &automata)
{
    tmpAutomata= new CellularAutomata<float>(automata.getDim1(),automata.getDim2());
    for(unsigned long i = 0;i<tmpAutomata->getDim1();i++)
        for(unsigned long j = 0;j<tmpAutomata->getDim2();j++)
            tmpAutomata->state(i,j)=0.0f;
}


float ComputeVerticalFlowValue(float remainingLiquid, float destinationLiquid){
    float value =0;
    float sum = remainingLiquid + destinationLiquid;

    if(sum <= LiquidSimulation::MaxValue)
        value = LiquidSimulation::MaxValue;
    else if (sum < 2* LiquidSimulation::MaxValue + LiquidSimulation::MaxCompression)
        value = (LiquidSimulation::MaxValue*LiquidSimulation::MaxValue +sum*LiquidSimulation::MaxCompression) / (LiquidSimulation::MaxValue+LiquidSimulation::MaxCompression);
    else
        value = (sum + LiquidSimulation::MaxCompression)/2.0f;
    return value;
}

void LiquidSimulation::transitionFunction(CellularAutomata<float> &automata, unsigned long i, unsigned long j)
{
    float valueCell = automata.state(i,j);

    CellularAutomata<float>& diff = *tmpAutomata;

    if(valueCell == SOLID){
        return;
    }

    if(valueCell == 0){
        return;
    }
    //add if settled return?

    if(valueCell<MinValue){
        automata.state(i,j)=0;
        return;
    }

    CellularAutomata<float>::Cell ** neighbours = automata.neighbour(i,j);

    float startValue = valueCell,remainingValue = valueCell;
    float flow=0;

    CellularAutomata<float>::Cell& bottom = *(neighbours[CellularAutomata<float>::DOWN]);
    //Flow to bottom cell
    if(neighbours[CellularAutomata<float>::DOWN] != NULL  && automata.state(bottom) != SOLID){

        flow = ComputeVerticalFlowValue(valueCell,automata.state(bottom)) - automata.state(bottom);
        //Determine rate of flow
        if(flow >MinFlow && automata.state(bottom)>0){
            flow *= FlowSpeed;
        }

        //Constrain flow
        flow = fmax(flow,0);
        if(flow > fmin(MaxFlow, valueCell))
            flow = fmin(MaxFlow,valueCell);

        //Update temp value
        if(flow !=0){
            remainingValue -=flow;
            diff.state(i,j)-=flow;
            diff.state(bottom)+=flow;
        }
    }

    if(remainingValue < MinValue){
        diff.state(i,j)-=remainingValue;

        for(int i=0; i < 4;i++)
            if(neighbours[i] != NULL)
                delete neighbours[i];
        delete neighbours;

        return;
    }

    CellularAutomata<float>::Cell& left = *(neighbours[CellularAutomata<float>::LEFT]);
    if(neighbours[CellularAutomata<float>::LEFT] != NULL  && automata.state(left) != SOLID){

        flow = (remainingValue - automata.state(left)) /4.0f;
        if(flow > MinFlow)
            flow*=FlowSpeed;

        flow = fmax(flow,0);
        if(flow > fmin(MaxFlow, remainingValue))
            flow = fmin(MaxFlow,remainingValue);

        //Update temp value
        if(flow !=0){
            remainingValue -=flow;
            diff.state(i,j)-=flow;
            diff.state(left)+=flow;
        }
    }

    if(remainingValue < MinValue){
        diff.state(i,j)-=remainingValue;

        for(int i=0; i < 4;i++)
            if(neighbours[i] != NULL)
                delete neighbours[i];
        delete neighbours;

        return;
    }

    CellularAutomata<float>::Cell& right = *(neighbours[CellularAutomata<float>::RIGHT]);
    if(neighbours[CellularAutomata<float>::RIGHT] != NULL  && automata.state(right) != SOLID){

        flow = (remainingValue - automata.state(right)) /3.0f;

        if(flow > MinFlow)
            flow*=FlowSpeed;

        flow = fmax(flow,0);
        if(flow > fmin(MaxFlow, remainingValue))
            flow = fmin(MaxFlow,remainingValue);

        //Update temp value
        if(flow !=0){
            remainingValue -=flow;
            diff.state(i,j)-=flow;
            diff.state(right)+=flow;
        }
    }

    if(remainingValue < MinValue){
        diff.state(i,j)-=remainingValue;

        for(int i=0; i < 4;i++)
            if(neighbours[i] != NULL)
                delete neighbours[i];
        delete neighbours;

        return;
    }

    CellularAutomata<float>::Cell& up = *(neighbours[CellularAutomata<float>::UP]);
    if(neighbours[CellularAutomata<float>::UP] != NULL  && automata.state(up) != SOLID){

        flow = remainingValue - ComputeVerticalFlowValue(valueCell,automata.state(up));
        if(flow > MinFlow)
            flow*=FlowSpeed;

        flow = fmax(flow,0);
        if(flow > fmin(MaxFlow, remainingValue))
            flow = fmin(MaxFlow,remainingValue);

        //Update temp value
        if(flow !=0){
            remainingValue -=flow;
            diff.state(i,j)-=flow;
            diff.state(up)+=flow;
        }
    }

    if(remainingValue < MinValue){
        diff.state(i,j)-=remainingValue;

    }

    for(int i=0; i < 4;i++)
        if(neighbours[i] != NULL)
            delete neighbours[i];
    delete neighbours;
}

void LiquidSimulation::applyTransitionFunctionToAutomata(CellularAutomata<float> &automata)
{

    for(unsigned long i=0; i < automata.getDim1();i++)
        for(unsigned long j=0; j< automata.getDim2();j++)
            transitionFunction(automata,i,j);

    for(unsigned long i=0; i < automata.getDim1();i++)
        for(unsigned long j=0; j< automata.getDim2();j++){
            automata.state(i,j)+= tmpAutomata->state(i,j);
            if(automata.state(i,j)< MinValue)
                automata.state(i,j)=0;
            tmpAutomata->state(i,j)=0.0f;
        }
}
