// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO/tests/Xt_Stopwatch
//
// = FILENAME
//   timer.h
//
// = AUTHOR
//   Bala <bala@cs.wustl.edu>
//
// ============================================================================

#ifndef TIMER_H
#define TIMER_H

#include "Stopwatch_display.h"

#if defined (ACE_HAS_XT)

#include <Xm/Xm.h>


class Timer_imp
{
 public:

  Timer_imp (XtAppContext &,
             CORBA::Long,
             Stopwatch_display *);
  // Constructor..

  void start (void);
  // Resets, and starts the clock ticking

  void stop (void);
  // Stops the clock

  CORBA::Float  elapsed_time (void);
  // Returns time since timer started

 private:

  static void tick_callback (XtPointer,
                             XtIntervalId *);
  // Static member function used for TimeOut callback.

  void tick (void);
  // Called every interval_ milliseconds

  virtual void report_time (CORBA::Float);
  // Called at each clock tick...

  Stopwatch_display *stopwatch_;
  // A copy of the stopwatch object

  CORBA::Long counter_;
  // Current number of ticks

  CORBA::Long interval_;
  // Time in milliseconds between updates

  XtIntervalId id_;
  // Identifier of current TimeOut

  XtAppContext app_;
  // Required by Xt functions
};

#endif /*ACE_HAS_XT*/

#endif /* TIMER_H */
