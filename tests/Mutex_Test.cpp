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
#include "ace/Log_Msg.h"
#include "ace/Thread_Manager.h"
#include "test_config.h"

static void *
test (void *args)
{
  ACE_Process_Mutex *pm = (ACE_Process_Mutex *) args;
  ACE_Thread_Control tc (ACE_Service_Config::thr_mgr ());

  ACE_OS::srand (ACE_OS::time (0));

  for (int i = 0; i < ACE_MAX_ITERATIONS; i++)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) = trying to acquire\n"));
      ACE_ASSERT (pm->acquire () == 0);
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) = acquired\n"));

      // Sleep for a random amount of time between 0 and 5 seconds.
      // Note that it's ok to use rand() here because we are running
      // within the critical section defined by the Process_Mutex.
      ACE_OS::sleep (ACE_OS::rand () % 5);

      ACE_ASSERT (pm->release () == 0);
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) = released\n"));
    }

  return 0;
}

static void
spawn (void)
{
  char *name = "hello";

#if !defined (ACE_WIN32) && !defined (VXWORKS)
  switch (ACE_OS::fork ())
    {
    case -1:
      ACE_ERROR ((LM_ERROR, "%p\n%a", "fork failed"));
      exit (-1);
    case 0: // In child
      {
	ACE_LOG_MSG->sync ("child");
	ACE_Process_Mutex pm (name);
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
  ACE_Process_Mutex pm (name);

  if (ACE_Service_Config::thr_mgr ()->spawn (ACE_THR_FUNC (test),
					     (void *) &pm,
					     THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n%a", "thread create failed"));

  if (ACE_Service_Config::thr_mgr ()->spawn (ACE_THR_FUNC (test),
					     (void *) &pm,
					     THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n%a", "thread create failed"));

  // Wait for the threads to exit.
  ACE_Service_Config::thr_mgr ()->wait ();

#else
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n%a", 1));
#endif /* ACE_HAS_THREADS */	
}

int
main (int, char *argv[])
{
  ACE_START_TEST ("Mutex_Test.cpp");

  spawn ();

  ACE_END_TEST;
  return 0;
}

