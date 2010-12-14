// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Recursive_Mutex_Test.cpp
//
// = DESCRIPTION
//      This test program verifies the functionality of the ACE_OS
//      implementation of recursive mutexes on Win32 and Posix
//      pthreads.
//
// = AUTHOR
//    Prashant Jain <pjain@cs.wustl.edu> and Douglas C. Schmidt <schmidt@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/Get_Opt.h"
#include "ace/Thread_Manager.h"
#include "ace/OS_NS_time.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Recursive_Thread_Mutex.h"



#if defined (ACE_HAS_THREADS)

#include "ace/Guard_T.h"

// For all platforms except for Windows use the
// ACE_Recursive_Thread_Mutex.  Since Windows only supports timed
// recursive process mutexes and not timed recursive thread mutexes,
// use ACE_Process_Mutex.
#if defined (ACE_HAS_WTHREADS)
# include "ace/Process_Mutex.h"
  typedef ACE_Process_Mutex ACE_TEST_MUTEX;
#else
# include "ace/Thread_Mutex.h"
  typedef ACE_Recursive_Thread_Mutex ACE_TEST_MUTEX;
#endif

#if !defined (ACE_HAS_MUTEX_TIMEOUTS)
static int reported_notsup = 0;
#endif /* ACE_HAS_MUTEX_TIMEOUTS */

// Total number of iterations.
static int const n_iterations = 100;
static size_t n_threads = ACE_MAX_THREADS;

// ACE_Recursive_Thread_Mutex::get_nesting_level() will return a
// meaningful value.
#if !defined (ACE_HAS_WTHREADS)
static bool nesting_level_supported = false;
#endif

static void
test_recursion_depth (int nesting_level,
                      ACE_TEST_MUTEX *rm)
{
  if (nesting_level < n_iterations)
    {
#if !defined (ACE_HAS_WTHREADS)
      // This will work for Windows, too, if ACE_TEST_MUTEX is
      // ACE_Recursive_Thread_Mutex instead of ACE_Process_Mutex.
      if (nesting_level_supported
          && nesting_level != 0
          && nesting_level != rm->get_nesting_level ())
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) Pre-mutex acquire nesting ")
                      ACE_TEXT ("levels do not match.\n")));
        }
#endif  /* !ACE_HAS_WTHREADS */
      int result = rm->acquire ();
      ACE_TEST_ASSERT (result == 0);
#if !defined (ACE_HAS_WTHREADS)
      if (nesting_level_supported
          && (nesting_level + 1) != rm->get_nesting_level ())
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) Post-mutex acquire nesting ")
                      ACE_TEXT ("levels do not match.\n")));
        }

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) = acquired, nesting = %d, thread id = %u\n"),
                  rm->get_nesting_level (),
                  rm->get_thread_id ()));
#endif /* !ACE_HAS_WTHREADS */

      test_recursion_depth (nesting_level + 1,
                            rm);

#if !defined (ACE_HAS_WTHREADS)
      if (nesting_level_supported
          && (nesting_level + 1) != rm->get_nesting_level ())
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) Post recursion nesting ")
                      ACE_TEXT ("levels do not match.\n")));
        }
#endif  /* !ACE_HAS_WTHREADS */
      result = rm->release ();
      ACE_TEST_ASSERT (result == 0);
#if !defined (ACE_HAS_WTHREADS)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) = released, nesting = %d, thread id = %u\n"),
                  rm->get_nesting_level (),
                  rm->get_thread_id ()));

      if (nesting_level_supported
          && nesting_level != 0
          && nesting_level != rm->get_nesting_level ())
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) Post-mutex release nesting ")
                      ACE_TEXT ("levels do not match.\n")));
        }
#endif /* !ACE_HAS_WTHREADS */
    }
}

