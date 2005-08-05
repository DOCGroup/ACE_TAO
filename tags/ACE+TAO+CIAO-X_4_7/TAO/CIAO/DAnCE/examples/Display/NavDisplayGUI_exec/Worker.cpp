// $Id$

#include "Worker.h"
#include <qapplication.h>
#include <qmotifstyle.h>
#include <qcdestyle.h>
#include <qwindowsstyle.h>
#include <qplatinumstyle.h>
#include <qsgistyle.h>


int
Worker::svc (void)
{
  ACE_DEBUG((LM_DEBUG, "ENTER: Worker::svc()\n"));

  QApplication a(argc_, argv_);
  a.setStyle(new QPlatinumStyle);
  QCanvas canvas(600, 387);
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

  main_wnd_ = NULL;

  ACE_DEBUG((LM_DEBUG, "LEAVE: Worker::svc()\n"));
  return retval;
}
