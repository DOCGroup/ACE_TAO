// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
// 
// = FILENAME
//    Mutex_Test.cpp
//
// = DESCRIPTION
//      This is a simple test to illustrate the functionality of
//      ACE_Process_Mutex. The test acquires and releases mutexes. No
//      command line arguments are needed to run the test.
//
// = AUTHOR
//    Prashant Jain and Doug Schmidt
// 
// ============================================================================

#include "ace/Service_Config.h"
#include "ace/Thread_Manager.h"
#include "test_config.h"

static void *
test (void *args)
{
  ACE_Process_Mutex *pm = (ACE_Process_Mutex *) args;
#if (defined (ACE_WIN32) || defined (VXWORKS)) && defined (ACE_HAS_THREADS)
  ACE_NEW_THREAD;
#endif /* (defined (ACE_WIN32) || defined (VXWORKS)) && defined (ACE_HAS_THREADS) */

  ACE_OS::srand (ACE_OS::time (0));

  for (int i = 0; i < ACE_MAX_ITERATIONS; i++)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) = trying to acquire on iteration %d\n", i));
      ACE_ASSERT (pm->acquire () == 0);
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) = acquired on iteration %d\n", i));

      // Sleep for a random amount of time between 0 and 5 seconds.
      // Note that it's ok to use rand() here because we are running
      // within the critical section defined by the Process_Mutex.
      ACE_OS::sleep (ACE_OS::rand () % 5);

      ACE_ASSERT (pm->release () == 0);
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) = released on iteration %d\n", i));
    }

  return 0;
}

static void
spawn (void)
{
  char *name = "hello";

#if !defined (ACE_WIN32) && !defined (VXWORKS)
  switch (ACE_OS::fork ("child"))
    {
    case -1:
      ACE_ERROR ((LM_ERROR, "%p\n%a", "fork failed"));
      ACE_OS::_exit (-1);
    case 0: // In child
      {
	ACE_Process_Mutex pm (ACE_WIDE_STRING (name));
	test (&pm);
	break;
      }
    default: // In parent
      {
	ACE_Process_Mutex pm (name);
	test (&pm);

	// Allow the client to exit, then remove the Process_Mutex.
	ACE_OS::wait ();
	pm.remove ();
	break;
      }
    }
#elif defined (ACE_HAS_THREADS)
  ACE_Process_Mutex pm (ACE_WIDE_STRING (name));

  if (ACE_Thread_Manager::instance ()->spawn (ACE_THR_FUNC (test),
					     (void *) &pm,
					     THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n%a", "thread create failed"));

  if (ACE_Thread_Manager::instance ()->spawn (ACE_THR_FUNC (test),
					     (void *) &pm,
					     THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n%a", "thread create failed"));

  // Wait for the threads to exit.
  ACE_Thread_Manager::instance ()->wait ();

#else
  ACE_ERROR ((LM_ERROR, 
	      "threads *and* processes not supported on this platform\n%"));
#endif /* ACE_HAS_THREADS */	
}

int
main (int, char *[])
{
  ACE_START_TEST ("Mutex_Test");

  spawn ();

  ACE_END_TEST;
  return 0;
}

