// $Id$

#include "RootPanel.h"
#include <Qt/qmenubar.h>
#include <Qt/qapplication.h>
#include <Qt/qprogressdialog.h>
#include <Qt/qsplitter.h>
#include <stdlib.h>
#include "NodeItem.h"
#include "Command.h"


RootPanel::RootPanel(Q3Canvas &c, QWidget *parent, const char *name)
: QMainWindow(parent, name), canvas(c)
{
  QSplitter *s1 = new QSplitter( Qt::Vertical, this , "main" );
  navview = new MapView(canvas, s1);
  s1->moveToFirst(navview);

  details = new DetailView(s1);

  QMenuBar* menu = menuBar();

  Q3PopupMenu* file = new Q3PopupMenu;
    //file->insertItem("&Fill canvas", this, SLOT(init()), CTRL+Key_F);
    //file->insertItem("&Erase canvas", this, SLOT(clear()), CTRL+Key_E);
    //file->insertItem("&New view", this, SLOT(newView()), CTRL+Key_N);
    file->insertSeparator();
    file->insertItem("E&xit", qApp, SLOT(quit()), Qt::CTRL + Qt::Key_Q);
  menu->insertItem("&File", file);

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
  UnitLocation loc = unit->getLocation();
  el->move(loc.x_, loc.y_);
  el->show();
  this->details->currentNode(unit);
}

void
RootPanel::updateUnit(NavUnit *unit)
{
  NodeItem *el = 0;
  nodeMap.find(unit->getID(), el);
  UnitLocation loc = unit->getLocation();
  el->move(loc.x_, loc.y_);
  //canvas.update();

  this->details->updateLocation(loc);
}

void
RootPanel::clear()
{
  navview->clear();
}

void
RootPanel::customEvent(QCustomEvent *e)
{
  CommandBase *cmd = (CommandBase*)(e->data());
  cmd->execute();
  delete cmd;
/*
  int elapsed = time.restart();
  int fps = (elapsed == 0 ? 1000 : 1000 / elapsed);

  if(prev_fps != fps)
  {
    prev_fps = fps;
    ups->setNum(prev_fps);
  }
*/
}
