// $Id$
#include "Stopwatch_display.h"

Stopwatch_display::Stopwatch_display (Widget &parent )
{
  // Instantiate the  sub-components of the Stopwatch_display
  this->frame_ = XtCreateWidget ("frame", xmFrameWidgetClass,
                                 parent, 0, 0 );
  this->label_ = XtCreateWidget ("label", xmLabelWidgetClass,
                                 this->frame_, 0, 0 );
  //_timer = new Stopwatch_displayTimer ( XtWidgetToApplicationContext ( parent ), 
  //                            _face, 
  //                            1000 );
}

Stopwatch_display::~Stopwatch_display ( )
{
  //delete _face;
  //delete _timer;
  // delete _control;
}

void 
Stopwatch_display::manage (void)
{
  XtManageChild (this->frame_);
  XtManageChild (this->label_);
}

/*void Stopwatch_display::timerStarted()
{
    // Empty
}

void Stopwatch_display::timerStopped()
{
    // Empty
}
*/