static void
test_timed_wait (int nesting_level,
                 ACE_TEST_MUTEX *rm)
{
  // Make sure that we're inside of a recursive level.
  if (nesting_level == 0)
    test_timed_wait (nesting_level + 1,
                     rm);
  else
    {
      ACE_OS::srand ((u_int) ACE_OS::time (0));

      for (size_t i = 0; i < ACE_MAX_ITERATIONS / 2; i++)
        {
          int result = 0;

          // First attempt to acquire the mutex with a timeout to verify
          // that mutex timeouts are working.

          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) = trying timed acquire on ")
                      ACE_TEXT ("iteration %d\n"),
                      i));

          ACE_Time_Value delta (1, 0);  // One second timeout
          ACE_Time_Value timeout = ACE_OS::gettimeofday ();
          timeout += delta;  // Must pass absolute time to acquire().

          if (rm->acquire (timeout) != 0)
            {
              if (errno == ETIME)
                ACE_DEBUG ((LM_DEBUG,
                            ACE_TEXT ("(%P|%t) = mutex acquisition ")
                            ACE_TEXT ("timed out\n")));
              else if (errno == ENOTSUP)
                {
#if !defined (ACE_HAS_MUTEX_TIMEOUTS)
                  if (!reported_notsup)
                    {
                      ACE_DEBUG ((LM_INFO,
                                  ACE_TEXT ("(%P|%t) %p, but ACE_HAS_MUTEX_TIMEOUTS is not defined - Ok\n"),
                                  ACE_TEXT ("mutex timed acquire")));
                      reported_notsup = 1;
                    }
#else
                  ACE_DEBUG ((LM_ERROR,
                              ACE_TEXT ("(%P|%t) %p - maybe ACE_HAS_MUTEX_TIMEOUTS should not be defined?\n"),
                              ACE_TEXT ("mutex timed acquire")));
#endif  /* ACE_HAS_MUTEX_TIMEOUTS */
                }
              else
                {
                  ACE_ERROR ((LM_ERROR,
                              ACE_TEXT ("(%P|%t) %p\n%a"),
                              ACE_TEXT ("mutex timeout failed\n")));
                  return;
                }
            }
          else
            {
              result = rm->release ();
              ACE_TEST_ASSERT (result == 0);
            }

          // Now try the standard mutex.

          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) = trying to acquire on iteration %d\n"),
                      i));
          result = rm->acquire ();
          ACE_TEST_ASSERT (result == 0);
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) = acquired on iteration %d\n"),
                      i));

          // Sleep for a random amount of time between 0 and 2 seconds.
          // Note that it's ok to use rand() here because we are running
          // within the critical section defined by the Thread_Mutex.
          ACE_OS::sleep (ACE_OS::rand () % 2);

          result = rm->release ();
          ACE_TEST_ASSERT (result == 0);
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) = released on iteration %d\n"),
                      i));

          // FUZZ: disable check_for_ACE_Guard
          // Basic ACE_Guard usage - automatically acquire the mutex on
          // guard construction and automatically release it on
          // destruction.
          {
            // Construct an ACE_Guard to implicitly acquire the mutex.
            ACE_Guard<ACE_TEST_MUTEX> guard (*rm);
            ACE_TEST_ASSERT (guard.locked () != 0);

            // Perform some operation which might exit the current scope
            // prematurely, e.g. by returning or throwing an exception.
            // ...

            // ACE_Guard object is destroyed when exiting scope and guard
            // destructor automatically releases mutex.
          }

          // Use an ACE_Guard to automatically acquire a mutex, but release
          // the mutex early.
          {
            // Construct an ACE_Guard to implicitly acquire the mutex.
            ACE_Guard<ACE_TEST_MUTEX> guard (*rm);
            ACE_TEST_ASSERT (guard.locked () != 0);

            // Perform some operation which might exit the current scope
            // prematurely, e.g. by returning or throwing an exception.
            // ...

            // Release the mutex since we no longer need it.
            guard.release ();
            ACE_TEST_ASSERT (guard.locked () == 0);

            // Do something else which does not require the mutex to be locked.
            // ...

            // ACE_Guard object's destructor will not release the mutex.
          }

          // Use an ACE_Guard to automatically acquire a mutex, but
          // relinquish ownership of the lock so that the mutex is not
          // automatically released on guard destruction. This is useful
          // when an operation might not release the mutex in some
          // conditions, in which case responsibility for releasing it is
          // passed to someone else.
          {
            // Construct an ACE_Guard to implicitly acquire the mutex.
            ACE_Guard<ACE_TEST_MUTEX> guard (*rm);
            ACE_TEST_ASSERT (guard.locked () != 0);

            // Perform some operation which might exit the current scope
            // prematurely, e.g. by returning or throwing an exception.
            // ...

            // Relinquish ownership of the mutex lock. Someone else must
            // now release it.
            guard.disown ();
            ACE_TEST_ASSERT (guard.locked () == 0);

            // ACE_Guard object's destructor will not release the mutex.
        }
          // We are now responsible for releasing the mutex.
          result = rm->release ();
          ACE_TEST_ASSERT (result == 0);

          // Construct an ACE_Guard without automatically acquiring the lock.
          {
            // Construct an ACE_Guard object without automatically
            // acquiring the mutex or taking ownership of an existing
            // lock. The third parameter tells the guard that the mutex
            // has not been locked.
            ACE_Guard<ACE_TEST_MUTEX> guard (*rm, 0, 0);
            ACE_TEST_ASSERT (guard.locked () == 0);

            // Conditionally acquire the mutex.
            if (i % 2 == 0)
              {
                guard.acquire ();
                ACE_TEST_ASSERT (guard.locked () != 0);
              }

            // Perform some operation that might exit the current scope
            // prematurely, e.g. by returning or throwing an exception.
            // ...

            // ACE_Guard object is destroyed when exiting scope and guard
            // destructor automatically releases if it was acquired above.
          }

          // Use an ACE_Guard to take ownership of a previously acquired
          // mutex.
          timeout = ACE_OS::gettimeofday ();
          timeout += delta;  // Must pass absolute time to acquire().
          if (rm->acquire (timeout) == 0)
            {
              // Construct an ACE_Guard object without automatically
              // acquiring the mutex, but instead take ownership of the
              // existing lock.  The third parameter tells the guard that
              // the mutex has already been locked.
              ACE_Guard<ACE_TEST_MUTEX> guard (*rm, 0, 1);
              ACE_TEST_ASSERT (guard.locked () != 0);

              // Perform some operation which might exit the current scope
              // prematurely, e.g. by returning or throwing an exception.
              // ...

              // ACE_Guard object is destroyed when exiting scope and guard
              // destructor automatically releases mutex.
            }
          // FUZZ: enable check_for_ACE_Guard
        }

      return;
    }
}

