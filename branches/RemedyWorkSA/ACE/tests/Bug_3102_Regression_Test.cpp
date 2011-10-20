/**
 * @file Bug_3102_Regression_Test.cpp
 *
 * $Id$
 *
 * Reproduces the problems reported in bug 3102
 *   http://deuce.doc.wustl.edu/bugzilla/show_bug.cgi?id=3102
 */

#include "test_config.h"
#include "ace/ACE.h"
#include "ace/Trace.h"
#include "ace/Thread_Mutex.h"
#include "ace/Managed_Object.h"
#include "ace/Guard_T.h"
#include "ace/OS_NS_stdio.h"

#if defined (ACE_HAS_THREADS)

void mutexTest(void)
{
  ACE_MT(ACE_Thread_Mutex *lock =
    ACE_Managed_Object<ACE_Thread_Mutex>::get_preallocated_object
    (ACE_Object_Manager::ACE_THREAD_EXIT_LOCK));
  ACE_GUARD (ACE_Thread_Mutex, ace_mon, *lock);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Mutex'd!\n")));
}

#endif /* ACE_HAS_THREADS */

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_3102_Regression_Test"));

#if defined (ACE_HAS_THREADS)
  ACE_Trace::start_tracing();

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Hello world!\n")));

  mutexTest();

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Done!\n")));

  ACE_Trace::stop_tracing();
#else
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("threads not supported on this platform\n")));
#endif /* ACE_HAS_THREADS */

  ACE_END_TEST;

  return 0;
}
