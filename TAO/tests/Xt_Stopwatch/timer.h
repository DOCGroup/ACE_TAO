
//=============================================================================
/**
 *  @file   timer.h
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
  Timer_imp (XtAppContext,
             CORBA::Long,
             Stopwatch_display *);

  /// Destructor
  virtual ~Timer_imp () = default;

  Timer_imp (Timer_imp const &) = delete;
  Timer_imp &operator= (Timer_imp const &) = delete;
  Timer_imp (Timer_imp &&) = delete;
  Timer_imp &operator= (Timer_imp &&) = delete;

  /// Resets, and starts the clock ticking
  void start ();

  /// Stops the clock
  void stop ();

  /// Returns time since timer started
  CORBA::Float elapsed_time () const;

 private:
  /// Static member function used for TimeOut callback.
  static void tick_callback (XtPointer,
                             XtIntervalId *);

  /// Called every interval_ milliseconds
  void tick ();

  /// Called at each clock tick...
  virtual void report_time (CORBA::Float);

  /// Borrowed display object. The server keeps it alive until after the timer.
  Stopwatch_display *stopwatch_ {}; // Non-owning; the display outlives the timer.

  /// Current number of ticks
  CORBA::Long counter_ {};

  /// Time in milliseconds between updates
  CORBA::Long interval_ {};

  /// Identifier of current TimeOut
  XtIntervalId id_ {};

  /// Required by Xt functions
  XtAppContext app_ {};
};

#endif /* TIMER_H */
