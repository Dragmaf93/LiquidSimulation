#ifndef LIQUIDSCENE_H
#define LIQUIDSCENE_H

#include<QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include"LiquidSimulation.h"
#include"CellGrid.h"
class LiquidScene : public QGraphicsScene
{
public:
    LiquidScene(CellularAutomata<float> &automata, unsigned cellSize);
    ~LiquidScene();
//    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
//    void mousePressEvent(QGraphicsSceneMouseEvent* event);
//    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

    void drawGrid(CellularAutomata<float> &automata);
private:
    unsigned long size1,size2;
    CellGrid*** grid;
    QPen pen;
    CellularAutomata<float> *automata;
    bool buttonLeft,buttonRight;
};

#endif // LIQUIDSCENE_H
