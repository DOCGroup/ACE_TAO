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
#include "ace/Timer_Heap.h"
#include "ace/Timer_Queue_Adapters.h"

ACE_RCSID(tests, Recursive_Condition_Test, "$Id$")

#if defined (ACE_HAS_THREADS)

typedef ACE_Thread_Timer_Queue_Adapter<ACE_Timer_Heap>
        Thread_Timer_Queue;

class Test_Handler : public ACE_Event_Handler
{
public:
  virtual int handle_timeout (const ACE_Time_Value &,
                              const void *arg)
  {
    ACE_DEBUG ((LM_DEBUG, "[%t] handle_timeout\n"));

    Thread_Timer_Queue *timer_queue = (Thread_Timer_Queue *) arg;

    ACE_Time_Value timeout = ACE_OS::gettimeofday () + ACE_Time_Value (1, 0);

    ACE_DEBUG ((LM_DEBUG, "[%t] scheduling timer\n"));

    int timer_id =
      timer_queue->schedule (this, timer_queue, timeout);
    ACE_DEBUG ((LM_DEBUG, "[%t] timer id = %d\n", timer_id));

    return 0;
  }
};

#endif /* ACE_HAS_THREADS */

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Recursive_Condition_Test"));

#if defined (ACE_HAS_THREADS)
  Thread_Timer_Queue timer_queue;
  Test_Handler handler;
  int status = timer_queue.activate ();

  ACE_ASSERT (status == 0);

  ACE_Time_Value timeout =
    ACE_OS::gettimeofday() + ACE_Time_Value (1, 0);

  ACE_DEBUG ((LM_DEBUG, "[%t] scheduling timer\n"));

  int timer_id = timer_queue.schedule (&handler, &timer_queue, timeout);
  ACE_DEBUG ((LM_DEBUG, "[%t] timer id = %d\n", timer_id));

  ACE_OS::sleep (10);

#else
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("ACE doesn't support recursive condition variables on this platform\n")));
#endif /* ACE_WIN32 */
  ACE_END_TEST;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Thread_Timer_Queue_Adapter<ACE_Timer_Heap>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Thread_Timer_Queue_Adapter<ACE_Timer_Heap>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

