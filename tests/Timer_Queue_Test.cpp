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
//    Douglas C. Schmidt and Prashant Jain
// 
// ============================================================================

#include "ace/Profile_Timer.h"
#include "ace/Timer_List.h"
#include "ace/Timer_Heap.h"
#include "test_config.h"

static void
randomize_array (int array[], size_t size)
{
  ACE_OS::srand (ACE_OS::time (0L));

  // Randomize the array.

  for (size_t i = 0; i < size; i++)
    {
      int index = ACE_OS::rand() % size--;
      int temp = array [index];
      array [index] = array [size];
      array [size] = temp;
    }
}

// Number of iterations for the performance tests.
static int max_iterations =  ACE_DEFAULT_TIMERS * 100 ;

// Keep track of the timer ids that were assigned to us.
static int *timer_ids = 0;

class Example_Handler : public ACE_Event_Handler
{
public:
  Example_Handler (void): close_count_ (0) {}

  virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask mask)
  {
    ACE_ASSERT (mask == ACE_Event_Handler::TIMER_MASK);
    this->close_count_++;
    return 0;
  }

  virtual int handle_timeout (const ACE_Time_Value &,
			      const void *arg)
  {
    ACE_ASSERT ((int) arg == 42 || (int) arg == 007);
    if ((int) arg != 42)
      return -1;
    else
      return 0;
  }

  int close_count_;
  // Keeps track of the number of times that <handle_close> is called.
};

static void
test_functionality (ACE_Timer_Queue *tq)
{
  Example_Handler eh;

  ACE_ASSERT (tq->is_empty ());
  ACE_ASSERT (ACE_Time_Value::zero == ACE_Time_Value (0));
  int timer_id; 
  
  timer_id = tq->schedule (&eh, (const void *) 1, 
			   ACE_OS::gettimeofday ());
  ACE_ASSERT (timer_id != -1);

  ACE_ASSERT (tq->schedule (&eh, (const void *) 42,
			   ACE_OS::gettimeofday ()) != -1);
  ACE_ASSERT (tq->schedule (&eh, (const void *) 42,
			   ACE_OS::gettimeofday ()) != -1);
  // The following method will trigger a call to <handle_close>.
  ACE_ASSERT (tq->cancel (timer_id, 0, 0) == 1);
  ACE_ASSERT (tq->is_empty () == 0);

  ACE_ASSERT (tq->expire () == 2);

  ACE_ASSERT (tq->schedule (&eh, (const void *) 4, 
			    ACE_OS::gettimeofday ()) != -1);
  ACE_ASSERT (tq->schedule (&eh, (const void *) 5, 
			    ACE_OS::gettimeofday ()) != -1);

  // The following method will trigger a call to <handle_close>.
  ACE_ASSERT (tq->cancel (&eh, 0) == 2);
  ACE_ASSERT (tq->is_empty ());
  ACE_ASSERT (tq->expire () == 0);
  ACE_ASSERT (tq->schedule (&eh, (const void *) 007, 
			    ACE_OS::gettimeofday ()) != -1);
  ACE_ASSERT (tq->expire () == 1);

  timer_id = tq->schedule (&eh, (const void *) 6,
			   ACE_OS::gettimeofday ());
  ACE_ASSERT (timer_id != -1);
  ACE_ASSERT (tq->schedule (&eh, (const void *) 7, 
			    ACE_OS::gettimeofday ()) != -1);

  // The following method will *not* trigger a call to <handle_close>.
  ACE_ASSERT (tq->cancel (timer_id) == 1);
  ACE_ASSERT (tq->cancel (&eh) == 1);
  ACE_ASSERT (tq->expire () == 0);
  ACE_ASSERT (eh.close_count_ == 2);
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

  // Test the amount of time required to schedule all the timers.

  timer.start ();

  for (i = 0; i < max_iterations; i++)
    {
      timer_ids[i] = tq->schedule (&eh, 
				   (const void *) 42, 
				   ACE_OS::gettimeofday ());
      ACE_ASSERT (timer_ids[i] != -1);
    }

  ACE_ASSERT (tq->is_empty () == 0);

  timer.stop ();

  ACE_Profile_Timer::ACE_Elapsed_Time et;

  timer.elapsed_time (et);

  ACE_DEBUG ((LM_DEBUG, 
	      "time to schedule %d timers for %s\n", 
	      max_iterations, test_name));
  ACE_DEBUG ((LM_DEBUG, 
	      "real time = %f secs, user time = %f secs, system time = %f secs\n",
	    et.real_time, et.user_time, et.system_time));
  ACE_DEBUG ((LM_DEBUG, 
	      "time per call = %f usecs\n", 
	      (et.user_time / double (max_iterations)) * 1000000));

  // Test the amount of time required to cancel all the timers.  We
  // start from the "back" in order to measure the worst case
  // performance for the <ACE_Timer_List> (which uses linear search).

  timer.start ();

  for (i = max_iterations - 1; i >= 0; i--)
    tq->cancel (timer_ids[i]);

  ACE_ASSERT (tq->is_empty ());

  timer.stop ();

  timer.elapsed_time (et);

  ACE_DEBUG ((LM_DEBUG, 
	      "time to cancel %d timers for %s\n",
	      max_iterations, test_name));
  ACE_DEBUG ((LM_DEBUG, 
	      "real time = %f secs, user time = %f secs, system time = %f secs\n",
	    et.real_time, et.user_time, et.system_time));
  ACE_DEBUG ((LM_DEBUG, 
	      "time per call = %f usecs\n", 
	      (et.user_time / double (max_iterations)) * 1000000));

  // Test the amount of time required to schedule and expire all the
  // timers.

  timer.start ();

  for (i = 0; i < max_iterations; i++)
    ACE_ASSERT (tq->schedule (&eh, 
			      (const void *) 42, 
			      ACE_OS::gettimeofday ()) != -1);

  ACE_ASSERT (tq->is_empty () == 0);

  // Expire all the timers.
  tq->expire ();

  timer.stop ();

  timer.elapsed_time (et);

  ACE_DEBUG ((LM_DEBUG, 
	      "time to schedule and expire %d timers for %s\n", 
	      max_iterations, test_name));
  ACE_DEBUG ((LM_DEBUG, 
	      "real time = %f secs, user time = %f secs, system time = %f secs\n",
	    et.real_time, et.user_time, et.system_time));
  ACE_DEBUG ((LM_DEBUG, 
	      "time per call = %f usecs\n", 
	      (et.user_time / double (max_iterations)) * 1000000));

  // Test the amount of time required to randomly expire all the
  // timers.

  for (i = 0; i < max_iterations; i++)
    {
      timer_ids[i] = tq->schedule (&eh, (const void *) 42,
				   ACE_OS::gettimeofday ()); 
      ACE_ASSERT (timer_ids[i] != -1);
    }

  ACE_ASSERT (tq->is_empty () == 0);

  randomize_array (timer_ids, max_iterations);

  timer.start ();

  for (i = 0; i < max_iterations; i++)
    tq->cancel (timer_ids[i]);

  ACE_ASSERT (tq->is_empty ());

  timer.stop ();

  timer.elapsed_time (et);

  ACE_DEBUG ((LM_DEBUG, "time to randomly expire %d timers for %s\n", 
       max_iterations, test_name));
  ACE_DEBUG ((LM_DEBUG, "real time = %f secs, user time = %f secs, system time = %f secs\n",
     et.real_time, et.user_time, et.system_time));
  ACE_DEBUG ((LM_DEBUG, "time per call = %f usecs\n", 
       (et.user_time / double (max_iterations)) * 1000000));
}

