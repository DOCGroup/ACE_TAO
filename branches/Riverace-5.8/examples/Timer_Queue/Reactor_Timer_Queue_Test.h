/* -*- C++ -*- */


//=============================================================================
/**
 *  @file    Reactor_Timer_Queue_Test.h
 *
 *  $Id$
 *
 *  This code is an implementation of a test driver for a reactor based
 *  timer queue.
 *
 *
 *  @author Nanbor Wang <nw1@cs.wustl.edu> and Sergio Flores-Gaitan <sergio@cs.wustl.edu>
 */
//=============================================================================


#ifndef _REACTOR_TIMER_QUEUE_TEST_H_
#define _REACTOR_TIMER_QUEUE_TEST_H_

#include "Driver.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/// @@todo: Not sure why this needs to be included. But am sure that,
/// there is some circular dependency setup. Needs to be
/// fixed. Atleast on g++
#include "ace/Timer_Queue.h"
#include "ace/Timer_Heap.h"
#include "ace/svc_export.h"

class Reactor_Timer_Queue_Test_Driver;

/**
 * @class Input_Handler
 *
 * @brief Implements the handler to be called for input events.  Also has
 * the logic to handle the different timer queue operations (i.e.,
 * schedule, cancel, list, shutdown).
 *
 * This class handles the reading of user input from stdin.  Also
 * has the logic to handle the commands that are to be invoked in
 * response to the user input.
 */
class Input_Handler : public ACE_Event_Handler
{
public:
  typedef int (Input_Handler::*ACTION) (void *);

  /// Sets <done_> flag to 0, <driver_> to <timer_queue_driver> and
  /// timer queue <tq_> to <tq>
  Input_Handler (ACE_Timer_Queue *tq,
                 Reactor_Timer_Queue_Test_Driver &timer_queue_driver);

  /// Hook method for the <ACE_Reactor> to call whenever there is input
  /// ready to be read.
  int handle_input (ACE_HANDLE);

  /**
   * returns the value for <done_> that indicates whether we are
   * exiting the program.A value of 0 indicates that we are NOT done,
   * 1 otherwise.
   */
  int done (void);

  // = Hook methods to be called from <Reactor_Timer_Queue_Test_Driver>

  /// Schedule a timer.  The (void *) will be mapped to the delay
  /// parameter for the timer queue schedule method.
  int schedule_timer (void *argument);

  /// Cancel a timer.  The (void *) will be mapped to the ID of the
  /// timer being cancelled.
  int cancel_timer (void *argument);

  /// Dump the timers in the queue.  The argument is ignored.
  int list_timer (void *argument);

  /// Processes the request to exit the timer queue application.
  /// argument is ignored.
  int shutdown_timer (void *argument);

private:
  /// Keep a pointer to the timer queue we are using so we can traverse
  /// the queue.
  ACE_Timer_Queue *tq_;

  /// Flag used to close down program.
  int done_;

  /// Test driver.  Used to call hook methods that are common code for
  /// all drivers.
  Reactor_Timer_Queue_Test_Driver &driver_;
};

/**
 * @class Reactor_Timer_Queue_Test_Driver
 *
 * @brief Implements a test driver for a reactive timer queue using
 * <ACE_Reactor>.
 *
 * This class implements the logic to test the reactor
 * implementation of timer queue, using an <ACE_Timer_Heap>.
 */
class ACE_Svc_Export Reactor_Timer_Queue_Test_Driver : public Timer_Queue_Test_Driver <ACE_Timer_Heap, Input_Handler, Input_Handler::ACTION>
{
public:
  /**
   * Sets the input handler <thandler_> with <timer_queue_> from the
   * <Timer_Queue_Test_Driver> class and a reference to "this", so the
   * input handler can call hook methods from the driver.  Such
   * methods are the common factored out code from other
   * implementations of timer queues.
   */
  Reactor_Timer_Queue_Test_Driver (void);

  /// Default destructor
  virtual ~Reactor_Timer_Queue_Test_Driver (void);

  /// Prints the menu of options.
  virtual int display_menu (void);

  /**
   * Sets the timer queue that the REACTOR will use; registers the
   * stdin input handler with the REACTOR and sets the <Command>s that
   * the <Timer_Queue_Test_Driver> will execute().
   */
  virtual int init (void);

  /// Main entry point to the test driver implementation.
  virtual int run_test (void);

private:
  /// This is the stdin handler.
  Input_Handler thandler_;
};

/**
 * @class Reactor_Timer_Handler
 *
 * @brief Target of the reactive timeout operation.
 */
class Reactor_Timer_Handler : public ACE_Event_Handler
{
public:
  /// Hook method that is called by the reactor when a timer expires.
  /// It prints the timer ID and the time it expired.
  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *);

  /// Sets the timer id for this handler <tid_> to <tid>
  void set_timer_id (long tid);

private:
  /// timer ID.
  long tid_;
};

#endif /* _REACTOR_TIMER_QUEUE_TEST_H_ */
