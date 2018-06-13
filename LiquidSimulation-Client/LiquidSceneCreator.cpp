#include "LiquidSceneCreator.h"


LiquidSceneCreator::LiquidSceneCreator(CellularAutomata<float> &automata, unsigned cellSize)
{
    this->automata = &automata;
    this->size1 = automata.getDim1();
    this->size2 = automata.getDim2();
    this->grid = new CellGrid**[this->size1];

    pen.setBrush(QBrush(Qt::gray));

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

LiquidSceneCreator::~LiquidSceneCreator()
{

}

void LiquidSceneCreator::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(buttonLeft){
        QGraphicsItem *item = itemAt(event->scenePos(), QTransform()) ;
        CellGrid *cell = qgraphicsitem_cast<CellGrid *>(item);
        if(cell){

            automata->state(cell->i,cell->j)=currentType;
            drawGrid(*automata);

        }
    }
    if(buttonRight){
        QGraphicsItem *item = itemAt(event->scenePos(), QTransform()) ;
        CellGrid *cell = qgraphicsitem_cast<CellGrid *>(item);
        if(cell){
            automata->state(cell->i,cell->j)=LiquidSimulation::BLANK;
            drawGrid(*automata);
        }
    }
}

void LiquidSceneCreator::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        buttonLeft=true;
        QGraphicsItem *item = itemAt(event->scenePos(), QTransform()) ;
        CellGrid *cell = qgraphicsitem_cast<CellGrid *>(item);
        if(cell){

            automata->state(cell->i,cell->j)=currentType;
            drawGrid(*automata);

        }
    }
    if(event->button() == Qt::RightButton){
        buttonRight=true;
        QGraphicsItem *item = itemAt(event->scenePos(), QTransform()) ;
        CellGrid *cell = qgraphicsitem_cast<CellGrid *>(item);
        if(cell){
            automata->state(cell->i,cell->j)=LiquidSimulation::BLANK;
            drawGrid(*automata);
        }
    }
}

void LiquidSceneCreator::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        buttonLeft=false;
    }
    if(event->button() == Qt::RightButton){
        buttonRight=false;
    }
}

void LiquidSceneCreator::setAutomata(CellularAutomata<float> &automata)
{
    this->automata = &automata;
}

void LiquidSceneCreator::drawGrid(CellularAutomata<float> &automata)
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
        }
    }
}

void LiquidSceneCreator::recreateGrid(CellularAutomata<float> &automata, unsigned cellSize)
{
    this->automata = &automata;
    for(unsigned long i=0; i < this->size1; i++)
        for(unsigned long j=0; j < this->size2; j++){
            this->removeItem(grid[i][j]);
            this->removeItem(grid[i][j]->color);

            delete grid[i][j];
        }

    for(unsigned long i=0; i < this->size1; i++){
        delete grid[i];
    }
    delete grid;

    this->size1 = automata.getDim1();
    this->size2 = automata.getDim2();


    this->grid = new CellGrid**[this->size1];
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

void LiquidSceneCreator::setAddingType(float type)
{
    qDebug() << "Change type"<<type;
    currentType = type;
}
