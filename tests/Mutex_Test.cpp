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

#include "ace/Synch.h"
#include "ace/Log_Msg.h"
#include "ace/Thread_Manager.h"
#include "test_config.h"

static void
test (void)
{
  char *name = "hello";

  ACE_Process_Mutex pm (name);

  for (int i = 0; i < ACE_MAX_ITERATIONS; i++)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) = trying to acquire\n"));
      ACE_ASSERT (pm.acquire () == 0);
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) = acquired\n"));

      ACE_OS::sleep (5);

      ACE_ASSERT (pm.release () == 0);
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) = released\n"));
    }
}

static void
spawn (void)
{
#if !defined (ACE_WIN32)
  switch (ACE_OS::fork ())
    {
    case -1:
      ACE_ERROR ((LM_ERROR, "%p\n%a", "fork failed"));
      exit (-1);
    case 0: 
      test ();
    default:
      test ();
    }
#elif defined (ACE_HAS_THREADS)
  ACE_Thread_Manager thr_mgr;
  if (thr_mgr.spawn (ACE_THR_FUNC (test),
		     (void *) 0,
		     THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n%a", "thread create failed"));

  if (thr_mgr.spawn (ACE_THR_FUNC (test),
		     (void *) 0,
		     THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n%a", "thread create failed"));
  thr_mgr.wait ();
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

