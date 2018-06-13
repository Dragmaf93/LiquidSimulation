#ifndef CELLGRID_H
#define CELLGRID_H

#include<QGraphicsRectItem>
#include <QPainter>
#include <QTransform>

#include"LiquidSimulation.h"

class CellGrid : public QGraphicsRectItem
{
public:
   CellGrid(int i,int j, int width, int height);
   ~CellGrid();

   unsigned long i,j,size;

   QGraphicsRectItem* color;
   void setColor(float liquidValue, float top,float bottom);
private:
   bool added;
   QRect* rectStater;
   QColor interpolate(QColor color1, QColor color2, float fraction);
};

#endif // CELLGRID_H
