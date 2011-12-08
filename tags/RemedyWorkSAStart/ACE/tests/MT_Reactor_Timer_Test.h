/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    MT_Reactor_Timer_Test.h
 *
 *  $Id$
 *
 *    This file contains class definitions needed for template
 *    instantiation in the MT_Reactor_Timer_Test.cpp file.
 *
 *
 *  @author Steve Huston <shuston@riverace.com>
 */
//=============================================================================


#ifndef ACE_TESTS_MT_REACTOR_TIMER_TEST_H
#define ACE_TESTS_MT_REACTOR_TIMER_TEST_H

#include "ace/Reactor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Task.h"
#include "ace/Pipe.h"

/**
 * @class Time_Handler
 *
 * @brief Test out the multi-threading features of the Reactor's timer
 * mechanism.
 */
class Time_Handler : public ACE_Task<ACE_SYNCH>
{
public:
  Time_Handler (void);

  void setup (void);

  int verify_results(void);

  /// Run by a daemon thread to handle deferred processing.
  virtual int svc (void);

  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg);

private:
  enum
  {
    TIMER_SLOTS = 10
  };

  /**
   * The timer_id_ array holds timer IDs.  They also have some other values
   * that are specific to this test:
   *    -1  the timer has not been set
   *    -2  the timer was set, but has been cancelled
   *    -3  the timer was set, and it already fired
   */
  long timer_id_[TIMER_SLOTS];
  enum { TIMER_NOTSET = -1, TIMER_CANCELLED = -2, TIMER_FIRED = -3 };

  long prev_timer_;

#if defined ACE_HAS_THREADS
  ACE_Thread_Mutex  lock_;
#endif /* ACE_HAS_THREADS */

};

/**
 * @class Dispatch_Count_Handler
 *
 * @brief A simple test to ensure that the Reactor counts the number of
 * dispatches correctly.
 */
class Dispatch_Count_Handler : public ACE_Event_Handler
{
public:
  Dispatch_Count_Handler (void);

  /// Clean up resources from the Reactor.
  int handle_close (ACE_HANDLE h,
                    ACE_Reactor_Mask m);

  /// Keep track of the number of timeouts.
  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg);

  /// Keep track of the number of I/O events.
  virtual int handle_input (ACE_HANDLE);

  /// Keep track of the number of notifies.
  virtual int handle_exception (ACE_HANDLE);

  /// Verify that the expected events did happen.
  int verify_results (void);

private:
  /// Provide something to trigger I/O.
  ACE_Pipe pipe_;

  int input_seen_;
  int notify_seen_;
  size_t timers_fired_;

};

#endif /* ACE_TESTS_MT_REACTOR_TIMER_TEST_H */
