// ============================================================================
// @(#)Timer_Queue_Test.cpp	1.1	10/18/96

//
// = LIBRARY
//    tests
// 
// = FILENAME
//    Timer_Queue_Test.cpp
//
// = DESCRIPTION
//      This is a simple test of ACE_Timer_Queue. The test sets up a
//      bunch of timers and then adds them to a timer queue. The
//      functionality of the timer queue is then tested. No command line
//      arguments are needed to run the test.
//
// = AUTHOR
//    Prashant Jain
// 
// ============================================================================

#include "ace/Log_Msg.h"
#include "ace/Timer_Queue.h"
#include "test_config.h"

class Example_Handler : public ACE_Event_Handler
{
public:
  virtual int handle_timeout (const ACE_Time_Value &tv, const void *arg)
  {
    ACE_ASSERT ((int) arg == 42);
    return 0;
  }
};

int
main (int argc, char *argv[])
{
  ACE_START_TEST;

  ACE_Timer_Queue tq;
  Example_Handler eh;

  ACE_ASSERT (tq.is_empty ());
  ACE_ASSERT (ACE_Time_Value::zero == ACE_Time_Value (0));
  int timer_id; 
  
  timer_id = tq.schedule (&eh, (const void *) 1, ACE_OS::gettimeofday ());
  tq.schedule (&eh, (const void *) 42, ACE_OS::gettimeofday ());
  tq.schedule (&eh, (const void *) 42, ACE_OS::gettimeofday ());
  tq.cancel (timer_id);
  ACE_ASSERT (!tq.is_empty ());

  tq.expire (ACE_OS::gettimeofday ());

  tq.schedule (&eh, (const void *) 4, ACE_OS::gettimeofday ());
  tq.schedule (&eh, (const void *) 5, ACE_OS::gettimeofday ());
  tq.cancel (&eh);
  ACE_ASSERT (tq.is_empty ());
  tq.expire (ACE_OS::gettimeofday ());

  ACE_END_TEST;
  return 0;
}

