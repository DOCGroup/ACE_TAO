// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
// 
// = FILENAME
//    Thread_Manager_Test.cpp
//
// = DESCRIPTION
//      This program tests out the group management mechanisms
//      provided by the ACE_Thread_Manager, including the group signal
//      handling, group suspension and resumption, and cooperative
//      thread cancellation mechanisms.
//
// = AUTHOR
//    Prashant Jain and Doug C. Schmidt
// 
// ============================================================================


#include "ace/Service_Config.h"
#include "ace/Thread_Manager.h"
#include "test_config.h"

#if defined (ACE_HAS_THREADS)

extern "C" void
handler (int signum)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) received signal %d\n", signum));
}

static void *
worker (int iterations)
{
  ACE_Thread_Control tc (ACE_Service_Config::thr_mgr ());
  ACE_NEW_THREAD;

  for (int i = 0; i < iterations; i++)
    {
      if ((i % 1000) == 0)
	{
	  if (ACE_Service_Config::thr_mgr ()->testcancel (ACE_Thread::self ()) != 0)
	    {
	      ACE_DEBUG ((LM_DEBUG, 
			  "(%t) has been cancelled before iteration %d!\n", 
			  i));
	      break;
	    }
	}
    }

  // Destructor removes thread from Thread_Manager.
  return 0;
}

static const int DEFAULT_THREADS = ACE_MAX_THREADS;
static const int DEFAULT_ITERATIONS = 100000;

#endif /* ACE_HAS_THREADS */

int
main (int, char *[])
{
  ACE_START_TEST ("Thread_Manager_Test");

#if defined (ACE_HAS_THREADS)
  ACE_Service_Config daemon;

  // Register a signal handler.
  ACE_Sig_Action sa ((ACE_SignalHandler) handler, SIGINT);

  int n_threads = DEFAULT_THREADS;
  int n_iterations = DEFAULT_ITERATIONS;

  ACE_Thread_Manager *thr_mgr = ACE_Service_Config::thr_mgr ();

  int grp_id = thr_mgr->spawn_n (n_threads, ACE_THR_FUNC (worker),
				 (void *) n_iterations,
				 THR_NEW_LWP | THR_DETACHED);

  // Wait for 1 second and then suspend every thread in the group.
  ACE_OS::sleep (1);
  ACE_DEBUG ((LM_DEBUG, "(%t) suspending group\n"));
  if (thr_mgr->suspend_grp (grp_id) == -1)
    ACE_ERROR ((LM_DEBUG, "(%t) %p\n", "suspend_grp"));

  // Wait for 1 more second and then resume every thread in the
  // group.
  ACE_OS::sleep (ACE_Time_Value (1));
  ACE_DEBUG ((LM_DEBUG, "(%t) resuming group\n"));
  if (thr_mgr->resume_grp (grp_id) == -1)
    ACE_ERROR ((LM_DEBUG, "(%t) %p\n", "resume_grp"));

  // Wait for 1 more second and then send a SIGINT to every thread in
  // the group.
  ACE_OS::sleep (ACE_Time_Value (1));
  ACE_DEBUG ((LM_DEBUG, "(%t) signaling group\n"));
  if (thr_mgr->kill_grp (grp_id, SIGINT) == -1)
    ACE_ERROR ((LM_DEBUG, "(%t) %p\n", "kill_grp"));

  // Wait for 1 more second and then cancel all the threads.
  ACE_OS::sleep (ACE_Time_Value (1));
  ACE_DEBUG ((LM_DEBUG, "(%t) cancelling group\n"));
  if (thr_mgr->cancel_grp (grp_id) == -1)
    ACE_ERROR ((LM_DEBUG, "(%t) %p\n", "cancel_grp"));

  // Perform a barrier wait until all the threads have shut down.
  thr_mgr->wait ();
#else
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return 0;
}
