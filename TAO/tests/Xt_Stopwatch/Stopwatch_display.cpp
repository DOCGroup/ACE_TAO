#include "Stopwatch_display.h"
#include <cstdio>

Stopwatch_display::Stopwatch_display (Widget parent)
{
  // Instantiate the  sub-components of the Stopwatch_display
  this->frame_ = XtCreateWidget ("frame",
                                 xmFrameWidgetClass,
                                 parent,
                                 nullptr,
                                 0);

  this->label_ = XtCreateWidget ("label",
                                 xmLabelWidgetClass,
                                 this->frame_,
                                 nullptr,
                                 0);
}

void
Stopwatch_display::manage ()
{
  XtManageChild (this->frame_);
  XtManageChild (this->label_);
}

void
Stopwatch_display::set_time (CORBA::Float time)
{
  char buf[50];

  // Format value as a string
  std::snprintf (buf, sizeof (buf), "%6.3f", time);

  // Convert to compound string
  XmString const xmstr = XmStringCreateSimple (buf);

  // Display the string in the Label widget
  XtVaSetValues (this->label_, XmNlabelString, xmstr, static_cast<void *> (nullptr));
  //??Can use XtSetValues with ac and al values..

  // The compound string can be freed once passed to the widget
  XmStringFree (xmstr);
}

