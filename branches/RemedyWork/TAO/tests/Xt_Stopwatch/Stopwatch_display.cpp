// $Id$

#include "Stopwatch_display.h"
#include "ace/OS_NS_stdio.h"

Stopwatch_display::Stopwatch_display (Widget &parent)
{
  // Instantiate the  sub-components of the Stopwatch_display
  this->frame_ = XtCreateWidget ("frame",
                                 xmFrameWidgetClass,
                                 parent,
                                 0,
                                 0);

  this->label_ = XtCreateWidget ("label",
                                 xmLabelWidgetClass,
                                 this->frame_,
                                 0,
                                 0);
}

Stopwatch_display::~Stopwatch_display (void)
{
  //No-op
}

void
Stopwatch_display::manage (void)
{
  XtManageChild (this->frame_);
  XtManageChild (this->label_);
}

void
Stopwatch_display::set_time (CORBA::Float time)
{
  char buf[50];

  // Format value as a string
  ACE_OS::sprintf (buf, "%6.3f", time);

  // Convert to compound string
  XmString xmstr = XmStringCreateSimple (buf);

  // Display the string in the Label widget
  XtVaSetValues (this->label_, XmNlabelString, xmstr, static_cast<void *>(0));
  //??Can use XtSetValues with ac and al values..

  // The compound string can be freed once passed to the widget
  XmStringFree (xmstr);
}

