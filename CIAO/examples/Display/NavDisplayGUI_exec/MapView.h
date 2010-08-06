// $Id$

#ifndef __MAPVIEW_H
#define __MAPVIEW_H

#include <qcanvas.h>
#include <qpixmap.h>

class MapView : public QCanvasView
{
  Q_OBJECT

public:
  MapView(QCanvas&, QWidget* parent=0, const char* name=0, WFlags f=0);
  void clear();

protected:
  QPixmap bg_pixmap_;
};



#endif // __MAPVIEW_H
