// $Id$

#include "MapView.h"
#include <stdlib.h>
#include "worldmap1.xpm"

MapView::MapView(
  QGraphicsScene& c,
  QWidget* parent )
  :
  QGraphicsView(&c, parent),
  bg_pixmap_(worldmap1)
{
  setSceneRect (0,0, 601, 388);
  setMinimumSize(bg_pixmap_.width() -10, bg_pixmap_.height() -10);
}

void
MapView::clear()
{
}

void
MapView::resizeEvent (QResizeEvent * event)
{
  QPixmap oo  = bg_pixmap_.scaled(this->width() , this->height());
  setBackgroundBrush(QBrush(oo));
}

