// $Id$

#include "MapView.h"


MapView::MapView(
	QCanvas& c,
  QWidget* parent,
	const char* name,
  WFlags f)
  :
  bg_pixmap_("worldmap1.gif"),
  //bg_pixmap_("ukraine-map.gif"),
  QCanvasView(&c, parent, name, f)
{
  bg_pixmap_.load("worldmap1.gif");
  //bg_pixmap_.load("ukraine-map.gif");

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
