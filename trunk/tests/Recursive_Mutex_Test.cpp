// ============================================================================
// $Id$

//
// = LIBRARY
//    tests
// 
// = FILENAME
//    Recursive_Mutex_Test.cpp
//
// = DESCRIPTION
//      This test program verifies the functionality of the ACE_OS
//      implementation of recursive mutexes on Win32 and Posix
//      pthreads. 
//
// = AUTHOR
//    Prashant Jain and Doug C. Schmidt
// 
// ============================================================================
#include "ace/Log_Msg.h"

#if defined (ACE_HAS_THREADS)

#include "ace/Service_Config.h"
#include "ace/Get_Opt.h"
#include "ace/Synch.h"
#include "test_config.h"

// Total number of iterations.
static size_t n_iterations = 100;
static size_t n_threads = ACE_MAX_THREADS;

static void
recursive_worker (size_t nesting_level, 
		  ACE_Recursive_Thread_Mutex *rm)
{
  if (nesting_level < n_iterations)
    {
      ACE_ASSERT (rm->acquire () == 0);
      ACE_DEBUG ((LM_DEBUG,
      		  "(%P|%t) = acquired, nesting = %d, thread id = %u\n",
		  rm->get_nesting_level (), rm->get_thread_id ()));

      recursive_worker (nesting_level + 1, rm);

      ACE_ASSERT (rm->release () == 0);
      ACE_DEBUG ((LM_DEBUG,
      		  "(%P|%t) = released, nesting = %d, thread id = %u\n",
		  rm->get_nesting_level (), rm->get_thread_id ()));
    }
}

static void *
worker (void *arg)
{
  ACE_Thread_Control tc (ACE_Service_Config::thr_mgr ());
  ACE_NEW_THREAD;

  ACE_Recursive_Thread_Mutex *rm = (ACE_Recursive_Thread_Mutex *) arg;
  recursive_worker (0, rm);
  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_START_TEST ("Recursive_Mutex_Test.cpp");

  ACE_Service_Config daemon (argv[0]);
  ACE_Recursive_Thread_Mutex rm;
  ACE_Service_Config::thr_mgr ()->spawn_n (n_threads, 
					   ACE_THR_FUNC (worker), 
					   (void *) &rm);

  ACE_Service_Config::thr_mgr ()->wait ();

  ACE_END_TEST;
  return 0;
}
#else
int 
main (void)
{
  ACE_ERROR_RETURN ((LM_ERROR, 
		     "ACE doesn't support support process mutexes on this platform (yet)\n"), 
		    -1);
}
#endif /* ACE_WIN32 */
