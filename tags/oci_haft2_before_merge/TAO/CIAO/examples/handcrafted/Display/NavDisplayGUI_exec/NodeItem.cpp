// $Id$

#include "NodeItem.h"
#include <qbrush.h>


QBrush *NodeItem::normal = new QBrush(red);
QBrush *NodeItem::highlighted = new QBrush(green);


NodeItem::NodeItem(QCanvas *canvas, NavUnit &unit)
  : QCanvasEllipse(8, 8, canvas),
    unit_(unit),
    blink_timer(canvas),
    is_selected_(0)
{
  setBrush(*normal);
  setZ(128);
}


void NodeItem::moveBy(double dx, double dy, double dz)
{
  QCanvasEllipse::moveBy(dx, dy);
}


void
NodeItem::timerDone()
{
  if(selected())
  {
    if(brush() == *normal)
      setBrush(*highlighted);
    else
      setBrush(*normal);

    canvas()->update();
  }
}


void
NodeItem::setSelected(bool yes)
{
  is_selected_ = yes;

  if(yes)
  {
    connect(&blink_timer, SIGNAL(timeout()),
             SLOT(timerDone()));

    blink_timer.start(500);
  }
  else
  {
    blink_timer.stop();
    disconnect(SIGNAL(timeout()), this,
      SLOT(timerDone()));
    setBrush(*normal);
    canvas()->update();
  }
}
