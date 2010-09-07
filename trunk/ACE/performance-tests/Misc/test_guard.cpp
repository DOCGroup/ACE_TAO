// $Id$

// This test program illustrates the performance of ACE_GUARD

#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"
#include "ace/Profile_Timer.h"

ACE_RCSID(Misc, test_guard, "$Id$")

#if defined (ACE_HAS_THREADS)

static const int DEFAULT_ITERATIONS = 100000000;

enum
{
  TEST_GUARD,
  TEST_THR_GUARD,
  TEST_END
};

ACE_Thread_Mutex lock_;
typedef void (*guard_func)(void);
int test_type = TEST_GUARD;
int dummy = 0;

void guard (void)
{
  ACE_GUARD (ACE_Thread_Mutex, _ace_mon, lock_);
  dummy++;
}

// FUZZ: disable check_for_ACE_Guard
char *test_name[TEST_END] = { "ACE_Guard" };
// FUZZ: enable check_for_ACE_Guard

guard_func test_function=guard;

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  ACE_Profile_Timer timer;

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("gn:"));

  int iterations = DEFAULT_ITERATIONS;
  int c, i;

  while ((c = get_opt()) != -1)
    switch (c)
      {
      case 'g':
        test_type = TEST_GUARD;
        test_function = guard;
        break;
      case 'n':
        iterations = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Invalid option\n"), -1);
      }

  ACE_DEBUG ((LM_DEBUG, "%s for iterations = %d\n", test_name[test_type], iterations));

  timer.start ();

  // Test the thread mutex (which doesn't use inheritance or dynamic
  // binding).

  for (i = 0; i < iterations; i++)
    test_function ();

  timer.stop ();

  ACE_Profile_Timer::ACE_Elapsed_Time et;

  timer.elapsed_time (et);

  ACE_DEBUG ((LM_DEBUG, "Thread_Mutex\n"));
  ACE_DEBUG ((LM_DEBUG, "real time = %f secs, user time = %f secs, system time = %f secs\n",
              et.real_time, et.user_time, et.system_time));

  ACE_DEBUG ((LM_DEBUG, "time per call = %f usecs\n",
              (et.real_time / double (iterations)) * 1000000));

  return 0;
}
#else
int
ACE_TMAIN(int, ACE_TCHAR *[])
{
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
  return 0;
}
#endif /* ACE_HAS_THREADS */
