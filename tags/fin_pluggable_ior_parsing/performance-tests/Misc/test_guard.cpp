// $Id$

// This test program illustrates the performance of ACE_Guard and
// ACE_Thread_Mutex_Guard.

#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"
#include "ace/Profile_Timer.h"
#include "ace/Synch.h"

#if !defined (ACE_USES_OBSOLETE_GUARD_CLASSES)
#error You must compile ACE and this program with ACE_USES_OBSOLETE_GUARD_CLASSES defined!
#endif /* ACE_USES_OBSOLETE_GUARD_CLASSES */

ACE_RCSID(Misc, test_guard, "$Id$")

#if defined (ACE_HAS_THREADS)

# define ACE_THREAD_GUARD(OBJ,LOCK) \
  ACE_Thread_Mutex_Guard OBJ (LOCK); \
    if (OBJ.locked () == 0) return;

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

void thr_guard (void)
{
  ACE_THREAD_GUARD (_ace_mon, lock_);
  dummy++;
}

char *test_name[TEST_END] = { "ACE_Guard", "ACE_Thread_Mutex_Guard" };

guard_func test_function=guard;

int
main (int argc, char *argv[])
{
  ACE_Profile_Timer timer;
  ACE_Get_Opt getopt (argc, argv, "gtn:");
  int iterations = DEFAULT_ITERATIONS;
  int c, i;

  while ((c = getopt()) != -1)
    switch (c)
      {
#if defined (ACE_USES_OBSOLETE_GUARD_CLASSES)
      case 't':
        test_type = TEST_THR_GUARD;
        test_function = thr_guard;
        break;
#endif /* ACE_USES_OBSOLETE_GUARD_CLASSES */
      case 'g':
        test_type = TEST_GUARD;
        test_function = guard;
        break;
      case 'n':
        iterations = ACE_OS::atoi (getopt.optarg);
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
main (int, char *[])
{
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
  return 0;
}
#endif /* ACE_HAS_THREADS */
