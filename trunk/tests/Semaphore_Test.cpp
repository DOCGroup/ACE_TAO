// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
// 
// = FILENAME
//    Semaphore Test
//
// = DESCRIPTION
//    This test verifies the functionality of the ACE_Thread_Semaphore
//    implementation.  
//
// = AUTHOR
//    Darrell Brunsch
// 
// ============================================================================

#include "ace/Synch.h"
#include "ace/Thread.h"
#include "ace/Service_Config.h"
#include "ace/Get_Opt.h"
#include "test_config.h"

#if defined (ACE_HAS_THREADS)

// Semaphore used in the tests.
static ACE_Thread_Semaphore s(0);

// Default number of iterations.
static size_t n_iterations = 10;

static size_t n_workers = 10;

static size_t n_semcount = 3;

static size_t timeouts = 0;

// Explain usage and exit.
static void 
print_usage_and_die (void)
{
  ACE_DEBUG ((LM_DEBUG, 
	      "usage: %n [-s n_semcount] [-w n_workers] [-n iteration_count]\n"));
  ACE_OS::exit (1);
}

static void
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, "s:w:n:");

  int c; 

  while ((c = get_opt ()) != -1)
    switch (c)
    {
    case 's':
      n_semcount = ACE_OS::atoi (get_opt.optarg);
      break;
    case 'w':
      n_workers = ACE_OS::atoi (get_opt.optarg);
      break;
    case 'n':
      n_iterations = ACE_OS::atoi (get_opt.optarg);
      break;
    default:
      print_usage_and_die ();
      break;
  }
}


// Tests the amount of time spent in a timed wait.
static int 
test_timeout (void)
{
  ACE_Time_Value begin, wait, diff;
  
  // Pick some random number of milliseconds to 
  wait = ACE_Time_Value (0, (ACE_OS::rand () % 10) * 100000);
  begin = ACE_OS::gettimeofday ();
  s.acquire(wait);
  diff = ACE_OS::gettimeofday () - begin;
  if (diff < wait)
  {
    ACE_DEBUG ((LM_DEBUG, "Timed wait fails length test\n"));
    ACE_DEBUG ((LM_DEBUG, "Value: %d us      Actual %d us\n", wait.usec (), diff.usec ()));
    return -1;
  }
  return 0;
}

// Worker tries to acquire the semaphore, hold it for a while, and then releases it.

static void *
worker (void *)
{
  ACE_Thread_Control tc (ACE_Thread_Manager::instance ());
  ACE_NEW_THREAD;
  
  for (size_t iterations = 1; iterations <= n_iterations; iterations++)
    {
      if (s.acquire (ACE_Time_Value (0, (ACE_OS::rand () % 1000) * 1000)))
        ++timeouts;
      else
        {
          // Hold the lock for a while.
          ACE_OS::sleep (ACE_Time_Value (0, (ACE_OS::rand () % 1000) * 1000));
          s.release ();
        }

      ACE_Thread::yield ();
    }

  return 0;
}

#endif /* ACE_HAS_THREADS */

// Test semaphore functionality

int main (int argc, char *argv[])
{
  ACE_START_TEST ("Semaphore_Test");

#if defined (ACE_HAS_THREADS)
  parse_args (argc, argv);
  ACE_OS::srand (ACE_OS::time (0L));
#if !defined (ACE_HAS_STHREADS)
  size_t i;  // Loop variable
  
  // Test out timed waits
  for (i = 0; i < 5; i++)
    test_timeout ();

  // Initialize the semaphore to a certain number
  for (i = 0; i < n_semcount; i++)
    s.release ();

  if (ACE_Thread_Manager::instance ()->spawn_n (n_workers, 
						ACE_THR_FUNC (worker), 
						0, 
						THR_NEW_LWP) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "spawn_n"), 1);

  ACE_Thread_Manager::instance ()->wait ();

  size_t percent = (timeouts * 100) / (n_workers * n_iterations);

  ACE_DEBUG ((LM_DEBUG, "Worker Threads timed out %d percent of the time\n", percent));

#endif /* ACE_HAS_STHREADS */
#else
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);
  ACE_ERROR ((LM_ERROR, "Threads not supported on this platform\n"));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return 0;
}

