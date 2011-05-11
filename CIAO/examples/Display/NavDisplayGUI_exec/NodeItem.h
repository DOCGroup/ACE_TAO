// $Id$

#ifndef __NODEITEM_H
#define __NODEITEM_H

#include "NavUnit.h"
#include <QtCore/qtimer.h>
#include <QtGui/qbrush.h>
#include <QtCore/qobject.h>
#include <QtGui/qgraphicsitem.h>

const int  arrayColors[] =  {
     12,  //Qt::yellow,
     7,   //Qt::red,
     8,   //Qt::green,
     11,  //Qt::magenta,
     9,   //Qt::blue,
     10,  //Qt::cyan
     3,   //Qt::white
   } ;

class NodeItem: public QObject,
                public QGraphicsRectItem
{
  Q_OBJECT

public:
  NodeItem(QGraphicsScene *canvas, NavUnit &unit);

public slots:

private:
  NavUnit &unit_;

};


#endif // __NODEITEM_H
