// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
// 
// = FILENAME
//    Timer_Queue_Test.cpp
//
// = DESCRIPTION
//      This is a simple test of <ACE_Timer_Queue> and two of its
//      subclasses (<ACE_Timer_List> and <ACE_Timer_Heap>).  The test
//      sets up a bunch of timers and then adds them to a timer
//      queue. The functionality of the timer queue is then tested. No
//      command line arguments are needed to run the test.
//
// = AUTHOR
//    Prashant Jain
// 
// ============================================================================

#include "ace/Timer_List.h"
#include "ace/Timer_Heap.h"
#include "test_config.h"

const int MAX_ITERATIONS = ACE_DEFAULT_MAX_TIMERS - 1;

class Example_Handler : public ACE_Event_Handler
{
public:
  virtual int handle_timeout (const ACE_Time_Value &,
			      const void *arg)
  {
    ACE_ASSERT ((int) arg == 42);
    return 0;
  }
};

static void
test_functionality (ACE_Timer_Queue *tq)
{
  Example_Handler eh;

  ACE_ASSERT (tq.is_empty ());
  ACE_ASSERT (ACE_Time_Value::zero == ACE_Time_Value (0));
  int timer_id; 
  
  timer_id = tq.schedule (&eh, (const void *) 1, ACE_OS::gettimeofday ());
  ACE_ASSERT (timer_id != -1);

  ACE_ASSERT (tq.schedule (&eh, (const void *) 42,
			   ACE_OS::gettimeofday ()) != -1);
  ACE_ASSERT (tq.schedule (&eh, (const void *) 42,
			   ACE_OS::gettimeofday ()) != -1);
  ACE_ASSERT (tq.cancel (timer_id) == 1);
  ACE_ASSERT (!tq.is_empty ());

  ACE_ASSERT (tq.expire (ACE_OS::gettimeofday () == 1));

  ACE_ASSERT (tq.schedule (&eh, (const void *) 4, ACE_OS::gettimeofday
			   ()) != -1);
  ACE_ASSERT (tq.schedule (&eh, (const void *) 5, ACE_OS::gettimeofday
			   ()) != -1);
  ACE_ASSERT (tq.cancel (&eh) == 2);
  ACE_ASSERT (tq.is_empty ());
  ACE_ASSERT (tq.expire (ACE_OS::gettimeofday ()) == 0);
}

static void
test_performance (ACE_Timer_Queue *tq,
		  const char *test_name)
{
  Example_Handler eh;
  ACE_Profile_Timer timer;
  int i;

  ACE_ASSERT (tq->is_empty ());
  ACE_ASSERT (ACE_Time_Value::zero == ACE_Time_Value (0));

  timer.start ();

  // Test the amount of time required to schedule all the timers.
  for (i = 0; i < MAX_ITERATIONS; i++)
    tq->schedule (&eh, (const void *) 42, ACE_OS::gettimeofday ());

  ACE_Profile_Timer::ACE_Elapsed_Time et;

  timer.elapsed_time (et);

  ACE_DEBUG ((LM_DEBUG, "time to schedule %d timers for %s\n", 
	      MAX_ITERATIONS, test_name));
  ACE_DEBUG ((LM_DEBUG, "real time = %f secs, user time = %f secs, system time = %f secs\n",
	    et.real_time, et.user_time, et.system_time));
  ACE_DEBUG ((LM_DEBUG, "time per call = %f usecs\n", 
	      (et.real_time / double (MAX_ITERATIONS)) * 1000000));

  timer.start ();

  int timer_id = tq->schedule (&eh, (const void *) 42,
			       ACE_OS::gettimeofday ());

  ACE_ASSERT (timer_id);

  // Test the amount of time required to cancel all the timers (this
  // takes advantage of the fact that <timer_ids> are always allocated
  // contiguously starting at 0.
  for (i = 0; i < MAX_ITERATIONS + 1; i++)
    tq->cancel (timer_id--);

  ACE_ASSERT (tq->is_empty ());

  timer.elapsed_time (et);

  ACE_DEBUG ((LM_DEBUG, "time to cancel %d timers for %s\n",
	      MAX_ITERATIONS, test_name));
  ACE_DEBUG ((LM_DEBUG, "real time = %f secs, user time = %f secs, system time = %f secs\n",
	    et.real_time, et.user_time, et.system_time));
  ACE_DEBUG ((LM_DEBUG, "time per call = %f usecs\n", 
	      (et.real_time / double (MAX_ITERATIONS)) * 1000000));

}

struct Timer_Queues
{
  ACE_Timer_Queue *queue_;
  // Pointer to the subclass of <ACE_Timer_Queue> that we're testing.
  
  const char *name_;
  // Name of the Queue that we're testing.
};

static Timer_Queues timer_queues[] =
{
  { new ACE_Timer_List, "ACE_Timer_List" },
  { new ACE_Timer_Heap, "ACE_Timer_Queue" },
  { 0, 0 };
}

int
main (int, char *[])
{
  ACE_START_TEST ("Timer_Queue_Test");

  for (int i = 0; timer_queues[i].name_ != 0; i++)
    {
      test_performance (timer_queues[i].queue_, timer_queues[i].name_);
      test_functionality (timer_queues[i].queue_, timer_queues[i].name_);
      delete timer_queues[i].queue_;
    }

  ACE_END_TEST;
  return 0;
}
