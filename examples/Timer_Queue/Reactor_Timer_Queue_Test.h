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
  //   Input_Handler
  //
  // = DESCRIPTION
  //   This class handles the reading of user input from stdin.  Also
  //   has the logic to handle the commands that are to be invoked in
  //   response to the user input.
{
public:
  typedef int (Input_Handler::*ACTION) (void *);

  // @@ Please comment all these methods.
  Input_Handler (ACE_Timer_Queue *tq,
		 Reactor_Timer_Queue_Test_Driver &timer_queue_driver);
  int handle_input (ACE_HANDLE);

  int done (void);
  int schedule_timer (void *argument);
  int cancel_timer (void *argument);
  int list_timer (void *argument);
  int shutdown_timer (void *argument);

private:
  ACE_Timer_Queue *tq_;
  // Keep a pointer to the timer queue we are using so we can
  // traverse the queue.
  
  int done_;
  // Flag used to close down program.

  Reactor_Timer_Queue_Test_Driver &driver_;
  // Test driver.
};

class Reactor_Timer_Queue_Test_Driver : public Timer_Queue_Test_Driver <ACE_Timer_Heap, Input_Handler, Input_Handler::ACTION>
  // = TITLE
  //   Reactor_Timer_Queue_Test_Driver
  //
  // = DESCRIPTION
  //   This class implements the logic to test the reactor
  //   implementation of timer queue, using an <ACE_Timer_Heap>.
{
public:
  // Please comment all these methods.
  Reactor_Timer_Queue_Test_Driver (void);
  
  virtual int display_menu (void);
  virtual int init (void);
  virtual int run_test (void);
  
private:
  // This is the stdin handler.
  Input_Handler thandler;
};

class Reactor_Timer_Handler : public ACE_Event_Handler
  // = TITLE
  //     Target of the reactive timeout operation.
{
public:
  // @@ Please comment all these methods.
  Reactor_Timer_Handler (void);

  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *);

  void set_timer_id (long tid);
  
private:
  long tid_;
};

#endif /* _REACTOR_TIMER_QUEUE_TEST_H_ */
