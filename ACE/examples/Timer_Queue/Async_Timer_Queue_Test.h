/* -*- C++ -*- */


//=============================================================================
/**
 *  @file    Async_Timer_Queue_Test.h
 *
 *  $Id$
 *
 *    This test exercises the <ACE_Asynch_Timer_Queue_Adapter>
 *    using an <ACE_Timer_Heap>.
 *
 *
 *  @author Douglas C. Schmidt and Sergio Flores-Gaitan
 */
//=============================================================================


#ifndef _ASYNC_TIMER_QUEUE_TEST_H_
#define _ASYNC_TIMER_QUEUE_TEST_H_

#include "ace/Signal.h"
#include "ace/svc_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Timer_Heap.h"
#include "ace/Timer_Queue_Adapters.h"

#include "Driver.h"

/**
 * @class Async_Timer_Handler
 *
 * @brief Target of the asynchronous timeout operation.
 */
class Async_Timer_Handler : public ACE_Event_Handler
{
public:
  /// Callback hook invoked by the <Timer_Queue>.
  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg);
};

/**
 * @class Async_Timer_Queue
 *
 * @brief Asynchronous Timer Queue Singleton.
 *
 * We use this class to avoid global variables and to
 * consolidate all the Timer Queue processing in one central
 * place.
 */
class Async_Timer_Queue
{
public:
  typedef int (Async_Timer_Queue::*ACTION) (void *);

   /// Singleton access point.
  static Async_Timer_Queue *instance (void);

  /// Schedule a timer to expire <microsecs> in the future.
  void schedule (u_int microsecs);

  /// Cancel a timer with <timer_id>.
  void cancel (long timer_id);

  /// Dump the contents of the queue.
  void dump (void);

  /// hook method to schedule a timer.  Called from
  /// <Timer_Queue_Test_Driver>
  int schedule_timer (void *argument);

  /// hook method to cancel a timer.  Called from
  /// <Timer_Queue_Test_Driver>
  int cancel_timer (void *argument);

  /// hook method to list timers.  Called from
  /// <Timer_Queue_Test_Driver>
  int list_timer (void *argument);

  /// hook method to exit the timer queue.  Called from
  /// <Timer_Queue_Test_Driver>
  int shutdown_timer (void *argument);

private:
  /// Private constructor enforces the Singleton.
  Async_Timer_Queue (ACE_Sig_Set *);

  /// Pointer to the timer queue.
  static Async_Timer_Queue *instance_;

  /// The adapter is instantiated by an <ACE_Timer_Heap>.
  ACE_Async_Timer_Queue_Adapter<ACE_Timer_Heap> tq_;
};

/**
 * @class Async_Timer_Queue_Test_Driver
 *
 * @brief Async_Timer_Queue_Test_Driver
 *
 * This class implements a test driver for the
 * <Async_Timer_Queue>.  Implements a display_menu() method that
 * prints the options for a user. and init() which initializes
 * the driver.  The rest of the common functionality is in the
 * parent class <Timer_Queue_Test_Driver>.
 */
class ACE_Svc_Export Async_Timer_Queue_Test_Driver : public Timer_Queue_Test_Driver <Async_Timer_Queue *, Async_Timer_Queue, Async_Timer_Queue::ACTION>
{
public:
  Async_Timer_Queue_Test_Driver (void);

  /// Print menu of options.
  virtual int display_menu (void);

  /// Initializes the driver's internal variables inherited from the parent
  virtual int init (void);
};

#endif /* _ASYNC_TIMER_QUEUE_TEST_H_ */
