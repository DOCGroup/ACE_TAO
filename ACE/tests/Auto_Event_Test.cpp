// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Auto_Event Test
//
// = DESCRIPTION
//    This test verifies the functionality of the <ACE_Auto_Event>
//    implementation.
//
// = AUTHOR
//    Martin Corino <mcorino@remedy.nl>
//
// ============================================================================

#include "test_config.h"
#include "ace/Auto_Event.h"
#include "ace/Thread.h"
#include "ace/Thread_Manager.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_time.h"
#include "ace/OS_NS_unistd.h"



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

// Event used in the tests.  Start it "unsignalled" (i.e., its initial
// state is 0).
static ACE_Auto_Event evt ((unsigned int) 0);

// Default number of iterations.
static int n_iterations = 10;

// Number of worker threads.
static size_t n_workers = 10;

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

  if (evt.wait (&wait) == -1)
    {
      if (errno != ETIME)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("%p\n"),
                      ACE_TEXT ("test_timeout should be ETIME but is")));
          status = -1;
        }
    }
  ACE_Time_Value wait_diff = ACE_OS::gettimeofday () - begin;

  msecs_waited = wait_diff.msec ();
  msecs_expected = wait_secs * 1000;
  msecs_diff = labs (msecs_expected - msecs_waited);

  if (msecs_diff > ACE_ALLOWED_SLACK)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Timed wait fails length test\n")));
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Expected %d ms, actual %d ms; %d allowed\n"),
                  (int)msecs_expected,
                  (int)msecs_waited,
                  (int)ACE_ALLOWED_SLACK));
      status = -1;
    }

  ++wait_secs;
  return status;
}

// Explain usage and exit.
static void
print_usage_and_die (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("usage: %n [-w n_workers] [-n iteration_count]\n")));
  ACE_OS::exit (1);
}

static void
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("w:n:"));

  int c;

  while ((c = get_opt ()) != -1)
    switch (c)
    {
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
      //FUZZ: disable check_for_lack_ACE_OS
      ACE_Time_Value wait (0,
                           iterations * 1000 * 100);  // Wait 'iter' msec
      //FUZZ: enable check_for_lack_ACE_OS

      ACE_Time_Value tv = ACE_OS::gettimeofday () + wait;
      if (evt.wait (&tv) == -1)
      {
          // verify that we have ETIME
          if (ACE_OS::last_error() != ETIME)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("%p\n"),
                          ACE_TEXT ("Worker should be ETIME but is")));
            }
          else
            ++timeouts;
          ACE_Time_Value diff = ACE_OS::gettimeofday ();
          diff = diff - tv;       // tv should have been reset to time acquired
          long diff_msec = diff.msec ();

          if (diff_msec > ACE_ALLOWED_SLACK)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("Acquire fails time reset test\n")));
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("Diff btw now and returned time: %d ms; ")
                          ACE_TEXT ("%d allowed\n"),
                          (int)diff_msec,
                          (int)ACE_ALLOWED_SLACK));
              test_result = 1;
            }
          // Hold the lock for a while.
          ACE_OS::sleep (ACE_Time_Value (0,
                                         (ACE_OS::rand () % 1000) * 1000));
          evt.signal ();
      }

      ACE_Thread::yield ();
    }

  return 0;
}

#endif /* ACE_HAS_THREADS */

// Test event functionality.

int run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Auto_Event_Test"));

#if defined (ACE_HAS_THREADS)
  parse_args (argc, argv);
  ACE_OS::srand ((u_int) ACE_OS::time (0L));

  //Test timed waits.
  for (size_t i = 0; i < test_timeout_count; i++)
    if (test_timeout () != 0)
      test_result = 1;

  if (ACE_Thread_Manager::instance ()->spawn_n
      (static_cast<size_t> (n_workers),
       ACE_THR_FUNC (worker),
       0,
       THR_NEW_LWP) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("spawn_n")),
                      1);

  // Release the first worker.
  evt.signal ();

  ACE_Thread_Manager::instance ()->wait ();

  size_t percent = (timeouts * 100) / (n_workers * n_iterations);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Worker threads timed out %d percent of the time\n"),
              (int)percent));

  if (test_result == 0)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Auto_Event Test successful\n")));
#else
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("Threads not supported on this platform\n")));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return test_result;
}