struct Timer_Queues
{
  ACE_Timer_Queue *queue_;
  // Pointer to the subclass of <ACE_Timer_Queue> that we're testing.
  
  const char *name_;
  // Name of the Queue that we're testing.
};

// New Timer_Queue implementations should be added to the end of this
// table.

static Timer_Queues timer_queues[] =
{
  { 0, "ACE_Timer_Heap (preallocated)" },
  { 0, "ACE_Timer_Heap (non-preallocated)" },
  { new ACE_Timer_List, "ACE_Timer_List" },
  { 0, 0 },
};

int
main (int argc, char *argv[])
{
  ACE_START_TEST ("Timer_Queue_Test");

  if (argc > 1)
    max_iterations = ACE_OS::atoi (argv[1]);

  // Preallocate memory.
  ACE_NEW_RETURN (timer_queues[0].queue_,
		  ACE_Timer_Heap (ACE_DEFAULT_TIMERS, 1),
//		  ACE_Timer_Heap (max_iterations, 1),
		  -1);

  // Don't preallocate memory.
  ACE_NEW_RETURN (timer_queues[1].queue_,
		  ACE_Timer_Heap,
//		  ACE_Timer_Heap (max_iterations),
		  -1);

  ACE_NEW_RETURN (timer_ids,
		  int[max_iterations],
		  -1);

  for (int i = 0; timer_queues[i].name_ != 0; i++)
    {
      ACE_DEBUG ((LM_DEBUG, "**** starting test of %s\n", 
		  timer_queues[i].name_));
      test_functionality (timer_queues[i].queue_);
      test_performance (timer_queues[i].queue_, 
			timer_queues[i].name_);
      delete timer_queues[i].queue_;
    }

  delete [] timer_ids;

  ACE_END_TEST;
  return 0;
}
