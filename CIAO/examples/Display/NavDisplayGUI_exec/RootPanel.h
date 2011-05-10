// $Id$

#ifndef __ROOTPANEL_H
#define __ROOTPANEL_H

#define QT_GENUINE_STR

#include "ace/Hash_Map_Manager.h"
#include "ace/Null_Mutex.h"
#include "MapView.h"
#include "DetailView.h"
#include <Qt/qmainwindow.h>
#include <Qt/qevent.h>
#include <Qt/qgraphicsscene.h>

#include "Command.h"

class NodeItem;
class QMouseEvent;

class NavEvent : public QEvent
{
public:
  NavEvent(CommandBase* cmd);
  virtual ~NavEvent ();

  CommandBase* cmd () const;

  static void set_type (int type);

  static QEvent::Type get_type ();

private:
  CommandBase* cmd_;

  static int registered_type_;
};

class RootPanel : public QMainWindow
{
  Q_OBJECT

typedef ACE_Hash_Map_Manager_Ex<ACE_UINT64,
                                NodeItem*,
                                ACE_Hash<ACE_UINT64>,
                                ACE_Equal_To<ACE_UINT64>,
                                ACE_Null_Mutex> NodeItemMap;

public:
  RootPanel(QGraphicsScene&, QWidget *parent = 0);
  ~RootPanel();

public:
  void addUnit(NavUnit* unit);
  void updateUnit(NavUnit *unit);

protected:
  void clear();
  virtual void customEvent(QEvent *e);

  QGraphicsScene& canvas;
  MapView *navview;
  DetailView *details;
  NodeItemMap nodeMap;
};

#endif // __ROOTPANEL_H
