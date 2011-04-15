// $Id$

#include "NodeItem.h"
#include <Qt/qbrush.h>

QBrush *NodeItem::normal = new QBrush(Qt::red);
QBrush *NodeItem::highlighted = new QBrush(Qt::green);

NodeItem::NodeItem(Q3Canvas *canvas, NavUnit &unit)
  : Q3CanvasEllipse(8, 8, canvas),
    unit_(unit),
    blink_timer(canvas),
    is_selected_(0)
{
  setBrush(*normal);
  setZ(128);
}

void NodeItem::moveBy(double dx, double dy, double /* dz */)
{
  Q3CanvasEllipse::moveBy (dx, dy);
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
    // FUZZ: disable check_for_lack_ACE_OS
    connect(&blink_timer, SIGNAL(timeout()),
             SLOT(timerDone()));
    // FUZZ: enable check_for_lack_ACE_OS

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
