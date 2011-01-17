// $Id$

#ifndef __NODEITEM_H
#define __NODEITEM_H

#include "NavUnit.h"
#include <Qt/q3canvas.h>
#include <Qt/qtimer.h>
#include <Qt/qbrush.h>
#include <Qt/qobject.h>

class NodeItem: public QObject,
                public Q3CanvasEllipse
{
  Q_OBJECT

public:
  NodeItem(Q3Canvas *canvas, NavUnit &unit);

  ~NodeItem() {}

  enum {
    nodeRTTI = 1
  };

  void moveBy(double dx, double dy, double dz);

  virtual int rtti() const {return nodeRTTI;}

  virtual void setSelected(bool yes);
  virtual bool selected() const {return is_selected_;}

public slots:
  void timerDone();

private:
  NavUnit &unit_;
  QTimer blink_timer;
  bool is_selected_;

  static QBrush *normal;
  static QBrush *highlighted;
};



#endif // __NODEITEM_H
