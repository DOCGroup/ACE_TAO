// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
// 
// = FILENAME
//    Barrier_Test.cpp
//
// = DESCRIPTION
//     This program illustrates how the ACE barrier synchronization
//     mechanisms work. 
//
// = AUTHOR
//    Prashant Jain and Doug Schmidt
// 
// ============================================================================

#include "test_config.h"
#include "ace/Synch.h"
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
  // Reference to the tester barrier.  This controls each miteration
  // of the tester function running in every thread.

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

#endif /* ACE_HAS_THREADS */

int 
main (int, char *[])
{
  ACE_START_TEST ("Barrier_Test");

#if defined (ACE_HAS_THREADS)
  int n_threads = ACE_MAX_THREADS;
  int n_iterations = ACE_MAX_ITERATIONS;

  ACE_Barrier tester_barrier (n_threads);
  
  Tester_Args args (tester_barrier, n_iterations);

  for (int iteration_count = 0;
       iteration_count < ACE_MAX_ITERATIONS;
       iteration_count++)
    {
      ACE_DEBUG ((LM_DEBUG, "starting iteration %d\n", 
		  iteration_count));

      if (ACE_Thread_Manager::instance ()->spawn_n 
          (n_threads,
	   ACE_THR_FUNC (tester), 
	   (void *) &args,
	   THR_NEW_LWP | THR_JOINABLE) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "spawn_n"), 1);

      ACE_Thread_Manager::instance ()->wait ();
    }

  ACE_DEBUG ((LM_DEBUG, "test done\n"));
#else
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return 0;
}