static void *
recursion_worker (void *arg)
{
  ACE_TEST_MUTEX *rm =
    reinterpret_cast<ACE_TEST_MUTEX *> (arg);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%P|%t) Starting test of recursion depth\n")));
  test_recursion_depth (0, rm);

  return 0;
}

static void *
timed_worker (void *arg)
{
  ACE_TEST_MUTEX *rm =
    reinterpret_cast<ACE_TEST_MUTEX *> (arg);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%P|%t) Starting test of timed wait\n")));
  test_timed_wait (0, rm);

  return 0;
}

#endif /* ACE_HAS_THREADS */

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Recursive_Mutex_Test"));

#if defined (ACE_HAS_THREADS)
  if (argc > 1)
    {
      n_threads = ACE_OS::atoi (argv[1]);
    }

  ACE_TEST_MUTEX rm;

#if !defined (ACE_HAS_WTHREADS)
  // This will work for Windows, too, if ACE_TEST_MUTEX is
  // ACE_Recursive_Thread_Mutex instead of ACE_Process_Mutex.
  nesting_level_supported =
    (rm.get_nesting_level () != -1 || errno != ENOTSUP);
#endif  /* !ACE_HAS_WTHREADS */

  ACE_Thread_Manager::instance ()->spawn_n (n_threads,
                                            ACE_THR_FUNC (recursion_worker),
                                            (void *) &rm);
  ACE_Thread_Manager::instance ()->wait ();

  ACE_Thread_Manager::instance ()->spawn_n (n_threads,
                                            ACE_THR_FUNC (timed_worker),
                                            (void *) &rm);
  ACE_Thread_Manager::instance ()->wait ();
#else
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("ACE doesn't support recursive process ")
              ACE_TEXT ("mutexes on this platform\n")));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return 0;
}
