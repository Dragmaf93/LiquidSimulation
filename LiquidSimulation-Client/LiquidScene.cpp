#include "LiquidScene.h"
#include<QDebug>
LiquidScene::LiquidScene(CellularAutomata<float> &automata, unsigned cellSize)
    :QGraphicsScene()
{
    this->size1 = automata.getDim1();
    this->size2 = automata.getDim2();
    this->grid = new CellGrid**[this->size1];

    this->automata = &automata;
//    pen.setBrush(QBrush(Qt::gray));

    for(unsigned long i=0; i < this->size1; i++){
        grid[i] = new CellGrid* [this->size2];
        for(unsigned long j=0; j < this->size2; j++)
        {
            grid[i][j] = new CellGrid(i,j,cellSize,cellSize);
            this->addItem(grid[i][j]->color);
                      this->addItem(grid[i][j]);
            grid[i][j]->setPen(pen);
        }
    }

}

LiquidScene::~LiquidScene()
{

}

//void LiquidScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
//{
//    if(buttonLeft){
//      QGraphicsItem *item = itemAt(event->scenePos(), QTransform()) ;
//      CellGrid *cell = qgraphicsitem_cast<CellGrid *>(item);
//            if(cell){
//                automata->state(cell->i,cell->j)=LiquidSimulation::AddWater;
//                qDebug()<<"Add Water"<<cell->i<<cell->j;
//                drawGrid(*automata);

//            }
//    }
//    if(buttonRight){
//        QGraphicsItem *item = itemAt(event->scenePos(), QTransform()) ;
//        CellGrid *cell = qgraphicsitem_cast<CellGrid *>(item);
//              if(cell){
//                  qDebug()<<"Add Block"<<cell->i<<cell->j;
//                  automata->state(cell->i,cell->j)=LiquidSimulation::SOLID;
//                  drawGrid(*automata);
//              }
//    }
//}

//void LiquidScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
//{
//    if(event->button() == Qt::LeftButton){
//        buttonLeft=true;
//    }
//    if(event->button() == Qt::RightButton){
//        buttonRight=true;
//    }
//}

//void LiquidScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
//{
//    if(event->button() == Qt::LeftButton){
//        buttonLeft=false;
//    }
//    if(event->button() == Qt::RightButton){
//        buttonRight=false;
//    }
//}

void LiquidScene::drawGrid(CellularAutomata<float> &automata)
{

    for(unsigned long i=0; i < this->size1; i++){
        for(unsigned long j=0; j < this->size2; j++){
            if(j>0 && j<size2-1)
                grid[j][i]->setColor(automata.state(i,j),automata.state(i,j-1),automata.state(i,j+1));
            else if(j>0)
                grid[j][i]->setColor(automata.state(i,j),automata.state(i,j-1),0);
            else if(j<size2-1)
                grid[j][i]->setColor(automata.state(i,j),0,automata.state(i,j+1));
            else
                grid[j][i]->setColor(automata.state(i,j),0,0);
            //Is a block
//            if(automata.state(i,j) == LiquidSimulator::SOLID){
//                grid[j][i]->setBrush(QBrush(Qt::black));
//            }

//            else if(automata.state(i,j) == 0){
//                grid[j][i]->setBrush(QBrush(Qt::white));
//            }
//            else{
//                grid[j][i]->setBrush(QBrush(QColor(0,135,200,255)));
//            }
        }
    }
}
