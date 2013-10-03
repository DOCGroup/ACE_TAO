// $Id$

// Test out the cooperative thread cancellation mechanisms provided by
// the ACE_Thread_Manager.

#include "ace/OS_NS_unistd.h"
#include "ace/OS_main.h"
#include "ace/Service_Config.h"
#include "ace/Thread_Manager.h"

#if defined (ACE_HAS_THREADS)

static void *
worker (intptr_t iterations)
{
  for (intptr_t i = 0; i < iterations; i++)
    {
      if ((i % 10) == 0
          && (ACE_Thread_Manager::instance ()->testcancel (ACE_Thread::self ()) != 0))
        {
          ACE_DEBUG ((LM_DEBUG, "(%t) has been cancelled before iteration!\n", i));
          break;
        }
    }

  return 0;
}

static const int DEFAULT_THREADS = ACE_DEFAULT_THREADS;
static const intptr_t DEFAULT_ITERATIONS = 100000;

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Service_Config daemon;

  daemon.open (argv[0]);

  int n_threads = argc > 1 ? ACE_OS::atoi (argv[1]) : DEFAULT_THREADS;
  intptr_t n_iterations =
    argc > 2 ? ACE_OS::atoi (argv[2]) : DEFAULT_ITERATIONS;

  ACE_Thread_Manager *thr_mgr = ACE_Thread_Manager::instance ();

  int grp_id = thr_mgr->spawn_n (n_threads, ACE_THR_FUNC (worker),
                                 (void *) n_iterations,
                                 THR_NEW_LWP | THR_DETACHED);

  // Wait for 2 seconds and then suspend every thread in the group.
  ACE_OS::sleep (2);
  thr_mgr->suspend_grp (grp_id);

  // Wait for 2 more seconds and then resume every thread in the
  // group.
  ACE_OS::sleep (ACE_Time_Value (2));
  thr_mgr->resume_grp (grp_id);

  // Wait for 2 more seconds and then send a SIGINT to every thread in
  // the group.
  ACE_OS::sleep (ACE_Time_Value (2));
  thr_mgr->kill_grp (grp_id, SIGINT);

  // Wait for 2 more seconds and then exit (which should kill all the
  // threads)!
  ACE_OS::sleep (ACE_Time_Value (2));

  return 0;
}
#else
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR_RETURN ((LM_ERROR, "threads not supported on this platform\n"), -1);
}
#endif /* ACE_HAS_THREADS */
