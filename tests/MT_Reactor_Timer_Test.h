/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    MT_Reactor_Timer_Test.h
//
// = DESCRIPTION
//      This file contains class definitions needed for template
//      instantiation in the MT_Reactor_Timer_Test.cpp file.
//
// = AUTHOR
//    Steve Huston <shuston@riverace.com>
//
// ============================================================================

#ifndef __MT_REACTOR_TIMER_TEST_H
#define __MT_REACTOR_TIMER_TEST_H

#include "ace/Reactor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Task.h"
#include "ace/Pipe.h"
#include "ace/Synch.h"

class Time_Handler : public ACE_Task<ACE_SYNCH>
{
  // = TITLE
  //   Test out the multi-threading features of the Reactor's timer
  //   mechanism.
public:
  Time_Handler (void);

  void setup (void);

  int verify_results(void);

  virtual int svc (void);
  // Run by a daemon thread to handle deferred processing.

  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg);

private:
  enum
  {
    TIMER_SLOTS = 10
  };

  long timer_id_[TIMER_SLOTS];
  // The timer_id_ array holds timer IDs.  They also have some other values
  // that are specific to this test:
  //    -1  the timer has not been set
  //    -2  the timer was set, but has been cancelled
  //    -3  the timer was set, and it already fired
  enum { TIMER_NOTSET = -1, TIMER_CANCELLED = -2, TIMER_FIRED = -3 };

  long prev_timer_;

#if defined ACE_HAS_THREADS
  ACE_Thread_Mutex  lock_;
#endif /* ACE_HAS_THREADS */

};

class Dispatch_Count_Handler : public ACE_Event_Handler
{
  // = TITLE
  //   A simple test to ensure that the Reactor counts the number of
  //   dispatches correctly.
public:
  Dispatch_Count_Handler (void);

  int handle_close (ACE_HANDLE h,
                    ACE_Reactor_Mask m);
  // Clean up resources from the Reactor.

  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg);
  // Keep track of the number of timeouts.

  virtual int handle_input (ACE_HANDLE);
  // Keep track of the number of I/O events.

  virtual int handle_exception (ACE_HANDLE);
  // Keep track of the number of notifies.

private:
  ACE_Pipe pipe_;
  // Provide something to trigger I/O.
};

#endif /* __MT_REACTOR_TIMER_TEST_H */
