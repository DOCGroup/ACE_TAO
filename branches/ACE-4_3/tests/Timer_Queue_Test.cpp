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
//      This is a simple test of <ACE_Timer_Queue> and four of its
//      subclasses (<ACE_Timer_List>, <ACE_Timer_Heap>, <ACE_Timer_Wheel>, and 
//      <ACE_Timer_Hash>).  The test sets up a bunch of timers and then adds 
//      them to a timer queue. The functionality of the timer queue is then 
//      tested. No command line arguments are needed to run the test.
//
// = AUTHORS
//    Douglas C. Schmidt, Prashant Jain, and Darrell Brunsch
// 
// ============================================================================

#include "ace/Profile_Timer.h"
#include "ace/Timer_Queue.h"
#include "ace/Timer_List.h"
#include "ace/Timer_Heap.h"
#include "ace/Timer_Wheel.h"
#include "ace/Timer_Hash.h"
#include "ace/Timer_Queue.h"
#include "test_config.h"

template <class T> void
randomize_array (T array[], size_t size)
{
  size_t i;

  // Randomize the array.

  for (i = 0; i < size; i++)
    {
      int index = ACE_OS::rand() % size--;
      T temp = array [index];
      array [index] = array [size];
      array [size] = temp;
    }
}

// Number of iterations for the performance tests.
static int max_iterations = ACE_DEFAULT_TIMERS * 100;

