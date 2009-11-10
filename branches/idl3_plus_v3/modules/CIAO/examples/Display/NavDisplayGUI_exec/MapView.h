// $Id$

#ifndef __MAPVIEW_H
#define __MAPVIEW_H

#include <Qt/q3canvas.h>
#include <Qt/qpixmap.h>

class MapView : public Q3CanvasView
{
  Q_OBJECT

public:
  MapView(Q3Canvas&, QWidget* parent=0, const char* name=0, Qt::WindowFlags f=0);
  void clear();

protected:
  QPixmap bg_pixmap_;
};



#endif // __MAPVIEW_H
