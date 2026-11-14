// This test program illustrates the performance of ACE_GUARD

#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"
#include "ace/Profile_Timer.h"
#include "ace/Thread_Mutex.h"
#include "ace/Guard_T.h"
#include "ace/OS_NS_stdlib.h"

#if defined (ACE_HAS_THREADS)

static constexpr int DEFAULT_ITERATIONS = 100000000;

enum
{
  TEST_GUARD,
  TEST_THR_GUARD,
  TEST_END
};

static ACE_Thread_Mutex lock_ {};
using guard_func = void (*) ();
static int test_type {TEST_GUARD};
static int dummy {};

static void guard ()
{
  ACE_GUARD (ACE_Thread_Mutex, _ace_mon, lock_);
  ++dummy;
}

// FUZZ: disable check_for_ACE_Guard
static char const *const test_name[TEST_END] { "ACE_Guard" };
// FUZZ: enable check_for_ACE_Guard

static guard_func test_function {guard};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Profile_Timer timer;

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("gn:"));

  int iterations {DEFAULT_ITERATIONS};
  int c {};

  while ((c = get_opt ()) != -1)
    switch (c)
      {
      case 'g':
        test_type = TEST_GUARD;
        test_function = guard;
        break;
      case 'n':
        iterations = ACE_OS::atoi (get_opt.opt_arg ());
        if (iterations <= 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Iterations must be greater than 0\n"),
                            1);
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Invalid option\n"), -1);
      }

  ACE_DEBUG ((LM_DEBUG, "%s for iterations = %d\n", test_name[test_type], iterations));

  timer.start ();

  // Test the thread mutex (which doesn't use inheritance or dynamic
  // binding).

  for (int i {}; i < iterations; ++i)
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
