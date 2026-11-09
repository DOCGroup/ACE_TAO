#include "Control.h"

Control::Control (Widget parent)
{
  // Instantiate the  sub-components of the Control
  this->frame_ = XmCreateFrame (parent,
                                (char *) "frame",
                                nullptr,
                                0);

  this->rowcolumn_ = XmCreateRowColumn (this->frame_,
                                         (char *)"rwc",
                                         nullptr,
                                         0);

  this->startwidget_ = XmCreatePushButton (this->rowcolumn_,
                                           (char *) "Start",
                                           nullptr,
                                           0);

  this->stopwidget_ = XmCreatePushButton (this->rowcolumn_,
                                          (char *) "Stop",
                                          nullptr,
                                          0);
}

void
Control::manage ()
{
  XtManageChild (frame_);
  XtManageChild (rowcolumn_);
  XtManageChild (startwidget_);
  XtManageChild (stopwidget_);
}

Widget
Control::startwidget () const
{
  return this->startwidget_;
}

Widget
Control::stopwidget () const
{
  return this->stopwidget_;
}

