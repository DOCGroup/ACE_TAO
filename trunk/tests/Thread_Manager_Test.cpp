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
//      This program tests the group management mechanisms provided by
//      the <ACE_Thread_Manager>, including the group signal handling,
//      group suspension and resumption, and cooperative thread
//      cancellation mechanisms.
//
// = AUTHOR
//    Prashant Jain and Doug C. Schmidt
// 
// ============================================================================

#include "ace/Service_Config.h"
#include "ace/Thread_Manager.h"
#include "test_config.h"

#if defined (ACE_HAS_THREADS)

class Signal_Catcher
  // = TITLE
  //     Keeps track of whether a thread has been signaled.
{
public:
  Signal_Catcher (void): signaled_ (0) {}

  sig_atomic_t signaled (void)
  {
    return this->signaled_;
  }

  void signaled (sig_atomic_t s)
  {
    this->signaled_ = s;
  }

private:
  sig_atomic_t signaled_;
};

// Each thread keeps track of whether it has been signaled within a
// separate TSS entry.
// See comment below about why it's dynamically allocated.
static ACE_TSS<Signal_Catcher> *signal_catcher;

extern "C" void
handler (int signum)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) received signal %d, signaled = %d\n", 
	      signum, (*signal_catcher)->signaled ()));
  (*signal_catcher)->signaled (1);
}

static void *
worker (int iterations)
{
  ACE_NEW_THREAD;

  ACE_Thread_Manager *thr_mgr = ACE_Thread_Manager::instance ();

  for (int i = 0; i < iterations; i++)
    {
      if ((i % 1000) == 0)
	{
#if !defined (ACE_LACKS_UNIX_SIGNALS)
	  if ((*signal_catcher)->signaled () > 0
	      // Only test for cancellation after we've been signaled, to
	      // avoid race conditions for suspend() and resume().
	      && thr_mgr->testcancel (ACE_Thread::self ()) != 0)
	    {
	      ACE_DEBUG ((LM_DEBUG, 
			  "(%t) has been cancelled before iteration %d!\n", 
			  i));
	      break;
	    }
#else
          ACE_UNUSED_ARG (thr_mgr);
#endif /* ACE_LACKS_UNIX_SIGNALS */
	  ACE_OS::sleep (1);
	}
    }
  // Destructor removes thread from Thread_Manager.
  return 0;
}

static const int DEFAULT_THREADS = ACE_MAX_THREADS;
static const int DEFAULT_ITERATIONS = 100000;

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_TSS<Signal_Catcher>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_TSS<Signal_Catcher>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* ACE_HAS_THREADS */

int
main (int, char *[])
{
  ACE_START_TEST ("Thread_Manager_Test");

#if defined (ACE_HAS_THREADS)
  // Dynamically allocate signal_catcher so that we can control when
  // it gets deleted.  Specifically, we need to delete it before
  // the main thread's TSS is cleaned up.
  ACE_NEW_RETURN (signal_catcher, ACE_TSS<Signal_Catcher>, 1);

  ACE_Service_Config daemon;

  // Register a signal handler.
  ACE_Sig_Action sa ((ACE_SignalHandler) handler, SIGINT);
  ACE_UNUSED_ARG (sa);

  int n_threads = DEFAULT_THREADS;
  int n_iterations = DEFAULT_ITERATIONS;

  ACE_Thread_Manager *thr_mgr = ACE_Thread_Manager::instance ();

  int grp_id = thr_mgr->spawn_n (n_threads,
				 ACE_THR_FUNC (worker),
				 (void *) n_iterations,
				 THR_BOUND | THR_DETACHED);

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

  delete signal_catcher;

#else
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
#endif /* ACE_HAS_THREADS */

  ACE_END_TEST;
  return 0;
}
