
//=============================================================================
/**
 *  @file    Barrier_Test.cpp
 *
 *  $Id$
 *
 *   This program illustrates how the ACE barrier synchronization
 *   mechanisms work.
 *
 *  @author Prashant Jain <pjain@cs.wustl.edu> and Doug Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================


#include "test_config.h"
#include "ace/Barrier.h"
#include "ace/Thread_Manager.h"



#if defined (ACE_HAS_THREADS)

struct Tester_Args
  // = TITLE
  //     These arguments are passed into each test thread.
{
  Tester_Args (ACE_Barrier &tb, int i)
    : tester_barrier_ (tb),
    n_iterations_ (i) {}

  ACE_Barrier &tester_barrier_;
  // Reference to the tester barrier.  This controls each iteration
  // of the tester function running in every thread.

  int n_iterations_;
  // Number of iterations to run.
};

// Iterate <n_iterations> time printing off a message and "waiting"
// for all other threads to complete this iteration.

static void *
wait_tester (Tester_Args *args)
{
  for (int iterations = 1;
       iterations <= args->n_iterations_;
       iterations++)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) in iteration %d\n"),
                  iterations));

      // Block until all other threads have waited, then continue.
      if (args->tester_barrier_.wait () != 0)
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%t) %p\n"),
                    ACE_TEXT ("wait failed")));
    }

  return 0;
}

// Wait on the barrier, expecting it to be shut down before completing
// the wait.

static void *
shut_tester (Tester_Args *args)
{
  if (args->tester_barrier_.wait () == 0)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%t) wait succeeded, should have shut down\n")));
  else if (errno != ESHUTDOWN)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%t) wait failed, expecting ESHUTDOWN, %p\n"),
                ACE_TEXT ("got")));
  return 0;
}

#endif /* ACE_HAS_THREADS */

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Barrier_Test"));

#if defined (ACE_HAS_THREADS)
  int n_threads = ACE_MAX_THREADS;
  int n_iterations = ACE_MAX_ITERATIONS;

  ACE_Barrier tester_barrier (n_threads);

  Tester_Args args (tester_barrier, n_iterations);

  for (size_t iteration_count = 0;
       iteration_count < ACE_MAX_ITERATIONS;
       iteration_count++)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("starting iteration %d\n"),
                  iteration_count));

      if (ACE_Thread_Manager::instance ()->spawn_n
          (n_threads,
           (ACE_THR_FUNC) wait_tester,
           (void *) &args,
           THR_NEW_LWP | THR_JOINABLE) == -1)

        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                           ACE_TEXT ("spawn_n")), 1);

      ACE_Thread_Manager::instance ()->wait ();
    }

  // Now test ACE_Barrier shutdown. Set up a barrier for n_threads, and start
  // n_threads - 1 threads to wait, then shut the barrier down.
  ACE_Barrier shut_barrier (n_threads);
  Tester_Args shut_args (shut_barrier, 1);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Starting shutdown test threads\n")));
  if (ACE_Thread_Manager::instance ()->spawn_n
      (n_threads - 1,
       (ACE_THR_FUNC) shut_tester,
       (void *) &shut_args,
       THR_NEW_LWP | THR_JOINABLE) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("spawn_n")), 1);

  shut_barrier.shutdown ();
  ACE_Thread_Manager::instance ()->wait ();

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("test done\n")));
#else
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("threads not supported on this platform\n")));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return 0;
}
