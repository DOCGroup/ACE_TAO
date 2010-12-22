// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Manual_Event Test
//
// = DESCRIPTION
//    This test verifies the functionality of the <ACE_Manual_Event>
//    implementation.
//
// = AUTHOR
//    Martin Corino <mcorino@remedy.nl>
//
// ============================================================================

#include "test_config.h"
#include "ace/Manual_Event.h"
#include "ace/Thread.h"
#include "ace/Thread_Manager.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_time.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Atomic_Op.h"



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
static ACE_Manual_Event evt ((unsigned int) 0);

// Number of worker threads.
static long n_workers = 10;

// Number of wakeups.
#if defined (ACE_HAS_BUILTIN_ATOMIC_OP)
static ACE_Atomic_Op<ACE_Thread_Mutex, long>   n_awoken;
static ACE_Atomic_Op<ACE_Thread_Mutex, long>   n_awoken2;
#else
static long                                    n_awoken;
static long                                    n_awoken2;
#endif

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
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("w:"));

  int c;

  while ((c = get_opt ()) != -1)
    switch (c)
      {
      case 'w':
        n_workers = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      default:
        print_usage_and_die ();
        break;
      }
}

// Worker tries to acquire the semaphore, hold it for a while, and
// then manually releases it.

static void *
worker (void *)
{
  if (evt.wait () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT (" (%P|%t) %p\n"),
                       ACE_TEXT ("Failed waiting for pulse ()")),
                      0);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT (" (%P|%t) awake\n")));

  if (++n_awoken < n_workers)
    {
      //FUZZ: disable check_for_lack_ACE_OS
      ACE_Time_Value wait (1, 0);   // Wait 10 sec
      //FUZZ: enable check_for_lack_ACE_OS

      ACE_Time_Value tv = ACE_OS::gettimeofday () + wait;

      if (evt.wait (&tv) == -1)
        {
          // verify that we have ETIME
          if (ACE_OS::last_error () == ETIME)
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT (" (%P|%t) timeout\n")));
          else
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT (" (%P|%t) wait failed %p\n"),
                        ACE_TEXT ("but not with ETIME")));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT (" (%P|%t) awake in time\n")));

          if (++n_awoken2 >= (n_workers/2))
            evt.reset ();    // reset signal (rest times out)
        }

      // Check to ensure that an "infinite timeout" will work.
      if (evt.wait (0) == -1)
        {
          //FUZZ: disable check_for_lack_ACE_OS
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT (" (%P|%t) %p\n"),
                      ACE_TEXT ("Failed waiting for signal ()\n")));
          //FUZZ: enable check_for_lack_ACE_OS
        }
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT (" (%P|%t) last awake; send signal\n")));
      // last one wakes others
      if (evt.signal () == -1)
        ACE_ERROR ((LM_ERROR, ACE_TEXT (" (%P|%t) %p\n"), ACE_TEXT ("signal")));

      ACE_OS::sleep (ACE_Time_Value (0, 200 * 1000 * 100));  // 200 msec
    }

  if (evt.wait () == -1)
    {
      //FUZZ: disable check_for_lack_ACE_OS
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT (" (%P|%t) %p\n"),
                  ACE_TEXT ("Failed waiting for signal ()\n")));
      //FUZZ: enable check_for_lack_ACE_OS
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT (" (%P|%t) worker finished\n")));

  return 0;
}

#endif /* ACE_HAS_THREADS */

// Test event functionality.

int run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Manual_Event_Test"));

#if defined (ACE_HAS_THREADS)
  parse_args (argc, argv);

  if (ACE_Thread_Manager::instance ()->spawn_n
      (static_cast<size_t> (n_workers),
       ACE_THR_FUNC (worker),
       0,
       THR_NEW_LWP) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("spawn_n")),
                      1);

  // gives all workers chance to start
  ACE_OS::sleep (5);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("sending pulse ()\n")));

  // Release the all workers.
  if (evt.pulse () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("pulse")),
                      1);

  // Wait 2 sec
  ACE_OS::sleep (2);

  //FUZZ: disable check_for_lack_ACE_OS
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("sending signal ()\n")));
  //FUZZ: enable check_for_lack_ACE_OS

  // Signal
  if (evt.signal () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("signal")),
                      1);

  ACE_Thread_Manager::instance ()->wait ();
#else
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("Threads not supported on this platform\n")));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return test_result;
}
