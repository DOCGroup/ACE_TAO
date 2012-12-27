
//=============================================================================
/**
 *  @file   Stopwatch_display.h
 *
 *  $Id$
 *
 *  @author Balachandran  Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

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

  /// Stopwatch_display constructor
  Stopwatch_display (Widget &parent);

  /// Destructor
  virtual ~Stopwatch_display (void);

  /// Manage the widget
  virtual void manage (void);

  /// Change the displayed time
  void set_time (CORBA::Float value);

private:
  /// A frame widget...
  Widget frame_;

  /// Label Widget that is used to display time..
  Widget label_;
};


#include /**/ "ace/post.h"
#endif /* STOPWATCH_H */
