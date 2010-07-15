/* -*- C++ -*- */

// $Id$
// ============================================================================
//
// = LIBRARY
//    examples
//
// = FILENAME
//    Thread_Timer_Queue_Custom_Handler_Test.h
//
// = DESCRIPTION
//    This code exercises the <ACE_Thread_Timer_Queue_Adapter> using
//    an <ACE_Timer_Heap_T>. It also demonstrates using a custom handler for
//    timer events.
//
// = AUTHORS
//    Carlos O'Ryan <coryan@cs.wustl.edu> and
//    Sergio Flores-Gaitan <sergio@cs.wustl.edu> and
//    Alon Diamant <diamant.alon@gmail.com>
//
// ============================================================================

#ifndef _THREAD_TIMER_QUEUE_TEST_H_
#define _THREAD_TIMER_QUEUE_TEST_H_

#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Null_Mutex.h"
#include "ace/Timer_Heap_T.h"
#include "ace/Timer_Queue_Adapters.h"
#include "ace/svc_export.h"
#include "ace/Condition_Recursive_Thread_Mutex.h"
#include "Driver.h"
#include "Custom_Handler.h"

// These typedefs ensure that we use the minimal amount of locking
// necessary.
typedef ACE_Timer_Heap_T<Custom_Handler*,
                         Custom_Handler_Upcall,
                         ACE_Null_Mutex>
        Timer_Heap;
typedef ACE_Timer_Heap_Iterator_T<Custom_Handler*,
                                  Custom_Handler_Upcall,
                                  ACE_Null_Mutex>
        Timer_Heap_Iterator;
typedef ACE_Thread_Timer_Queue_Adapter<Timer_Heap, Custom_Handler*>
        Thread_Timer_Queue;

// Forward declaration.
class Thread_Timer_Queue_Custom_Handler_Test;

class Custom_Handler_Input_Task : public ACE_Task_Base
{
  // = TITLE
  //   Read user actions on the Timer_Queue from stdin.
  //
  // = DESCRIPTION
  //   This class reads user input from stdin; those commands permit
  //   the control of a Timer_Queue, which is dispatched by another
  //   thread.
public:
  typedef int (Custom_Handler_Input_Task::*ACTION) (void *);

  Custom_Handler_Input_Task (Thread_Timer_Queue *queue,
              Thread_Timer_Queue_Custom_Handler_Test &timer_queue_driver);

  virtual int svc (void);
  // This method runs the event loop in the new thread.

  // = Some helper methods.

  int add_timer (void *);
  // Add a new timer to expire in <seconds> more.

  int cancel_timer (void *);
  // Cancel timer <id>.

  int list_timer (void *);
  // List the current scheduled timers.

  int shutdown_timer (void *);
  // Shutdown task.

  void dump (void);
  // Dump the state of the timer queue.

private:
  Thread_Timer_Queue *queue_;
  // The timer queue implementation.

  const int usecs_;
  // How many micro seconds are in a second.

  Thread_Timer_Queue_Custom_Handler_Test &driver_;
  // The thread timer queue test driver.
};

class ACE_Svc_Export Thread_Timer_Queue_Custom_Handler_Test : public Timer_Queue_Test_Driver <Thread_Timer_Queue, Custom_Handler_Input_Task, Custom_Handler_Input_Task::ACTION>
{
  // = TITLE
  //    Implements an example application that exercises
  //    <Thread_Timer_Queue> timer queue.
  //
  // = DESCRIPTION
  //    This class implements a simple test driver for the
  //    <Thread_Timer_Queue>.  The <display_menu> hook method is
  //    called from the base class to print a menu specific to the
  //    thread implementation of the timer queue.
public:
  Thread_Timer_Queue_Custom_Handler_Test (void);
  ~Thread_Timer_Queue_Custom_Handler_Test (void);

  virtual int display_menu (void);
  virtual int init (void);
  virtual int run_test (void);

private:
  Custom_Handler_Input_Task input_task_;
  // Subclassed from ACE_Task.
};

#endif /* _THREAD_TIMER_QUEUE_TEST_H_ */
