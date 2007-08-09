/* -*- C++ -*- */

// $Id$

// ============================================================================
//
// = LIBRARY
//    examples
//
// = FILENAME
//    Thread_Timer_Queue_Test.h
//
// = DESCRIPTION
//    This code exercises the <ACE_Thread_Timer_Queue_Adapter> using
//    an <ACE_Timer_Heap_T>.
//
// = AUTHORS
//    Carlos O'Ryan <coryan@cs.wustl.edu> and
//    Sergio Flores-Gaitan <sergio@cs.wustl.edu>
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

// These typedefs ensure that we use the minimal amount of locking
// necessary.
typedef ACE_Event_Handler_Handle_Timeout_Upcall<ACE_Null_Mutex>
        Upcall;
typedef ACE_Timer_Heap_T<ACE_Event_Handler *,
                         Upcall,
                         ACE_Null_Mutex>
        Timer_Heap;
typedef ACE_Timer_Heap_Iterator_T<ACE_Event_Handler *,
                                  Upcall,
                                  ACE_Null_Mutex>
        Timer_Heap_Iterator;
typedef ACE_Thread_Timer_Queue_Adapter<Timer_Heap>
        Thread_Timer_Queue;

// Forward declaration.
class Thread_Timer_Queue_Test_Driver;

class Input_Task : public ACE_Task_Base
{
  // = TITLE
  //   Read user actions on the Timer_Queue from stdin.
  //
  // = DESCRIPTION
  //   This class reads user input from stdin; those commands permit
  //   the control of a Timer_Queue, which is dispatched by another
  //   thread.
public:
  typedef int (Input_Task::*ACTION) (void *);

  Input_Task (Thread_Timer_Queue *queue,
              Thread_Timer_Queue_Test_Driver &timer_queue_driver);

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

  Thread_Timer_Queue_Test_Driver &driver_;
  // The thread timer queue test driver.
};

class ACE_Svc_Export Thread_Timer_Queue_Test_Driver : public Timer_Queue_Test_Driver <Thread_Timer_Queue, Input_Task, Input_Task::ACTION>
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
  Thread_Timer_Queue_Test_Driver (void);
  ~Thread_Timer_Queue_Test_Driver (void);

  virtual int display_menu (void);
  virtual int init (void);
  virtual int run_test (void);

private:
  Input_Task input_task_;
  // Subclassed from ACE_Task.
};

class Handler : public ACE_Event_Handler
{
  // = TITLE
  //     Event handler for the timer queue timeout events.
  //
  // = DESCRIPTION
  //     The <handle_timeout> hook method prints out the current time,
  //     prints the time when this timer expired and deletes "this".
public:
  Handler (const ACE_Time_Value &expiration_time);
  ~Handler (void);

  void set_id (int id);
  // Store an "id" for the Handler, which is only use to print better
  // messages.

  virtual int handle_timeout (const ACE_Time_Value &current_time,
                              const void *arg);
  // Call back hook.

private:
  ACE_Time_Value expires_;
  // Store the expected time of expiration, it is used to print a nice
  // message saying how much delay was at the actual expiration time.

  int id_;
  // Store an "id" for the Handler, which is only use to print better
  // messages.
};

#endif /* _THREAD_TIMER_QUEUE_TEST_H_ */
