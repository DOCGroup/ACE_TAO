/* -*- C++ -*- */

// $Id$

// ============================================================================
//
// = LIBRARY
//    examples
// 
// = FILENAME
//    Reactor_Timer_Queue_Test.h
//
// = DESCRIPTION
//    This code is an implementation of a test driver for a reactor based
//    timer queue.
//
// = AUTHORS
//    Nanbor Wang <nw1@cs.wustl.edu> and
//    Sergio Flores-Gaitan <sergio@cs.wustl.edu>
// 
// ============================================================================

#if !defined (_REACTOR_TIMER_QUEUE_TEST_H_)
#define _REACTOR_TIMER_QUEUE_TEST_H_

#include "ace/Timer_Heap.h"
#include "Driver.h"

class Reactor_Timer_Queue_Test_Driver;

class Input_Handler : public ACE_Event_Handler
  // = TITLE
  //   Implements the handler to be called for input events.  Also has
  //   the logic to handle the different timer queue operations (i.e.,
  //   schedule, cancel, list, shutdown).
  //
  // = DESCRIPTION
  //   This class handles the reading of user input from stdin.  Also
  //   has the logic to handle the commands that are to be invoked in
  //   response to the user input.
{
public:
  typedef int (Input_Handler::*ACTION) (void *);

  Input_Handler (ACE_Timer_Queue *tq,
		 Reactor_Timer_Queue_Test_Driver &timer_queue_driver);
  // Sets <done_> flag to 0, <driver_> to <timer_queue_driver> and
  // timer queue <tq_> to <tq>

  int handle_input (ACE_HANDLE);
  // Hook method for the <ACE_Reactor> to call whenever there is input
  // ready to be read.

  int done (void);
  // returns the value for <done_> that indicates whether we are
  // exiting the program.A value of 0 indicates that we are NOT done,
  // 1 otherwise.

  // = Hook methods to be called from <Reactor_Timer_Queue_Test_Driver>

  int schedule_timer (void *argument);
  // Schedule a timer.  The (void *) will be mapped to the delay
  // parameter for the timer queue schedule method.

  int cancel_timer (void *argument);
  // Cancel a timer.  The (void *) will be mapped to the ID of the
  // timer beiing cancelled.

  int list_timer (void *argument);
  // Dump the timers in the queue.  The argument is ignored.

  int shutdown_timer (void *argument);
  // Processes the request to exit the timer queue application.
  // argument is ignored.

private:
  ACE_Timer_Queue *tq_;
  // Keep a pointer to the timer queue we are using so we can traverse
  // the queue.
  
  int done_;
  // Flag used to close down program.

  Reactor_Timer_Queue_Test_Driver &driver_;
  // Test driver.  Used to call hook methods that are common code for
  // all drivers.
};

class Reactor_Timer_Queue_Test_Driver : public Timer_Queue_Test_Driver <ACE_Timer_Heap, Input_Handler, Input_Handler::ACTION>
  // = TITLE
  //   Implements a test driver for a reactive timer queue using
  //   <ACE_Reactor>.  
  //
  // = DESCRIPTION
  //   This class implements the logic to test the reactor
  //   implementation of timer queue, using an <ACE_Timer_Heap>.
{
public:
  Reactor_Timer_Queue_Test_Driver (void);
  // Sets the input handler <thandler_> with <timer_queue_> from the
  // <Timer_Queue_Test_Driver> class and a reference to "this", so the
  // input handler can call hook methods from the driver.  Such
  // methods are the common factored out code from other
  // implementations of timer queues.
  
  virtual int display_menu (void);
  // Prints the menu of options.

  virtual int init (void);
  // Sets the timer queue that the REACTOR will use; registers the
  // stdin input handler with the REACTOR and sets the <Command>s that
  // the <Timer_Queue_Test_Driver> will execute().

  virtual int run_test (void);
  // Main entry point to the test driver implementation.
  
private:
  Input_Handler thandler_;
  // This is the stdin handler.
};

class Reactor_Timer_Handler : public ACE_Event_Handler
  // = TITLE
  //     Target of the reactive timeout operation.
{
public:
  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *);
  // Hook method that is called by the reactor when a timer expires.
  // It prints the timer ID and the time it expired.

  void set_timer_id (long tid);
  // Sets the timer id for this handler <tid_> to <tid>
  
private:
  long tid_;
  // timer ID.
};

#endif /* _REACTOR_TIMER_QUEUE_TEST_H_ */
