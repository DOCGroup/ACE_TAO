// $Id$

#ifndef __MAPVIEW_H
#define __MAPVIEW_H

#include <Qt/qpixmap.h>
#include <Qt/qgraphicsscene.h>
#include <Qt/qgraphicsview.h>
#include <Qt/qwidget.h>
#include <Qt/qevent.h>

class MapView : public QGraphicsView
{
  Q_OBJECT

public:
  MapView(QGraphicsScene&, QWidget* parent=0);
  void clear();
  void resizeEvent (QResizeEvent *);

protected:
  QPixmap bg_pixmap_;
};



#endif // __MAPVIEW_H
