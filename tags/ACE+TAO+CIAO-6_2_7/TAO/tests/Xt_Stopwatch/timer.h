
//=============================================================================
/**
 *  @file   timer.h
 *
 *  $Id$
 *
 *  @author Bala <bala@cs.wustl.edu>
 */
//=============================================================================


#ifndef TIMER_H
#define TIMER_H

#include "Stopwatch_display.h"


#include <Xm/Xm.h>


class Timer_imp
{
 public:

  /// Constructor
  Timer_imp (XtAppContext &,
             CORBA::Long,
             Stopwatch_display *);

  /// Destructor
  virtual ~Timer_imp ();

  /// Resets, and starts the clock ticking
  void start (void);

  /// Stops the clock
  void stop (void);

  /// Returns time since timer started
  CORBA::Float  elapsed_time (void);

 private:

  /// Static member function used for TimeOut callback.
  static void tick_callback (XtPointer,
                             XtIntervalId *);

  /// Called every interval_ milliseconds
  void tick (void);

  /// Called at each clock tick...
  virtual void report_time (CORBA::Float);

  /// A copy of the stopwatch object
  Stopwatch_display *stopwatch_;

  /// Current number of ticks
  CORBA::Long counter_;

  /// Time in milliseconds between updates
  CORBA::Long interval_;

  /// Identifier of current TimeOut
  XtIntervalId id_;

  /// Required by Xt functions
  XtAppContext app_;
};


#endif /* TIMER_H */
