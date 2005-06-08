// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO/tests/Xt_Stopwatch/Stopwatch_server
//
// = FILENAME
//   Stopwatch.h
//
// = AUTHOR
//   Balachandran  Natarajan <bala@cs.wustl.edu>
//
// ============================================================================

#ifndef STOPWATCH_H
#define STOPWATCH_H
#include /**/ "ace/pre.h"
#include "tao/ORB_Constants.h"

#include <Xm/Xm.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>

class Stopwatch_display
{
public:

  Stopwatch_display (Widget &parent);
  // Stopwatch_display constructor

  virtual ~Stopwatch_display (void);
  // Destructor

  virtual void manage (void);
  // Manage the widget

  void set_time (CORBA::Float value);
  // Change the displayed time

private:
  Widget frame_;
  // A frame widget...

  Widget label_;
  // Label Widget that is used to display time..
};


#include /**/ "ace/post.h"
#endif /* STOPWATCH_H */
