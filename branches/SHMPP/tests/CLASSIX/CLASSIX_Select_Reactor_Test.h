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

#ifndef __MT_REACTOR_TIMER_TEST_H
#define __MT_REACTOR_TIMER_TEST_H

#include "ace/Reactor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Task.h"

class Time_Handler : public ACE_Task<ACE_SYNCH>
{
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

#endif /* __MT_REACTOR_TIMER_TEST_H */
