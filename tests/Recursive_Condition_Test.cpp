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
//      This test program verifies the functionality of the
//      ACE_Condition<ACE_Recursive_Thread_Mutex> template
//      specialization when combined with the
//      ACE_Thread_Timer_Queue_Adapter on Win32 and Posix pthreads.
//
// = AUTHOR
//    Stephen Howard <stephen.e.howard@lmco.com> and
//    Douglas C. Schmidt <schmidt@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/Event_Handler.h"
#include "ace/Synch.h"
#include "ace/Trace.h"
#include "ace/Thread_Manager.h"
#include "ace/Timer_Heap.h"
#include "ace/Timer_Queue_Adapters.h"

ACE_RCSID(tests, Recursive_Condition_Test, "$Id$")

#if defined (ACE_HAS_THREADS)

  typedef ACE_Thread_Timer_Queue_Adapter<ACE_Timer_Heap> Thread_Timer_Queue;

class Test_Handler : public ACE_Event_Handler
{
public:
  virtual int handle_timeout (const ACE_Time_Value &,
                              const void * /*arg*/)
  {
    ACE_TRACE ("Test_Handler::handle_timeout");

    /*Thread_Timer_Queue *timer_queue =
      (Thread_Timer_Queue *) arg;*/

    ACE_Time_Value timeout = ACE_OS::gettimeofday () + ACE_Time_Value (1, 0);

    ACE_UNUSED_ARG (timeout);

    ACE_DEBUG ((LM_DEBUG,
               ACE_TEXT ("(%P|%t) scheduling timer\n")));

    /*
      int timer_id =
      timer_queue->schedule (this, timer_queue, timeout);
    */
    return 0;
  }
};

// These are for the second test - simple wait/signal.
ACE_SYNCH_RECURSIVE_MUTEX mutex_;
ACE_SYNCH_RECURSIVE_CONDITION condition_(mutex_);

ACE_THR_FUNC_RETURN waiter (void *) {
  ACE_ASSERT (mutex_.acquire () == 0);
  ACE_TRACE (ACE_TEXT ("(%t) waiting for cv signal...\n"));
  condition_.wait();
  ACE_TRACE (ACE_TEXT ("(%t) woken up!!!\n"));
  mutex_.release ();
  return 0;
}


int
test_1(void)
{
  if (ACE_Thread_Manager::instance()->spawn (waiter) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("spawn")), 1);
    }

  ACE_OS::sleep (2);
  if (mutex_.acquire () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                         ACE_TEXT ("mutex acquire")),
                        1);
    }
  ACE_TRACE (ACE_TEXT ("(%t) signaling condition...\n"));

  if (condition_.signal () == -1)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("signal")));
    }

  mutex_.release ();
  ACE_Thread_Manager::instance ()->wait ();
  return 0;
}

int
test_2(void)
{
  if (ACE_Thread_Manager::instance()->spawn (waiter) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("spawn")), 1);
    }

  ACE_OS::sleep (2);
  if (mutex_.acquire () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                         ACE_TEXT ("mutex acquire")),
                        1);
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) signaling condition...\n")));

  if (condition_.signal () == -1)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("signal")));
    }

  ACE_OS::sleep(2);
  mutex_.release ();
  ACE_Thread_Manager::instance ()->wait ();
  return 0;
}

int
test_3()
{
  if (ACE_Thread_Manager::instance()->spawn_n (4,waiter) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("spawn")), 1);
    }

  ACE_OS::sleep (2);
  if (mutex_.acquire () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                         ACE_TEXT ("mutex acquire")),
                      1);
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) signaling condition...\n")));

  if (condition_.broadcast () == -1)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("signal")));
    }
  mutex_.release ();
  ACE_Thread_Manager::instance ()->wait ();

  return 0;
}

int
test_4()
{
  if (ACE_Thread_Manager::instance()->spawn (waiter) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("spawn")), 1);
    }

  ACE_OS::sleep (2);
  for(int i = 0; i < 3; ++i){
    if (mutex_.acquire () == -1)
      {
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                           ACE_TEXT ("mutex acquire")),
                          1);
      }
  }

  if (mutex_.get_nesting_level() != 3)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT("%d\n"),
                         ACE_TEXT("get_nestling_level")),
                        1);
    }

  ACE_TRACE (ACE_TEXT ("(%t) signaling condition...\n"));
  if (condition_.signal () == -1)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("signal")));
    }

  for(int i = 0; i < 3; ++i){
     mutex_.release ();
  }

  if (mutex_.get_nesting_level() != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT("%d\n"),
                         ACE_TEXT("get_nestling_level")),
                        1);
    }
  ACE_Thread_Manager::instance ()->wait ();
  return 0;
}
#endif /* ACE_HAS_THREADS */

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Recursive_Condition_Test"));

#if defined (ACE_HAS_THREADS)
#if 0
  Thread_Timer_Queue timer_queue;
  Test_Handler handler;
  int status = timer_queue.activate ();

  ACE_ASSERT (status == 0);

  ACE_Time_Value timeout =
    ACE_OS::gettimeofday() + ACE_Time_Value (1, 0);

  int timer_id = timer_queue.schedule (&handler, &timer_queue, timeout);

  ACE_OS::sleep (10);
  timer_queue.deactivate ();
  timer_queue.wait ();
#endif

  /* Test 1 - Simple test */
  if (test_1 () != 0)
    {
      ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT("test #1")), 1);
    }

  /* Test #2 - Sleep 2 seconds before releasing mutex */
  if (test_2 () != 0)
    {
      ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT("test #2")), 1);
    }

  /* Test #3 - One main thread - 4 subthreads */
  if (test_3 () != 0)
    {
      ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT("test #3")), 1);
    }

  /* Test #4 - Multiple calls to mutex_.acquire and mutex_.release */
  if (test_4 () != 0)
    {
      ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT("test #4")), 1);
    }

#else
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("ACE doesn't support recursive condition variables on this platform\n")));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Thread_Timer_Queue_Adapter<ACE_Timer_Heap>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Thread_Timer_Queue_Adapter<ACE_Timer_Heap>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
