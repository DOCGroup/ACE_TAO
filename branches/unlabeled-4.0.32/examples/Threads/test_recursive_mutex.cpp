// This test program verifies the functionality of the ACE_OS
// @(#)test_recursive_mutex.cpp	1.1	10/18/96

// implementation of recursive mutexes on Win32 and Posix pthreads.

#include "ace/Log_Msg.h"

#if defined (ACE_HAS_THREADS)

#include "ace/Service_Config.h"
#include "ace/Get_Opt.h"
#include "ace/Synch.h"

// Total number of iterations.
static size_t n_iterations = 1000;
static size_t n_threads = 4;

// Explain usage and exit.
static void 
print_usage_and_die (void)
{
  ACE_DEBUG ((LM_DEBUG, 
	      "usage: %n [-t n_threads] [-n iteration_count]\n"));
  ACE_OS::exit (1);
}

// Parse the command-line arguments and set options.

static void
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, "n:t:");

  int c; 

  while ((c = get_opt ()) != -1)
    switch (c)
      {
      case 'n':
	n_iterations = ACE_OS::atoi (get_opt.optarg);
	break;
      case 't':
	n_threads = ACE_OS::atoi (get_opt.optarg);
	break;
      default:
	print_usage_and_die ();
	break;
      }
}

static void
recursive_worker (size_t nesting_level, 
		  ACE_Recursive_Thread_Mutex *rm)
{
  if (nesting_level < n_iterations)
    {
      ACE_DEBUG ((LM_DEBUG, 
		  "(%P|%t) = trying to acquire, nesting = %d, thread id = %u\n",
		  rm->get_nesting_level (), rm->get_thread_id ()));
      {
	// This illustrates the use of the ACE_Guard<LOCK> with an
	// ACE_Recursive_Thread_Mutex.
	ACE_GUARD (ACE_Recursive_Thread_Mutex, ace_mon, *rm);
	ACE_DEBUG ((LM_DEBUG,
		    "(%P|%t) = acquired, nesting = %d, thread id = %u\n",
		    rm->get_nesting_level (), rm->get_thread_id ()));

	recursive_worker (nesting_level + 1, rm);
      }
      ACE_DEBUG ((LM_DEBUG,
      		  "(%P|%t) = released, nesting = %d, thread id = %u\n",
		  rm->get_nesting_level (), rm->get_thread_id ()));
    }
}

static void *
worker (void *arg)
{
  ACE_Thread_Control tc (ACE_Service_Config::thr_mgr ());

  ACE_Recursive_Thread_Mutex *rm = (ACE_Recursive_Thread_Mutex *) arg;

  recursive_worker (0, rm);
  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_Service_Config daemon (argv[0]);

  parse_args (argc, argv);
  ACE_Recursive_Thread_Mutex rm;

  ACE_Service_Config::thr_mgr ()->spawn_n (n_threads, 
					   ACE_THR_FUNC (worker), 
					   (void *) &rm);

  ACE_Service_Config::thr_mgr ()->wait ();
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
