#include "CellGrid.h"


CellGrid::CellGrid(int i, int j, int width, int height)
    :QGraphicsRectItem(i*width,j*width,width,height)
{
    this->i=j;
    this->j=i;
    this->size = width;
    rectStater = new QRect(i*width,j*width,width,height);
    color =new QGraphicsRectItem(*rectStater);
    color->setPen(Qt::NoPen);
    color->setTransformOriginPoint(i*width+width,j*width);

}

CellGrid::~CellGrid()
{
    delete color;
    delete rectStater;
}

void CellGrid::setColor(float liquidValue, float topValue,float bottomValue)
{

    if(liquidValue == LiquidSimulation::SOLID){
        color->setRect(*rectStater);
        color->setBrush(QBrush(Qt::black));
    }

    else if(liquidValue == 0){
        color->setRect(*rectStater);
        color->setBrush(QBrush(Qt::white));
    }
    else if(liquidValue == LiquidSimulation::SOURCE_WATER){
        color->setRect(*rectStater);
        color->setBrush(QBrush(Qt::blue));
    }
    else{
        float f = fmin(1,liquidValue);
        color->setRect(QRect(j*size,i*size+size-size*f+1,size,size*f));



        color->setBrush(QBrush(interpolate(QColor(135,206,250,255),QColor(65,105,225,255),liquidValue/4.0f)));
    }
}

QColor CellGrid::interpolate(QColor color1, QColor color2, float fraction)
{
    QColor color((color2.red()-color1.red())*fraction+color1.red(),
                 (color2.green()-color1.green())*fraction+color1.green(),
                 (color2.blue()-color1.blue())*fraction+color1.blue(),
                  (color2.alpha()-color1.alpha())*fraction+color1.alpha());
    if(!color.isValid())
        return (QColor(8,23,204,255));
    return color;

}
