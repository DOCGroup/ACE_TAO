// $Id$

#include "MapView.h"
#include <stdlib.h>

MapView::MapView(
	QCanvas& c,
  QWidget* parent,
	const char* name,
  WFlags f)
  :
  QCanvasView(&c, parent, name, f)
{
  QString path_to_movie;
  char *ace_root = getenv("ACE_ROOT");
  if(ace_root)
  {
    path_to_movie += ace_root;
    path_to_movie += "/TAO/CIAO/examples/handcrafted/Display/NavDisplayGUI_exec/worldmap1.gif";
  }
  else
  {
    path_to_movie = "../NavDisplayGUI_exec/worldmap1.gif";
  }
  bg_pixmap_.load(path_to_movie);
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
