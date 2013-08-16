// $Id$

#include "RootPanel.h"
#include <QtGui/qmenubar.h>
#include <QtGui/qapplication.h>
#include <QtGui/qsplitter.h>
#include <stdlib.h>
#include "NodeItem.h"
#include "Command.h"


RootPanel::RootPanel(QGraphicsScene &c, QWidget *parent)
: QMainWindow(parent), canvas(c)
{
  QSplitter *s1 = new QSplitter( Qt::Vertical, parent );

  navview = new MapView(canvas, s1);
  details = new DetailView(s1);

  QMenu *filemenu = menuBar()->addMenu(tr("&File"));
  QAction *exitAct = new QAction(tr("E&xit"), this);
  exitAct->setShortcut(Qt::CTRL + Qt::Key_Q);
  connect(exitAct, SIGNAL(triggered()), qApp, SLOT(quit()));
  filemenu->addAction(exitAct);
  setCentralWidget(s1);
}

RootPanel::~RootPanel()
{
  clear();
}

void
RootPanel::addUnit(NavUnit *unit)
{
  NodeItem *el = new NodeItem(&canvas, *unit);
  nodeMap.bind(unit->getID(), el);
  // only for first GPS  write details.
  if(unit->getID() == 1)
    {
      this->details->currentNode(unit);
    }
}

void
RootPanel::updateUnit(NavUnit *unit)
{
  NodeItem *el = 0;
  nodeMap.find(unit->getID(), el);

  UnitLocation loc = unit->getLocation();
  //width is incl. borders and text, so correct for this .
  loc.x_ = (long)loc.x_ % (navview->width() - 40);
  loc.y_ = (long)loc.y_ % (navview->height() - 80);

  el->moveBy(loc.x_ - el->pos().x() , loc.y_ - el->pos().y());
  el->show();
  canvas.update();
  if(unit->getID() == 1)
    {
      this->details->updateLocation(loc);
    }
}

void
RootPanel::clear()
{
  navview->clear();
}

int NavEvent::registered_type_ = (-1);

NavEvent::NavEvent(CommandBase* cmd)
 : QEvent (static_cast<QEvent::Type> (NavEvent::registered_type_)),
   cmd_ (cmd)
{
}
NavEvent::~NavEvent ()
{
}

CommandBase* NavEvent::cmd () const
{
  return this->cmd_;
}

void NavEvent::set_type (int type)
{
  NavEvent::registered_type_ = type;
}

QEvent::Type NavEvent::get_type ()
{
  return static_cast<QEvent::Type> (NavEvent::registered_type_);
}

void
RootPanel::customEvent(QEvent *e)
{
  if (e->type () == NavEvent::get_type ())
    {
      NavEvent* ne = dynamic_cast<NavEvent*> (e);
      CommandBase* cmd = ne->cmd ();
      cmd->execute();
      delete cmd;
    }
}
