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
//    Prashant Jain and Douglas C. Schmidt
//
// ============================================================================

#include "ace/Thread_Manager.h"
#include "ace/Signal.h"
#include "test_config.h"

#if defined (ACE_HAS_THREADS)

#include "Thread_Manager_Test.h"

// Each thread keeps track of whether it has been signaled within a
// separate TSS entry.  See comment below about why it's allocated
// dynamically.
static ACE_TSS<Signal_Catcher> *signal_catcher = 0;

// Synchronize starts of threads, so that they all start before the
// main thread cancels them.  To avoid creating a static object, it
// is dynamically allocated, before spawning any threads.
static ACE_Barrier *thread_start;

extern "C" void
handler (int signum)
{
  if (signal_catcher)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%t) received signal %d, signaled = %d\n",
                  signum,
                  (*signal_catcher)->signaled ()));
      (*signal_catcher)->signaled (1);
    }
}

static void *
worker (int iterations)
{
#if !defined (ACE_LACKS_UNIX_SIGNALS)
  // Cache a pointer to this thread's Signal_Catcher.  That way, we
  // try to avoid dereferencing signal_catcher below in a thread that
  // hasn't terminated when main exits.  That shouldn't happen, but it
  // seems to on Linux and LynxOS.
  if (!signal_catcher)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%t) (worker): signal catcher is 0!!!!\n"));
      return (void *) -1;
    }

  Signal_Catcher *my_signal_catcher = *signal_catcher;

  if (my_signal_catcher == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%t) (worker): *signal catcher is 0!!!!\n"));
      return (void *) -1;
    }

  ACE_Thread_Manager *thr_mgr = ACE_Thread_Manager::instance ();
#endif /* ! ACE_LACKS_UNIX_SIGNAL */

  // After setting up the signal catcher, block on the start barrier.
  thread_start->wait ();

  ACE_DEBUG ((LM_DEBUG,
              "(%t) worker starting loop\n"));

  for (int i = 0; i < iterations; i++)
    {
      if ((i % 1000) == 0)
        {
#if !defined (ACE_LACKS_UNIX_SIGNALS)
          if (my_signal_catcher->signaled () > 0  &&
              // Only test for cancellation after we've been signaled,
              // to avoid race conditions for suspend() and resume().
              thr_mgr->testcancel (ACE_Thread::self ()) != 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "(%t) has been cancelled before iteration %d!\n",
                          i));
              break;
            }
#endif /* ! ACE_LACKS_UNIX_SIGNAL */
          ACE_OS::sleep (1);
        }
    }

  // Destructor removes thread from Thread_Manager.
  return 0;
}

static const int DEFAULT_THREADS = ACE_MAX_THREADS;
static const int DEFAULT_ITERATIONS = 10000;

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
  int n_threads = DEFAULT_THREADS;
  int n_iterations = DEFAULT_ITERATIONS;

  // Dynamically allocate signal_catcher so that we can control when
  // it gets deleted.  Specifically, we need to delete it before the
  // main thread's TSS is cleaned up.
  ACE_NEW_RETURN (signal_catcher, ACE_TSS<Signal_Catcher>, 1);

  // And similarly, dynamically allocate the thread_start barrier.
  ACE_NEW_RETURN (thread_start, ACE_Barrier (n_threads + 1), -1);

  // Register a signal handler.
  ACE_Sig_Action sa ((ACE_SignalHandler) handler, SIGINT);
  ACE_UNUSED_ARG (sa);

  ACE_Thread_Manager *thr_mgr = ACE_Thread_Manager::instance ();

  int grp_id = thr_mgr->spawn_n (n_threads,
                                 ACE_THR_FUNC (worker),
                                 (void *) n_iterations,
                                 THR_BOUND | THR_DETACHED);

  ACE_ASSERT (grp_id != -1);
  thread_start->wait ();

  // Pthreads doesn't do suspend/resume.  Neither do DCEThreads.
#if !defined (ACE_HAS_PTHREADS) && !defined (ACE_HAS_DCETHREADS)
  // Wait for 1 second and then suspend every thread in the group.
  ACE_OS::sleep (1);
  ACE_DEBUG ((LM_DEBUG, "(%t) suspending group\n"));

  ACE_ASSERT (thr_mgr->suspend_grp (grp_id) != -1);

  // Wait for 1 more second and then resume every thread in the
  // group.
  ACE_OS::sleep (ACE_Time_Value (1));

  ACE_DEBUG ((LM_DEBUG, "(%t) resuming group\n"));

  ACE_ASSERT (thr_mgr->resume_grp (grp_id) != -1);
#endif /* ! ACE_HAS_PTHREADS && ! ACE_HAS_DCETHREADS */

  // Wait for 1 more second and then send a SIGINT to every thread in
  // the group.
  ACE_OS::sleep (ACE_Time_Value (1));

  ACE_DEBUG ((LM_DEBUG, "(%t) signaling group\n"));

#if defined (ACE_HAS_WTHREADS)
  thr_mgr->kill_grp (grp_id, SIGINT);
#else
  ACE_ASSERT (thr_mgr->kill_grp (grp_id, SIGINT) != -1);
#endif /* ACE_HAS_WTHREADS */

  // Wait for 1 more second and then cancel all the threads.
  ACE_OS::sleep (ACE_Time_Value (1));

  ACE_DEBUG ((LM_DEBUG, "(%t) cancelling group\n"));

  ACE_ASSERT (thr_mgr->cancel_grp (grp_id) != -1);

  // Perform a barrier wait until all the threads have shut down.
  ACE_ASSERT (thr_mgr->wait () != -1);

  ACE_DEBUG ((LM_DEBUG, "(%t) main thread finished\n"));

  delete thread_start;
  thread_start = 0;
  delete signal_catcher;
  signal_catcher = 0;

#else
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
#endif /* ACE_HAS_THREADS */

  ACE_END_TEST;
  return 0;
}
