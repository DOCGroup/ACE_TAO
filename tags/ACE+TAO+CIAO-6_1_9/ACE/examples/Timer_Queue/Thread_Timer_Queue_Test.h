/* -*- C++ -*- */


//=============================================================================
/**
 *  @file    Thread_Timer_Queue_Test.h
 *
 *  $Id$
 *
 *  This code exercises the <ACE_Thread_Timer_Queue_Adapter> using
 *  an <ACE_Timer_Heap_T>.
 *
 *
 *  @author Carlos O'Ryan <coryan@cs.wustl.edu> and Sergio Flores-Gaitan <sergio@cs.wustl.edu>
 */
//=============================================================================


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
#include "ace/Event_Handler_Handle_Timeout_Upcall.h"
#include "Driver.h"

// These typedefs ensure that we use the minimal amount of locking
// necessary.
typedef ACE_Event_Handler_Handle_Timeout_Upcall
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

/**
 * @class Input_Task
 *
 * @brief Read user actions on the Timer_Queue from stdin.
 *
 * This class reads user input from stdin; those commands permit
 * the control of a Timer_Queue, which is dispatched by another
 * thread.
 */
class Input_Task : public ACE_Task_Base
{
public:
  typedef int (Input_Task::*ACTION) (void *);

  Input_Task (Thread_Timer_Queue *queue,
              Thread_Timer_Queue_Test_Driver &timer_queue_driver);

  /// This method runs the event loop in the new thread.
  virtual int svc (void);

  // = Some helper methods.

  /// Add a new timer to expire in <seconds> more.
  int add_timer (void *);

  /// Cancel timer <id>.
  int cancel_timer (void *);

  /// List the current scheduled timers.
  int list_timer (void *);

  /// Shutdown task.
  int shutdown_timer (void *);

  /// Dump the state of the timer queue.
  void dump (void);

private:
  /// The timer queue implementation.
  Thread_Timer_Queue *queue_;

  /// How many micro seconds are in a second.
  const int usecs_;

  /// The thread timer queue test driver.
  Thread_Timer_Queue_Test_Driver &driver_;
};

/**
 * @class Thread_Timer_Queue_Test_Driver
 *
 * @brief Implements an example application that exercises
 * <Thread_Timer_Queue> timer queue.
 *
 * This class implements a simple test driver for the
 * <Thread_Timer_Queue>.  The <display_menu> hook method is
 * called from the base class to print a menu specific to the
 * thread implementation of the timer queue.
 */
class ACE_Svc_Export Thread_Timer_Queue_Test_Driver : public Timer_Queue_Test_Driver <Thread_Timer_Queue, Input_Task, Input_Task::ACTION>
{
public:
  Thread_Timer_Queue_Test_Driver (void);
  ~Thread_Timer_Queue_Test_Driver (void);

  virtual int display_menu (void);
  virtual int init (void);
  virtual int run_test (void);

private:
  /// Subclassed from ACE_Task.
  Input_Task input_task_;
};

/**
 * @class Handler
 *
 * @brief Event handler for the timer queue timeout events.
 *
 * The <handle_timeout> hook method prints out the current time,
 * prints the time when this timer expired and deletes "this".
 */
class Handler : public ACE_Event_Handler
{
public:
  Handler (const ACE_Time_Value &expiration_time);
  ~Handler (void);

  /// Store an "id" for the Handler, which is only use to print better
  /// messages.
  void set_id (int id);

  /// Call back hook.
  virtual int handle_timeout (const ACE_Time_Value &current_time,
                              const void *arg);

private:
  /// Store the expected time of expiration, it is used to print a nice
  /// message saying how much delay was at the actual expiration time.
  ACE_Time_Value expires_;

  /// Store an "id" for the Handler, which is only use to print better
  /// messages.
  int id_;
};

#endif /* _THREAD_TIMER_QUEUE_TEST_H_ */
