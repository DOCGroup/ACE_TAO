/* -*- C++ -*- */

// $Id$

// ============================================================================
//
// = LIBRARY
//    examples
// 
// = FILENAME
//    Async_Timer_Queue_Test.h
//
// = DESCRIPTION
//      This test exercises the <ACE_Asynch_Timer_Queue_Adapter> 
//      using an <ACE_Timer_Heap>.
//
// = AUTHORS
//    Douglas C. Schmidt and
//    Sergio Flores-Gaitan
// ============================================================================

#if !defined (_ASYNC_TIMER_QUEUE_TEST_H_)
#define _ASYNC_TIMER_QUEUE_TEST_H_

#include "ace/Signal.h"
#include "ace/Timer_Heap.h"
#include "ace/Timer_Queue_Adapters.h"

#include "Driver.h"

class Async_Timer_Handler : public ACE_Event_Handler
  // = TITLE
  //     Target of the asynchronous timeout operation.
{
public:
  virtual int handle_timeout (const ACE_Time_Value &tv,
			      const void *arg);
  // Callback hook invoked by the <Timer_Queue>.
};

class Async_Timer_Queue 
  // = TITLE
  //     Asynchronous Timer Queue Singleton.
  //
  // = DESCRIPTION
  //     We use this class to avoid global variables and to
  //     consolidate all the Timer Queue processing in one central 
  //     place. 
{
public:
  typedef int (Async_Timer_Queue::*ACTION) (void *);

  static Async_Timer_Queue *instance (void);
   // Singleton access point.

  void schedule (u_int microsecs);
  // Schedule a timer to expire <microsecs> in the future.

  void cancel (long timer_id);
  // Cancel a timer with <timer_id>.

  void dump (void);
  // Dump the contents of the queue.

  int schedule_timer (void *argument);
  // hook method to schedule a timer.  Called from <Timer_Queue_Test_Driver>

  int cancel_timer (void *argument);
  // hook method to cancel a timer.  Called from <Timer_Queue_Test_Driver>

  int list_timer (void *argument);
  // hook method to list timers.  Called from <Timer_Queue_Test_Driver>

  int shutdown_timer (void *argument);
  // hook method to exit the timer queue.  Called from <Timer_Queue_Test_Driver>

private:
  Async_Timer_Queue (ACE_Sig_Set *);
  // Private constructor enforces the Singleton.

  static Async_Timer_Queue *instance_;
  // Pointer to the timer queue.

  ACE_Async_Timer_Queue_Adapter<ACE_Timer_Heap> tq_;
  // The adapter is instantiated by an <ACE_Timer_Heap>.
};

class Async_Timer_Queue_Test_Driver : public Timer_Queue_Test_Driver <Async_Timer_Queue *, Async_Timer_Queue, Async_Timer_Queue::ACTION>
  // = TITLE
  //    Async_Timer_Queue_Test_Driver
  //
  // = DESCRIPTION
  //    This class implements a test driver for the
  //    <Async_Timer_Queue>.  Implements a display_menu() method that
  //    prints the options for a user. and init() which initializes
  //    the driver.  The rest of the common functionality is in the
  //    parent class <Timer_Queue_Test_Driver>.
{
public:
  Async_Timer_Queue_Test_Driver (void);

  // Print menu of options.

  virtual int display_menu (void);

  // Initializes the driver's internal variables inherited from the parent

  virtual int init (void);
};

#endif /* _ASYNC_TIMER_QUEUE_TEST_H_ */
