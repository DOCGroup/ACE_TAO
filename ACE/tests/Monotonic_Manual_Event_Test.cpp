
//=============================================================================
/**
 *  @file    Monotonic_Manual_Event_Test.cpp
 *
 *  $Id$
 *
 *  This test verifies the functionality of the time policy support
 *  for the <ACE_Manual_Event> implementation.
 *
 *
 *  @author Martin Corino <mcorino@remedy.nl>
 */
//=============================================================================


#include "test_config.h"
#include "ace/Manual_Event.h"
#include "ace/Thread.h"
#include "ace/Thread_Manager.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_time.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Atomic_Op.h"

#if defined (ACE_WIN32) || \
    (defined (_POSIX_MONOTONIC_CLOCK) && !defined (ACE_LACKS_MONOTONIC_TIME)) || \
    defined (ACE_HAS_CLOCK_GETTIME_MONOTONIC)

# include "ace/Monotonic_Time_Policy.h"
# if defined (ACE_WIN32)
#   include "ace/Date_Time.h"
#   include "ace/Truncate.h"
# endif

// Test results, 'success' is 0
static int test_result = 0;

#if defined (ACE_HAS_THREADS)

// Event used in the tests.  Start it "unsignalled" (i.e., its initial
// state is 0).
static ACE_Event_Base *evt = 0;

// use monotonic or system time policy
static bool use_monotonic = true;

// Number of worker threads.
static long n_workers = 10;

// Absolute timeout for initial wait of workers
static ACE_Time_Value *initial_timeout;

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
              ACE_TEXT ("usage: %n [-w n_workers] [-s]\n")));
  ACE_OS::exit (1);
}

static void
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("w:s"));

  int c;

  while ((c = get_opt ()) != -1)
    switch (c)
      {
      case 'w':
        n_workers = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      case 's':
        use_monotonic = false;
        break;
      default:
        print_usage_and_die ();
        break;
      }
}

void set_system_time(const ACE_Time_Value& tv)
{
#   if defined (ACE_WIN32)
  ACE_Date_Time curdt (tv);
  SYSTEMTIME sys_time;
  sys_time.wDay = ACE_Utils::truncate_cast <WORD> (curdt.day ());
  sys_time.wMonth = ACE_Utils::truncate_cast <WORD> (curdt.month ());
  sys_time.wYear = ACE_Utils::truncate_cast <WORD> (curdt.year ());
  sys_time.wHour = ACE_Utils::truncate_cast <WORD> (curdt.hour ());
  sys_time.wMinute = ACE_Utils::truncate_cast <WORD> (curdt.minute ());
  sys_time.wSecond = ACE_Utils::truncate_cast <WORD> (curdt.second ());
  sys_time.wMilliseconds = ACE_Utils::truncate_cast <WORD> (curdt.microsec () / 1000);
  if (!::SetLocalTime (&sys_time))
#   else
  timespec_t curts;
  curts = tv;
  if (ACE_OS::clock_settime (CLOCK_REALTIME, &curts) != 0)
#   endif
    {
      ACE_DEBUG((LM_INFO,
                  "(%P|%t) Unable to reset OS time. Insufficient privileges or not supported.\n"));
    }
}

// Worker tries to acquire the semaphore, hold it for a while, and
// then manually releases it.

static void *
worker (void *)
{
  if (evt->wait (initial_timeout) == -1)
    {
      if (ACE_OS::last_error () == ETIME)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT (" (%P|%t) Timed out waiting for start pulse\n")),
                          0);
      else
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT (" (%P|%t) %p\n"),
                           ACE_TEXT ("Failed waiting for pulse ()")),
                          0);
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT (" (%P|%t) awake\n")));

  if (++n_awoken < n_workers)
    {
      ACE_Time_Value *tv;
      ACE_Time_Value_T<ACE_Monotonic_Time_Policy> tm_mono;
      ACE_Time_Value tm_sys;

      //FUZZ: disable check_for_lack_ACE_OS
      ACE_Time_Value wait (1, 0);   // Wait 1 sec
      //FUZZ: enable check_for_lack_ACE_OS

      if (use_monotonic)
        {
          tm_mono = tm_mono.now () + wait;
          tv = &tm_mono;
        }
      else
        {
          tm_sys = ACE_OS::gettimeofday () + wait;
          tv = &tm_sys;
        }

      if (evt->wait (tv) == -1)
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
            evt->reset ();    // reset signal (rest times out)
        }

      // Check to ensure that an "infinite timeout" will work.
      if (evt->wait (0) == -1)
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
      if (evt->signal () == -1)
        ACE_ERROR ((LM_ERROR, ACE_TEXT (" (%P|%t) %p\n"), ACE_TEXT ("signal")));

      ACE_OS::sleep (ACE_Time_Value (0, 200 * 1000 * 100));  // 200 msec
    }

  if (evt->wait () == -1)
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
  ACE_START_TEST (ACE_TEXT ("Monotonic_Manual_Event_Test"));

#if defined (ACE_HAS_THREADS)
  ACE_Manual_Event_T<ACE_Monotonic_Time_Policy> monotonic_event (0);
  ACE_Manual_Event systime_event (0);
  ACE_Time_Value_T<ACE_Monotonic_Time_Policy> tm_mono;
  ACE_Time_Value tm_sys;

  parse_args (argc, argv);

  //FUZZ: disable check_for_lack_ACE_OS
  ACE_Time_Value wait (7, 0);   // Wait 7 sec
  //FUZZ: enable check_for_lack_ACE_OS

  if (use_monotonic)
    {
      evt = &monotonic_event;
      tm_mono = tm_mono.now () + wait;
      initial_timeout = &tm_mono;
    }
  else
    {
      evt = &systime_event;
      tm_sys = ACE_OS::gettimeofday () + wait;
      initial_timeout = &tm_sys;
    }

  // shift forward in time 3 sec; this will mess up timeouts if
  // monotonic timer is not used
  ACE_Time_Value tv_shift (3, 0);
  set_system_time (ACE_OS::gettimeofday () + tv_shift);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("spawning workers - initial timeout till%#T\n"), initial_timeout));

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
              ACE_TEXT ("finished waiting for workers to start\n")));

  // restore time (workers will already have failed without monotonic timer)
  set_system_time (ACE_OS::gettimeofday () - tv_shift);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("sending pulse ()\n")));

  // Release the all workers.
  if (evt->pulse () == -1)
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
  if (evt->signal () == -1)
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

#else

int
run_main (int , ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Monotonic_Manual_Event_Test"));
  ACE_DEBUG((LM_INFO,
              "(%P|%t) ACE not compiled with monotonic time.\n"));
  ACE_END_TEST;
  return 0;
}

#endif
