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

#include "ace/Synch.h"
#include "ace/Thread_Manager.h"
#include "ace/Service_Config.h"
#include "test_config.h"

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
  ACE_NEW_THREAD;

#if defined (VXWORKS)
  ACE_Thread_Control tc (ACE_Thread_Manager::instance ());
#endif /* VXWORKS */

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
  ACE_hthread_t *thread_handles;
  
  ACE_NEW_RETURN (thread_handles, ACE_hthread_t[n_threads], -1);
  
  Tester_Args args (tester_barrier, n_iterations);

  for (int iteration_count = 0;
       iteration_count < ACE_MAX_ITERATIONS;
       iteration_count++)
    {
      ACE_DEBUG ((LM_DEBUG, "starting iteration %d\n", 
		  iteration_count));

      if (ACE_Thread_Manager::instance ()->spawn_n 
	  ((ACE_thread_t *) 0,
	   n_threads,
	   ACE_THR_FUNC (tester), 
	   (void *) &args,
	   THR_NEW_LWP | THR_JOINABLE,
	   ACE_DEFAULT_THREAD_PRIORITY,
	   -1,
	   0,
	   0,
	   thread_handles) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "spawn_n"), 1);

#if defined (VXWORKS)
      // VxWorks doesn't support thr_join() semantics...  Someday
      // we'll fix this.
      ACE_Thread_Manager::instance ()->wait ();
#else
      // Wait for all the threads to reach their exit point and then join
      // with all the exiting threads.
      for (int i = 0;
	   i < n_threads;
	   i++)
	if (ACE_Thread::join (thread_handles[i]) == -1)
	  ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "join"), -1);
#endif /* VXWORKS */     
    }
  
  ACE_DEBUG ((LM_DEBUG, "test done\n"));
#else
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return 0;
}
