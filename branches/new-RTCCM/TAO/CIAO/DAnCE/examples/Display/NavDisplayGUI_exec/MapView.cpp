// $Id$

#include "MapView.h"
#include <stdlib.h>

#include "worldmap1.xpm"

MapView::MapView(
	QCanvas& c,
  QWidget* parent,
	const char* name,
  WFlags f)
  :
  QCanvasView(&c, parent, name, f),
  bg_pixmap_(worldmap1)
{
  viewport()->setBackgroundMode(NoBackground);
  enableClipper(TRUE);
  if(!bg_pixmap_.isNull())
  {
    resizeContents(bg_pixmap_.width(), bg_pixmap_.height());
  }
  else
    resizeContents(100, 100);

  canvas()->setBackgroundPixmap(bg_pixmap_);
}

void
MapView::clear()
{
  /*QCanvasItemList list = canvas()->allItems();
  for(QCanvasItemList::Iterator it = list.begin(); it != list.end(); ++it)
  {
  	if(*it)
	    delete *it;
  }*/
}
