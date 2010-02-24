// $Id$

#include "Worker.h"
#include <Qt/qapplication.h>
#include <Qt/qmotifstyle.h>
#include <Qt/qcdestyle.h>
#include <Qt/qwindowsstyle.h>
#include <Qt/qdesktopwidget.h>

int
Worker::svc (void)
{
  ACE_DEBUG((LM_DEBUG, "ENTER: Worker::svc()\n"));

  QApplication a(argc_, argv_);
//  a.setStyle(new QPlatinumStyle);
  Q3Canvas canvas(600, 387);
  //QCanvas canvas(566, 679);
  //QCanvas canvas(550, 723);
  canvas.setAdvancePeriod(30);
  RootPanel root_panel(canvas);
  root_panel.resize(root_panel.sizeHint());

  root_panel.resize(600, 650);
  root_panel.setCaption("NavDisplay");

  a.setMainWidget(&root_panel);

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
