#ifndef LIQUIDSCENECREATOR_H
#define LIQUIDSCENECREATOR_H

#include<QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include"LiquidSimulation.h"
#include"CellGrid.h"
class LiquidSceneCreator : public QGraphicsScene
{
public:
    LiquidSceneCreator(CellularAutomata<float> &automata, unsigned cellSize);
    ~LiquidSceneCreator();

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

    void setAutomata(CellularAutomata<float> &automata);
    void drawGrid(CellularAutomata<float> &automata);
    void recreateGrid(CellularAutomata<float> &automata,unsigned cellSize);

    void setAddingType(float type);
private:
    unsigned long size1,size2;
    CellGrid*** grid;
    QPen pen;
    CellularAutomata<float> *automata;
    bool buttonLeft,buttonRight;
    float currentType;
};

#endif // LIQUIDSCENECREATOR_H
