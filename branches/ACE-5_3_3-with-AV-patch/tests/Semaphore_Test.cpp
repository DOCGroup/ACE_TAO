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
//    This test verifies the functionality of the <ACE_Thread_Semaphore>
//    implementation.
//
// = AUTHOR
//    Darrell Brunsch <brunsch@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/Synch.h"
#include "ace/Thread.h"
#include "ace/Thread_Manager.h"
#include "ace/Get_Opt.h"

ACE_RCSID(tests, Semaphore_Test, "$Id$")

// msec that times are allowed to differ before test fails.
#if defined (ACE_HAS_HI_RES_TIMER) || defined (ACE_HAS_AIX_HI_RES_TIMER) || \
    defined (ACE_HAS_PENTIUM) || defined (ACE_HAS_ALPHA_TIMER) || \
    defined (ACE_HAS_POWERPC_TIMER)
# define ACE_ALLOWED_SLACK 100
#else  /* don't have a high-res timer */
# define ACE_ALLOWED_SLACK 1100
#endif /* don't have a high-res timer */

// Test results, 'success' is 0
static int test_result = 0;

#if defined (ACE_HAS_THREADS)

// Semaphore used in the tests.  Start it "locked" (i.e., its initial
// count is 0).
static ACE_Thread_Semaphore s (0);

// Default number of iterations.
static int n_iterations = 10;

// Number of worker threads.
static size_t n_workers = 10;

// Amount to release the semaphore.
static u_int n_release_count = 3;

#if !defined (ACE_HAS_STHREADS) && !defined (ACE_HAS_POSIX_SEM)
// Number of timeouts.
static size_t timeouts = 0;

// Number of times to call test_timeout ().
static size_t test_timeout_count = 3;

// Tests the amount of time spent in a timed wait.
static int
test_timeout (void)
{
  int status = 0;

  // milliseconds...
  long msecs_expected;
  long msecs_waited;
  long msecs_diff;

  // Wait a little longer each time
  static long wait_secs = 3;

  ACE_Time_Value wait = ACE_OS::gettimeofday ();

  ACE_Time_Value begin = wait;

  wait.sec (wait.sec () + wait_secs);

  if (s.acquire (wait) == -1)
    ACE_ASSERT (errno == ETIME);

  ACE_Time_Value wait_diff = ACE_OS::gettimeofday () - begin;

  msecs_waited = wait_diff.msec ();
  msecs_expected = wait_secs * 1000;
  msecs_diff = labs (msecs_expected - msecs_waited);

  if (msecs_diff > ACE_ALLOWED_SLACK)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Timed wait fails length test\n")));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Value: %d ms, actual %d ms\n"),
                  msecs_expected,
                  msecs_waited));
      status = -1;
    }

  ++wait_secs;
  return status;
}
#endif /* ACE_HAS_STHREADS && ACE_HAS_POSIX_SEM */

// Explain usage and exit.
static void
print_usage_and_die (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("usage: %n [-s n_release_count] [-w n_workers] [-n iteration_count]\n")));
  ACE_OS::exit (1);
}

static void
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("s:w:n:"));

  int c;

  while ((c = get_opt ()) != -1)
    switch (c)
    {
    case 's':
      n_release_count = ACE_OS::atoi (get_opt.opt_arg ());
      break;
    case 'w':
      n_workers = ACE_OS::atoi (get_opt.opt_arg ());
      break;
    case 'n':
      n_iterations = ACE_OS::atoi (get_opt.opt_arg ());
      break;
    default:
      print_usage_and_die ();
      break;
  }
}

// Worker tries to acquire the semaphore, hold it for a while, and
// then releases it.

static void *
worker (void *)
{
  for (int iterations = 1;
       iterations <= n_iterations;
       iterations++)
    {
#if !defined (ACE_HAS_STHREADS) && !defined (ACE_HAS_POSIX_SEM)
      ACE_Time_Value wait (0,
                           iterations * 1000 * 100);  // Wait 'iter' msec
      ACE_Time_Value tv = ACE_OS::gettimeofday () + wait;

      if (s.acquire (tv))
        ++timeouts;
      else
        {
          ACE_Time_Value diff = ACE_OS::gettimeofday ();
          diff = diff - tv;       // tv should have been reset to time acquired
          if (diff.msec () > ACE_ALLOWED_SLACK)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("Acquire fails time reset test\n")));
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("Diff btw now and returned time: %d ms\n"),
                          diff.msec ()));
              test_result = 1;
            }
          // Hold the lock for a while.
          ACE_OS::sleep (ACE_Time_Value (0,
                                         (ACE_OS::rand () % 1000) * 1000));
          s.release ();
        }
#else
      s.acquire ();
      // Hold the lock for a while.
      ACE_OS::sleep (ACE_Time_Value (0,
                                     (ACE_OS::rand () % 1000) * 1000));
      s.release ();
#endif /* ACE_HAS_STHREADS && ACE_HAS_POSIX_SEM */
      ACE_Thread::yield ();
    }

  return 0;
}

#endif /* ACE_HAS_THREADS */

// Test semaphore functionality.

int run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Semaphore_Test"));

#if defined (ACE_HAS_THREADS)
  parse_args (argc, argv);
  ACE_OS::srand (ACE_OS::time (0L));

#  if !defined (ACE_HAS_STHREADS) && !defined (ACE_HAS_POSIX_SEM)
  //Test timed waits.
  for (size_t i = 0; i < test_timeout_count; i++)
    if (test_timeout () != 0)
      test_result = 1;
#  endif /* ACE_HAS_STHREADS && ACE_HAS_POSIX_SEM */

  // Release the semaphore a certain number of times.
  s.release (n_release_count);

  if (ACE_Thread_Manager::instance ()->spawn_n
      (ACE_static_cast (size_t, n_workers),
       ACE_THR_FUNC (worker),
       0,
       THR_NEW_LWP) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("spawn_n")),
                      1);

  ACE_Thread_Manager::instance ()->wait ();

#  if !defined (ACE_HAS_STHREADS) && !defined (ACE_HAS_POSIX_SEM)
  size_t percent = (timeouts * 100) / (n_workers * n_iterations);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Worker threads timed out %d percent of the time\n"),
              percent));
#  endif /* ACE_HAS_STHREADS && ACE_HAS_POSIX_SEM */

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Semaphore Test successful\n")));
#else
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("Threads not supported on this platform\n")));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return test_result;
}
