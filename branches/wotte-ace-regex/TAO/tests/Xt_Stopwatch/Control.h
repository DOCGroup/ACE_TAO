// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO/tests/Xt_Stopwatch/Stopwatch_client
//
// = FILENAME
//   Control.h
//
// = AUTHOR
//   Balachandran  Natarajan <bala@cs.wustl.edu>
//
// ============================================================================

#ifndef CONTROL_H
#define CONTROL_H
#include "Client.h"

#if defined (ACE_HAS_XT)

#include <Xm/Xm.h>
#include <Xm/PushB.h>
#include <Xm/Label.h>
#include <Xm/RowColumn.h>
#include <Xm/Frame.h>


class Control
{
 public:
  Control (Widget &parent);
  // Stopwatch constructor

  virtual ~Control (void);
  // Destructor

  virtual void manage (void);
  // Manage the widget

  Widget& startwidget (void);
  // Return the start widget

  Widget& stopwidget (void);
  // Return the stop widget..

 private:

  Widget frame_;
  // A frame widget...

  Widget rowcolumn_;
  // A row column widget..

  Widget startwidget_;
  // Pushbutton  Widget that is used to start the  timer..

  Widget stopwidget_;
  // Pushbutton widget that is used to stop the timer..

};

#endif /*ACE_HAS_XT */

#endif /* CONTROL_H */
