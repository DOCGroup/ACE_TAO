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
//    Steve Huston
// 
// ============================================================================

#if !defined (__MT_REACTOR_TIMER_TEST_H)
#define __MT_REACTOR_TIMER_TEST_H

#include "ace/Reactor.h"
#include "ace/Task.h"
#include "ace/Pipe.h"

class Time_Handler : public ACE_Task<ACE_SYNCH>
{
  // = TITLE
  //   Test out the multi-threading features of the Reactor's timer
  //   mechanism. 
public:
  Time_Handler (void);

  void setup (void);

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
  int step_;
  ACE_Reactor my_reactor_;
};

class Dispatch_Count_Handler : public ACE_Event_Handler
{
  // = TITLE
  //   A simple test to ensure that the Reactor counts the number of
  //   dispatches correctly.
public:
  Dispatch_Count_Handler (void);

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
