// $Id$

// This test program verifies the functionality of the ACE_OS
// implementation of recursive mutexes on Win32 and Posix pthreads.

#include "ace/OS_main.h"
#include "ace/Service_Config.h"
#include "ace/Thread_Manager.h"
#include "ace/Get_Opt.h"

ACE_RCSID(Threads, recursive_mutex, "$Id$")

#if defined (ACE_HAS_THREADS)

#include "ace/Guard_T.h"
#include "ace/Recursive_Thread_Mutex.h"

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
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("n:t:"));

  int c;

  while ((c = get_opt ()) != -1)
    switch (c)
      {
      case 'n':
        n_iterations = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      case 't':
        n_threads = ACE_OS::atoi (get_opt.opt_arg ());
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
        // This illustrates the use of the ACE_GUARD with an
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
  ACE_Recursive_Thread_Mutex *rm
    = (ACE_Recursive_Thread_Mutex *) arg;

  recursive_worker (0, rm);
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Service_Config daemon (argv[0]);

  parse_args (argc, argv);
  ACE_Recursive_Thread_Mutex rm;

  ACE_Thread_Manager::instance ()->spawn_n (n_threads,
                                            ACE_THR_FUNC (worker),
                                            (void *) &rm);

  ACE_Thread_Manager::instance ()->wait ();
  return 0;
}
#else
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     "ACE doesn't support support process mutexes on this platform (yet)\n"),
                    -1);
}
#endif /* ACE_WIN32 */