// Keep track of the timer ids that were assigned to us.
static long *timer_ids = 0;

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
    ACE_ASSERT (arg == (const void *) 42 || arg == (const void *)007);
    if (arg != (const void *)42)
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
  long timer_id; 
  
  timer_id = tq->schedule (&eh, (const void *) 1, 
			   tq->gettimeofday ());
  ACE_ASSERT (timer_id != -1);
  ACE_ASSERT (tq->is_empty () == 0); //==

  ACE_ASSERT (tq->schedule (&eh, (const void *) 42,
			   tq->gettimeofday ()) != -1);
  ACE_ASSERT (tq->is_empty () == 0); //==
  ACE_ASSERT (tq->schedule (&eh, (const void *) 42,
			   tq->gettimeofday ()) != -1);
  ACE_ASSERT (tq->is_empty () == 0); //==
  // The following method will trigger a call to <handle_close>.
  ACE_ASSERT (tq->cancel (timer_id, 0, 0) == 1);
  ACE_ASSERT (tq->is_empty () == 0);

  ACE_ASSERT (tq->expire () == 2);

  ACE_ASSERT (tq->schedule (&eh, (const void *) 007, 
			    tq->gettimeofday ()) != -1);
  ACE_ASSERT (tq->schedule (&eh, (const void *) 42,
			   tq->gettimeofday () + ACE_Time_Value (100)) != -1);
  ACE_ASSERT (tq->schedule (&eh, (const void *) 42,
			   tq->gettimeofday () + ACE_Time_Value (100)) != -1);

  // The following will trigger a call to <handle_close> when 
  // it cancels the second timer.  This happens because the first
  // timer has an <act> of 007, which causes eh.handle_timeout () to 
  // return -1.  Since -1 is returned, all timers that use <eh> will
  // be cancelled (and <handle_close> will only be called on the first 
  // timer that is cancelled).  
  ACE_ASSERT (tq->expire () == 1);
  ACE_ASSERT (tq->is_empty () != 0);
  
  ACE_ASSERT (tq->schedule (&eh, (const void *) 4, 
			    tq->gettimeofday ()) != -1);
  ACE_ASSERT (tq->schedule (&eh, (const void *) 5, 
			    tq->gettimeofday ()) != -1);
  
  // The following method will trigger a call to <handle_close>.
  ACE_ASSERT (tq->cancel (&eh, 0) == 2);
  ACE_ASSERT (tq->is_empty ());
  ACE_ASSERT (tq->expire () == 0);

  // This tests to make sure that <handle_close> is called when there
  // is only one timer of the type in the queue
  ACE_ASSERT (tq->schedule (&eh, (const void *) 007, 
			    tq->gettimeofday ()) != -1);
  ACE_ASSERT (tq->expire () == 1);

  timer_id = tq->schedule (&eh, (const void *) 6,
			   tq->gettimeofday ());
  ACE_ASSERT (timer_id != -1);
  ACE_ASSERT (tq->schedule (&eh, (const void *) 7, 
			    tq->gettimeofday ()) != -1);

  // The following method will *not* trigger a call to <handle_close>.
  ACE_ASSERT (tq->cancel (timer_id) == 1);
  ACE_ASSERT (tq->cancel (&eh) == 1);
  ACE_ASSERT (tq->expire () == 0);
  ACE_ASSERT (eh.close_count_ == 4);
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

  ACE_Time_Value *times;
  ACE_NEW (times, ACE_Time_Value[max_iterations]);

  for (i = 0; i < max_iterations; i++)
    times[i] = tq->gettimeofday ();

  timer.start ();

  for (i = 0; i < max_iterations; i++)
    {
      timer_ids[i] = tq->schedule (&eh, 
				   (const void *) 42, 
				   times[i]);
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

  // Test the amount of time required to cancel all the timers.

  timer.start ();

  for (i = max_iterations - 1; i >= 0; i--)
    tq->cancel (timer_ids[i]);

  timer.stop ();

  ACE_ASSERT (tq->is_empty ());

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
			      tq->gettimeofday ()) != -1);

  ACE_ASSERT (tq->is_empty () == 0);

  // Expire all the timers.
  tq->expire ();

  timer.stop ();

  if (!tq->is_empty ())
  {
    ACE_OS::sleep (ACE_Time_Value (1));
    tq->expire ();
  }

  ACE_ASSERT (tq->is_empty ());

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

  // Test the amount of time required to randomly cancel all the
  // timers.

  for (i = 0; i < max_iterations; i++)
    {
      timer_ids[i] = tq->schedule (&eh, 
                                   (const void *) 42,
				   tq->gettimeofday ()); 
      ACE_ASSERT (timer_ids[i] != -1);
    }

  ACE_ASSERT (tq->is_empty () == 0);

  timer.start ();

  for (i = max_iterations - 1; i >= 0; i--)
    tq->cancel (timer_ids[i]);

  if (!tq->is_empty ())
  {
    ACE_OS::sleep (ACE_Time_Value (1));
    tq->expire ();
  }

  ACE_ASSERT (tq->is_empty ());

  timer.stop ();

  timer.elapsed_time (et);

  ACE_DEBUG ((LM_DEBUG, 
             "time to randomly cancel %d timers for %s\n", 
             max_iterations, test_name));
  ACE_DEBUG ((LM_DEBUG, 
             "real time = %f secs, user time = %f secs, system time = %f secs\n",
             et.real_time, et.user_time, et.system_time));
  ACE_DEBUG ((LM_DEBUG, 
             "time per call = %f usecs\n", 
             (et.user_time / double (max_iterations)) * 1000000));

  // Test the amount of time required to randomly schedule all the timers.

  ACE_Time_Value now = tq->gettimeofday ();
  
  for (i = 0; i < max_iterations; i++)
    times[i] = now - ACE_Time_Value (0, ACE_OS::rand () % 1000000);

  timer.start ();

  for (i = 0; i < max_iterations; i++)
    {
      timer_ids[i] = tq->schedule (&eh, 
				   (const void *) 42, 
				   times[i]);
      ACE_ASSERT (timer_ids[i] != -1);
    }

  timer.stop ();

  ACE_ASSERT (tq->is_empty () == 0);

  timer.elapsed_time (et);

  ACE_DEBUG ((LM_DEBUG, 
	      "time to randomly schedule %d timers for %s\n", 
	      max_iterations, test_name));
  ACE_DEBUG ((LM_DEBUG, 
	      "real time = %f secs, user time = %f secs, system time = %f secs\n",
              et.real_time, et.user_time, et.system_time));
  ACE_DEBUG ((LM_DEBUG, 
	      "time per call = %f usecs\n", 
	      (et.user_time / double (max_iterations)) * 1000000));

  // Test the amount of time required to cancel all the timers.

  timer.start ();

  tq->expire ();

  if (!tq->is_empty ())
  {
    ACE_OS::sleep (ACE_Time_Value (1));
    tq->expire ();
  }

  ACE_ASSERT (tq->is_empty ());

  timer.stop ();

  timer.elapsed_time (et);

  ACE_DEBUG ((LM_DEBUG, 
	      "time to expire %d randomly scheduled timers for %s\n",
	      max_iterations, test_name));
  ACE_DEBUG ((LM_DEBUG, 
	      "real time = %f secs, user time = %f secs, system time = %f secs\n",
	    et.real_time, et.user_time, et.system_time));
  ACE_DEBUG ((LM_DEBUG, 
	      "time per call = %f usecs\n", 
	      (et.user_time / double (max_iterations)) * 1000000));
  
  delete [] times;
}

