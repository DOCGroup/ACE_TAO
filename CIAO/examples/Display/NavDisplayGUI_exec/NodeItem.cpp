// $Id$

#include "NodeItem.h"
#include <QtGui/qbrush.h>

NodeItem::NodeItem(QGraphicsScene *canvas, NavUnit &unit)
  : QGraphicsRectItem(0.0, 0.0, 10.0, 10.0,0, canvas),
    unit_(unit)
{
   setRect(unit_.getLocation().x_, unit_.getLocation().y_, 10.0, 10.0);
  // There are only 7 colors defined.
  int color_id = unit_.getID() %  7;
  QBrush  *br =  new QBrush(Qt::GlobalColor(arrayColors[color_id]));
  setBrush(*br);
}

