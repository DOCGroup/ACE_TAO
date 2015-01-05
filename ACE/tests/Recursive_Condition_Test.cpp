// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Recursive_Condition_Test.cpp
//
// = DESCRIPTION
//      This test program validates the functionality of the
//      ACE_Condition<ACE_Recursive_Thread_Mutex> template
//      specialization.
//
// = AUTHOR
//    Stephen Howard <stephen.e.howard@lmco.com> and
//    Douglas C. Schmidt <schmidt@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/Event_Handler.h"
#include "ace/Log_Msg.h"
#include "ace/Thread_Manager.h"
#include "ace/Timer_Heap.h"
#include "ace/Timer_Queue_Adapters.h"



#if defined (ACE_HAS_THREADS)

typedef ACE_Thread_Timer_Queue_Adapter<ACE_Timer_Heap> Thread_Timer_Queue;

class Test_Handler : public ACE_Event_Handler
{
public:
  Test_Handler (void) : nr_expirations_ (0) {}
  int nr_expirations (void) { return this->nr_expirations_; }

  virtual int handle_timeout (const ACE_Time_Value &,
                              const void *arg)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Test_Handler::handle_timeout\n")));
    ++this->nr_expirations_;

    void *nc_arg = const_cast<void *> (arg);
    Thread_Timer_Queue *timer_queue =
      reinterpret_cast<Thread_Timer_Queue *> (nc_arg);

    ACE_Time_Value timeout = ACE_OS::gettimeofday () + ACE_Time_Value (1, 0);

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) scheduling new timer 1 sec from now\n")));
    if (timer_queue->schedule (this, timer_queue, timeout) == -1)
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%t) %p\n"),
                  ACE_TEXT ("schedule failed")));

    return 0;
  }

private:
  int nr_expirations_;
};

// These are for the basic functionality tests.
ACE_SYNCH_RECURSIVE_MUTEX mutex_;
ACE_Condition<ACE_SYNCH_RECURSIVE_MUTEX> condition_ (mutex_);
// Test driver sets this to non-zero before spawning and to zero for
// waiter.
int protected_int = 0;

static ACE_THR_FUNC_RETURN
waiter (void *)
{
  if (mutex_.acquire () != 0)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("(%t) %p\n"),
                       ACE_TEXT ("acquire")), 0);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) waiting for cv signal...\n")));
  if (condition_.wait () == 0)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) woken up!!!\n")));
  else
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%t) %p\n"), ACE_TEXT ("wait")));

  int const copy_int = protected_int;  // Copy it in case it's erroneously changing
  if (copy_int != 0)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%t) waiter found protected_int %d\n"),
                copy_int));

  if (mutex_.release () != 0)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%t) %p\n"), ACE_TEXT ("release")));

  return 0;
}

static int
test_1 (void)
{
  protected_int = 1;
  if (ACE_Thread_Manager::instance()->spawn (waiter) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("(%t) %p\n"),
                       ACE_TEXT ("test 1 spawn")),
                      1);

  ACE_OS::sleep (2);
  if (mutex_.acquire () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("(%t) %p\n"),
                       ACE_TEXT ("test 1 mutex acquire")),
                      1);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) signaling condition...\n")));
  protected_int = 0;
  if (condition_.signal () == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%t) %p\n"),
                ACE_TEXT ("test 1 signal")));

  if (mutex_.release () == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%t) %p\n"),
                ACE_TEXT ("test 1 release")));

  ACE_Thread_Manager::instance ()->wait ();
  return 0;
}

static int
test_2 (void)
{
  protected_int = 1;
  if (ACE_Thread_Manager::instance()->spawn (waiter) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("(%t) %p\n"),
                       ACE_TEXT ("test 2 spawn")),
                      1);

  ACE_OS::sleep (2);
  if (mutex_.acquire () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("(%t) %p\n"),
                       ACE_TEXT ("test 2 mutex acquire")),
                      1);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) signaling condition...\n")));
  if (condition_.signal () == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("test 2 signal")));

  // Wait to clear protected_int to be sure cv properly reacquires the
  // mutex before returning control to caller.
  ACE_OS::sleep(2);
  protected_int = 0;
  if (mutex_.release () == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%t) %p\n"),
                ACE_TEXT ("test 2 release")));

  ACE_Thread_Manager::instance ()->wait ();
  return 0;
}