struct Timer_Queue_List
{
  Timer_Queue_List (ACE_Timer_Queue *queue, const char *name, Timer_Queue_List *next = NULL)
    : queue_ (queue),
      name_ (name),
      next_ (next)
  {}

  ACE_Timer_Queue *queue_;
  // Pointer to the subclass of <ACE_Timer_Queue> that we're testing.
  
  const char *name_;
  // Name of the Queue that we're testing.

  Timer_Queue_List *next_;
  // Pointer to the next <Timer_Queues> structure
};

int
main (int argc, char *argv[])
{
  ACE_START_TEST ("Timer_Queue_Test");

  ACE_OS::srand (ACE_OS::time (0L));
 
  if (argc > 1)
    max_iterations = ACE_OS::atoi (argv[1]);

  // = Perform initializations.
  
  Timer_Queue_List *tq_list = NULL;

  // Add new Timer_Queue implementations here.
 

  // Timer_Hash (Heap)

  ACE_NEW_RETURN (tq_list,
                  Timer_Queue_List (new ACE_Timer_Hash_Heap,
                                    "ACE_Timer_Hash (Heap)",
                                    tq_list),
                  -1);

  // Timer_Hash
  
  ACE_NEW_RETURN (tq_list,
                  Timer_Queue_List (new ACE_Timer_Hash,
                                    "ACE_Timer_Hash",
                                    tq_list),
                  -1);

  // Timer_List

  ACE_NEW_RETURN (tq_list,
                  Timer_Queue_List (new ACE_Timer_List,
                                    "ACE_Timer_List",
                                    tq_list),
                  -1);

  // Timer_Wheel without preallocated memory

  ACE_NEW_RETURN (tq_list,
                  Timer_Queue_List (new ACE_Timer_Wheel,
                                    "ACE_Timer_Wheel (non-preallocated)",
                                    tq_list),
                  -1);

  // Timer_Wheel with preallocated memory.

  ACE_NEW_RETURN (tq_list,
                  Timer_Queue_List (new ACE_Timer_Wheel (ACE_DEFAULT_TIMER_WHEEL_SIZE, 
                                                         ACE_DEFAULT_TIMER_WHEEL_RESOLUTION, 
                                                         max_iterations),
                                    "ACE_Timer_Wheel (preallocated)",
                                    tq_list),
                  -1);



  // Timer_Heap without preallocated memory.

  ACE_NEW_RETURN (tq_list,
                  Timer_Queue_List (new ACE_Timer_Heap,
                                    "ACE_Timer_Heap (non-preallocated)",
                                    tq_list),
                  -1);

  // Timer_Heap with preallocate memory.

  ACE_NEW_RETURN (tq_list,
                  Timer_Queue_List (new ACE_Timer_Heap (max_iterations, 1),
                                    "ACE_Timer_Heap (preallocated)",
                                    tq_list),
                  -1);
  
  // Create the Timer ID array

  ACE_NEW_RETURN (timer_ids,
		  long[max_iterations],
		  -1);
  
  Timer_Queue_List *tq_ptr = tq_list;

  while (tq_ptr != NULL)
    {
      ACE_DEBUG ((LM_DEBUG, "**** starting test of %s\n", 
		  tq_ptr->name_));
      test_functionality (tq_ptr->queue_);
      test_performance (tq_ptr->queue_, 
			tq_ptr->name_);
      delete tq_ptr->queue_;
      Timer_Queue_List *temp = tq_ptr;
      tq_ptr = tq_ptr->next_;
      delete temp;
    }

  delete [] timer_ids;
  
  ACE_END_TEST;
  return 0;
}
