//=============================================================================
/**
 *  @file   Stopwatch_display.h
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
  explicit Stopwatch_display (Widget parent);

  virtual ~Stopwatch_display () = default;

  Stopwatch_display (Stopwatch_display const &) = delete;
  Stopwatch_display &operator= (Stopwatch_display const &) = delete;
  Stopwatch_display (Stopwatch_display &&) = delete;
  Stopwatch_display &operator= (Stopwatch_display &&) = delete;

  /// Manage the widget
  virtual void manage ();

  /// Change the displayed time
  void set_time (CORBA::Float value);

private:
  /// A frame widget...
  Widget frame_ {};

  /// Label Widget that is used to display time..
  Widget label_ {};
};

#include /**/ "ace/post.h"
#endif /* STOPWATCH_H */
