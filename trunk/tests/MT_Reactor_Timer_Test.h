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
#include "ace/Task.h"

#define TIMER_SLOTS 10

class Time_Handler : public ACE_Task<ACE_MT_SYNCH>
{
public:
  Time_Handler();

  void   setup(void);

  virtual int svc (void);
  // Run by a daemon thread to handle deferred processing.

  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg);

private:
  long		timer_id[TIMER_SLOTS];
  int		step;
  ACE_Reactor	my_reactor;

};

#endif /* __MT_REACTOR_TIMER_TEST_H */