static int
test_3 (void)
{
  protected_int = 1;
  if (ACE_Thread_Manager::instance()->spawn_n (4, waiter) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                       ACE_TEXT ("test 3 spawn")), 1);

  ACE_OS::sleep (2);
  if (mutex_.acquire () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("(%t) %p\n"),
                       ACE_TEXT ("test 3 mutex acquire")),
                      1);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) broadcasting condition...\n")));
  if (condition_.broadcast () == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%t) %p\n"),
                ACE_TEXT ("test 3 broadcast")));
  protected_int = 0;
  if (mutex_.release () == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%t) %p\n"),
                ACE_TEXT ("test 3 release")));

  ACE_Thread_Manager::instance ()->wait ();

  return 0;
}

static int
test_4 (void)
{
  const int recurse_count = 3;

  protected_int = recurse_count;
  if (ACE_Thread_Manager::instance()->spawn (waiter) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("(%t) %p\n"),
                       ACE_TEXT ("spawn")), 1);

  ACE_OS::sleep (2);
  int i;
  for (i = 0; i < recurse_count; ++i)
    if (mutex_.acquire () == -1)
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("pass %d, %p\n"),
                         i + 1,
                         ACE_TEXT ("recursive acquire")),
                        1);

  // Don't report a failure if the mutex doesn't offer a view of the
  // recursion count.
  int nesting_level = mutex_.get_nesting_level ();
  if (!(nesting_level == -1 && errno == ENOTSUP) && nesting_level != i)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT("test 4 nesting level %d;")
                       ACE_TEXT (" should be %d\n"),
                       nesting_level, i),
                      1);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) signaling condition...\n")));
  if (condition_.signal () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("(%t) %p\n"),
                       ACE_TEXT ("test 4 signal")),
                      1);

  for (i = 0; i < recurse_count; ++i)
    {
      // Only decrement - be sure all the waiting threads are not released
      // before we release the mutex the correct number of times.
      --protected_int;
      mutex_.release ();
    }

  // The waiter thread will acquire the mutex as a result of the releases
  // above... don't check the nesting level until waiter() has had a chance
  // to wake up, acquire, and release the mutex.
  ACE_Thread_Manager::instance ()->wait ();

  nesting_level = mutex_.get_nesting_level ();
  if (!(nesting_level == -1 && errno == ENOTSUP) && nesting_level != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT("(%t) nesting level %d; should be 0\n"),
                       nesting_level),
                      1);

  return 0;
}
#endif /* ACE_HAS_THREADS */

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Recursive_Condition_Test"));

#if defined (ACE_HAS_THREADS)
  int status = 0;

  /* Test 1 - Simple test */
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Test 1...\n")));
  if (test_1 () != 0)
    ++status;

  /* Test #2 - Sleep 2 seconds before releasing mutex */
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Test 2...\n")));
  if (test_2 () != 0)
    ++status;

  /* Test #3 - One main thread - 4 subthreads */
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Test 3...\n")));
  if (test_3 () != 0)
    ++status;

  /* Test #4 - Multiple calls to mutex_.acquire and mutex_.release */
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Test 4...\n")));
  if (test_4 () != 0)
    ++status;

  // Timer queue usage.
  Thread_Timer_Queue timer_queue;
  Test_Handler handler;
  if (0 != timer_queue.activate ())
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%t) %p\n"), ACE_TEXT ("activate")));
      ++status;
    }

  ACE_Time_Value timeout =
    ACE_OS::gettimeofday() + ACE_Time_Value (1, 0);

  if (-1 == timer_queue.schedule (&handler, &timer_queue, timeout))
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%t) %p\n"), ACE_TEXT ("schedule")));
      ++status;
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Starting to sleep\n")));
  ACE_OS::sleep (10);
  timer_queue.deactivate ();
  timer_queue.wait ();
  // Scheduling every second, waiting 10 seconds, should get at least 9
  int expirations = handler.nr_expirations ();
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Caught %d timer expirations\n"),
              expirations));
  if (expirations < 9)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Should have caught at least 9\n")));

#else
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("ACE doesn't support recursive condition variables on this platform\n")));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return 0;
}

