/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Thread_Timer_Queue_Custom_Handler_Test.h
 *
 *  $Id$
 *
 *  This code exercises the <ACE_Thread_Timer_Queue_Adapter> using
 *  an <ACE_Timer_Heap_T>. It also demonstrates using a custom handler for
 *  timer events.
 *
 *
 *  @author Carlos O'Ryan <coryan@cs.wustl.edu> and Sergio Flores-Gaitan <sergio@cs.wustl.edu> and Alon Diamant <diamant.alon@gmail.com>
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

/**
 * @class Custom_Handler_Input_Task
 *
 * @brief Read user actions on the Timer_Queue from stdin.
 *
 * This class reads user input from stdin; those commands permit
 * the control of a Timer_Queue, which is dispatched by another
 * thread.
 */
class Custom_Handler_Input_Task : public ACE_Task_Base
{
public:
  typedef int (Custom_Handler_Input_Task::*ACTION) (void *);

  Custom_Handler_Input_Task (Thread_Timer_Queue *queue,
              Thread_Timer_Queue_Custom_Handler_Test &timer_queue_driver);

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
  Thread_Timer_Queue_Custom_Handler_Test &driver_;
};

/**
 * @class Thread_Timer_Queue_Custom_Handler_Test
 *
 * @brief Implements an example application that exercises
 * <Thread_Timer_Queue> timer queue.
 *
 * This class implements a simple test driver for the
 * <Thread_Timer_Queue>.  The <display_menu> hook method is
 * called from the base class to print a menu specific to the
 * thread implementation of the timer queue.
 */
class ACE_Svc_Export Thread_Timer_Queue_Custom_Handler_Test : public Timer_Queue_Test_Driver <Thread_Timer_Queue, Custom_Handler_Input_Task, Custom_Handler_Input_Task::ACTION>
{
public:
  Thread_Timer_Queue_Custom_Handler_Test (void);
  ~Thread_Timer_Queue_Custom_Handler_Test (void);

  virtual int display_menu (void);
  virtual int init (void);
  virtual int run_test (void);

private:
  /// Subclassed from ACE_Task.
  Custom_Handler_Input_Task input_task_;
};

#endif /* _THREAD_TIMER_QUEUE_TEST_H_ */
