// $Id$

// Test out the group management mechanisms provided by the
// ACE_Thread_Manager, including the group signal handling, group
// suspension and resumption, and cooperative thread cancellation
// mechanisms.

#include "ace/OS_NS_unistd.h"
#include "ace/OS_main.h"
#include "ace/Service_Config.h"
#include "ace/Thread_Manager.h"
#include "ace/Signal.h"



#if defined (ACE_HAS_THREADS)

extern "C" void
handler (int signum)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) received signal %d\n", signum));
}

static void *
worker (intptr_t iterations)
{
  for (intptr_t i = 0; i < iterations; i++)
    {
      if ((i % 1000) == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%t) checking cancellation before iteration %d!\n",
                      i));

          if (ACE_Thread_Manager::instance ()->testcancel (ACE_Thread::self ()) != 0)
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

static const int DEFAULT_THREADS = ACE_DEFAULT_THREADS;
static const intptr_t DEFAULT_ITERATIONS = 100000;

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Service_Config daemon;

  daemon.open (argv[0]);

  // Register a signal handler.
  ACE_Sig_Action sa ((ACE_SignalHandler) handler, SIGINT);
  ACE_UNUSED_ARG (sa);

  int n_threads = argc > 1 ? ACE_OS::atoi (argv[1]) : DEFAULT_THREADS;
  intptr_t n_iterations =
    argc > 2 ? ACE_OS::atoi (argv[2]) : DEFAULT_ITERATIONS;

  ACE_Thread_Manager *thr_mgr = ACE_Thread_Manager::instance ();

  int grp_id = thr_mgr->spawn_n (n_threads,
                                 ACE_THR_FUNC (worker),
                                 reinterpret_cast<void *> (n_iterations),
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
  return 0;
}
#else
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR_RETURN ((LM_ERROR, "threads not supported on this platform\n"), -1);
}
#endif /* ACE_HAS_THREADS */
