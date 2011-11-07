// $Id$

// This test program illustrates how the ACE barrier synchronization
// mechanisms work.

#include "ace/OS_main.h"
#include "ace/Barrier.h"
#include "ace/Thread_Manager.h"
#include "ace/Service_Config.h"



#if defined (ACE_HAS_THREADS)

struct Tester_Args
  // = TITLE
  //     These arguments are passed into each test thread.
{
  Tester_Args (ACE_Barrier &tb, int i)
    : tester_barrier_ (tb),
      n_iterations_ (i) {}

  ACE_Barrier &tester_barrier_;
  // Reference to the tester barrier.  This controls each miteration of
  // the tester function running in every thread.

  int n_iterations_;
  // Number of iterations to run.
};

// Iterate <n_iterations> time printing off a message and "waiting"
// for all other threads to complete this iteration.

static void *
tester (Tester_Args *args)
{
  for (int iterations = 1;
       iterations <= args->n_iterations_;
       iterations++)
    {
      ACE_DEBUG ((LM_DEBUG, "(%t) in iteration %d\n", iterations));

      // Block until all other threads have waited, then continue.
      args->tester_barrier_.wait ();
    }

  return 0;
}

// Default number of threads to spawn.
static const int DEFAULT_ITERATIONS = 5;

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Service_Config daemon (argv[0]);

  int n_threads = argc > 1 ? ACE_OS::atoi (argv[1]) : ACE_DEFAULT_THREADS;
  int n_iterations = argc > 2 ? ACE_OS::atoi (argv[2]) : DEFAULT_ITERATIONS;

  ACE_Barrier tester_barrier (n_threads);

  Tester_Args args (tester_barrier, n_iterations);

  if (ACE_Thread_Manager::instance ()->spawn_n
      (int(n_threads), ACE_THR_FUNC (tester),
       (void *) &args, THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "spawn_n"), 1);

  // Wait for all the threads to reach their exit point.
  ACE_Thread_Manager::instance ()->wait ();

  ACE_DEBUG ((LM_DEBUG, "(%t) done\n"));
  return 0;
}
#else
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
  return 0;
}
#endif /* ACE_HAS_THREADS */
