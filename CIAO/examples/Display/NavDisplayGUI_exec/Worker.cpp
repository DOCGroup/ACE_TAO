// $Id$

#include "Worker.h"
#include <Qt/qapplication.h>
#include <Qt/qmotifstyle.h>
#include <Qt/qcdestyle.h>
#include <Qt/qwindowsstyle.h>
#include <Qt/qdesktopwidget.h>
#include <Qt/qgraphicsscene.h>

int
Worker::svc (void)
{
  ACE_DEBUG((LM_DEBUG, "ENTER: Worker::svc()\n"));

  QApplication a(argc_, argv_);
  QGraphicsScene canvas(1,1,650, 410);
  RootPanel root_panel(canvas);
  root_panel.resize(620, 460);
  root_panel.setWindowTitle("NavDisplay");

  a.setActiveWindow(&root_panel);

  QObject::connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );

  if(QApplication::desktop()->width() > 550
    && QApplication::desktop()->height() > 366)
    root_panel.show();
  else
    root_panel.showMaximized();

  main_wnd_ = &root_panel;
  init_complete_.signal();

  int retval = a.exec();

  main_wnd_ = 0;

  ACE_DEBUG((LM_DEBUG, "LEAVE: Worker::svc()\n"));
  return retval;
}
