// $Id$

#include "tao/corba.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "ace/Thread_Manager.h"
#include "ace/High_Res_Timer.h"
#include "ace/Get_Opt.h"

static int test_try_lock_flag =
#if defined (ACE_HAS_MUTEX_TIMEOUTS) && !defined (ACE_HAS_WTHREADS)
1;
#else
// Don't test try_lock timed waits unless the underlying OS supports it
// Windows is the exception.  It supports some mutex timeouts, so
// it has ACE_HAS_MUTEX_TIMEOUTS defined, but it doesn't support
// thread mutex timeouts which is what is needed for this to work.
0;
#endif /* defined (ACE_HAS_MUTEX_TIMEOUTS) && !defined (ACE_HAS_WTHREADS) */

// Parse command-line arguments.

static int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "t");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 't':
        test_try_lock_flag = 0;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-t"
                           "\n",
                           argv [0]),
                          -1);
      }

  return 0;
}

static int
check_for_nil (CORBA::Object_ptr obj, const char *msg)
{
  if (CORBA::is_nil (obj))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ERROR: Object reference <%s> is nil\n",
                       msg),
                      -1);
  else
    return 0;
}

static int
test_mutex_simple (RTCORBA::RTORB_ptr rt_orb)
{
  // Test the basic interface of the RTCORBA::Mutex This test should
  // run even on platforms without thread support.
  ACE_TRY_NEW_ENV
    {
      RTCORBA::Mutex_var my_mutex;

      my_mutex = rt_orb->create_mutex (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      my_mutex->lock (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      my_mutex->unlock (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      my_mutex->lock (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      my_mutex->unlock (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      rt_orb->destroy_mutex (my_mutex.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Unexpected exception caught in test_mutex_simple()");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

#if (TAO_HAS_NAMED_RT_MUTEXES == 1)
static int
test_named_mutex_simple (RTCORBA::RTORB_ptr rt_orb)
{
  // Test the basic interface of the named RTCORBA::Mutex(es) This
  // test should run even on platforms without thread support.
  ACE_TRY_NEW_ENV
    {
      RTCORBA::Mutex_var larry_mutex1;
      RTCORBA::Mutex_var moe_mutex1;
      CORBA::Boolean created_flag;

      larry_mutex1 = rt_orb->create_named_mutex ("larry",
                                                 created_flag ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (created_flag != 1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ERROR: Expected named mutex larry to be created, but it wasn't\n"),
                          -1);

      moe_mutex1 = rt_orb->create_named_mutex ("moe",
                                               created_flag
                                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (created_flag != 1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ERROR: Expected named mutex moe to be created, but it wasn't\n"),
                          -1);

      larry_mutex1->lock (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      larry_mutex1->unlock (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Test creating the mutex a second time
      {
        RTCORBA::Mutex_var larry_mutex2;
        larry_mutex2 = rt_orb->create_named_mutex ("larry",
                                                   created_flag
                                                   ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        if (created_flag != 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "ERROR: Expected named mutex to already be created, but it wasn't\n"),
                            -1);

        // test the pointers...
        if (ACE_reinterpret_cast (void *, larry_mutex1.in ())
            != ACE_reinterpret_cast (void *, larry_mutex2.in ()))
          ACE_ERROR_RETURN ((LM_ERROR,
                             "ERROR: Should have gotten the same mutex, but didn't\n"),
                            -1);

        larry_mutex2->lock (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;

        larry_mutex2->unlock (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }

      // test opening the mutex
      {
        RTCORBA::Mutex_var larry_mutex3;
        larry_mutex3 = rt_orb->open_named_mutex ("larry"
                                                 ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        // test the pointers...
        if (ACE_reinterpret_cast (void *,larry_mutex1.in ())
            != ACE_reinterpret_cast (void *,larry_mutex3.in ()))
          ACE_ERROR_RETURN ((LM_ERROR,
                             "ERROR: Should have gotten the same mutex, but didn't\n"),
                            -1);

        larry_mutex3->lock (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;

        larry_mutex3->unlock (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }

      // Make sure that nothing has been broken behind the scenes.
      larry_mutex1->lock (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      larry_mutex1->unlock (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      rt_orb->destroy_mutex (larry_mutex1.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      rt_orb->destroy_mutex (moe_mutex1.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Unexpected exception caught in test_named_mutex_simple()");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

static int
test_named_mutex_exception (RTCORBA::RTORB_ptr rt_orb)
{
  // Test that open_named_mutex returns an exception when the mutex
  // name isn't found.

  // This test should run even on platforms without thread support.
  ACE_TRY_NEW_ENV
    {
      RTCORBA::Mutex_var larry_mutex1;

      larry_mutex1 = rt_orb->open_named_mutex ("larry" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_ERROR_RETURN ((LM_ERROR,
                         "Expected a MutexNotFound exception, but didn't get one.\n"),
                        -1);
    }
  ACE_CATCH (RTCORBA::RTORB::MutexNotFound, ex)
    {
      ACE_DEBUG ((LM_DEBUG, "Caught expected MutexNotFound exception.\n"));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Unexpected exception caught in test_named_mutex_exception()");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
#endif /* TAO_HAS_NAMED_RT_MUTEXES == 1 */

#if defined (ACE_HAS_THREADS)
const size_t MAX_ITERATIONS=10;
const size_t MAX_THREADS=4;

struct Mutex_Test_Data
{
  RTCORBA::Mutex_ptr mutex;
  int *shared_var;
  int *error_flag;
};

static void *
mutex_test_thread (void *args)
{
  Mutex_Test_Data *data = ACE_reinterpret_cast (Mutex_Test_Data *, args);

  RTCORBA::Mutex_ptr mutex = data->mutex;
  int *shared_var = data->shared_var;

  ACE_OS::srand (ACE_OS::time (0));

  ACE_TRY_NEW_ENV
    {
      for (size_t i = 0; i < MAX_ITERATIONS / 2; i++)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) = trying to lock on iteration %d\n"),
                      i));
          mutex->lock ();
          ACE_TRY_CHECK;

          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) = locked on iteration %d\n"),
                      i));

          // Check if the shared var is a value it shouldn't be when
          // we're under the lock.
          if (*shared_var != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "Expected shared_var to be 0 under the mutex\n"));
              *data->error_flag = 1;
            }

          *shared_var = 1;

          // Sleep for a random amount of time between 0 and 2
          // seconds.  Note that it's ok to use rand() here because we
          // are running within the critical section defined by the
          // Thread_Mutex.
          ACE_OS::sleep (ACE_OS::rand () % 2);

          if (*shared_var != 1)
            {
              ACE_ERROR ((LM_ERROR,
                          "Expected shared_var to still be 1 after sleeping\n"));
              *data->error_flag = 1;
            }

          *shared_var = 0;

          mutex->unlock ();
          ACE_TRY_CHECK;

          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) = unlocked on iteration %d\n"),
                      i));
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Unexpected exception caught in mutex_test_thread()");
      *data->error_flag = 1;
    }
  ACE_ENDTRY;

  return 0;
}

static int
test_mutex_threads (RTCORBA::RTORB_ptr rt_orb)
{
  // test the RTCORBA::Mutex implementation be spawning many threads
  // that repeatedly content for a lock.  This code is based on the
  // tests/Thread_Mutex_Test code.

  Mutex_Test_Data test_data;

  const u_int n_threads =
#if defined (__Lynx__)
    3;  /* It just doesn't work with 4 threads. (Advice from Thread_Mutex_Test.cpp) */
#else  /* ! __Lynx__ */
  MAX_THREADS;
#endif /* ! __Lynx__ */

  int shared_var = 0;
  int error_flag = 0;

  ACE_TRY_NEW_ENV
    {
      RTCORBA::Mutex_ptr mutex = rt_orb->create_mutex (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test_data.mutex = mutex;
      test_data.shared_var = &shared_var;
      test_data.error_flag = &error_flag;

      if (ACE_Thread_Manager::instance ()->spawn_n (n_threads,
                                                    ACE_THR_FUNC (mutex_test_thread),
                                                    (void *) &test_data,
                                                    THR_NEW_LWP | THR_DETACHED) == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("%p\n%a"),
                    ACE_TEXT ("thread create failed")));

      // Wait for the threads to exit.
      ACE_Thread_Manager::instance ()->wait ();

      CORBA::release (mutex);

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Unexpected exception caught in test_mutex_threads()");
      return -1;
    }
  ACE_ENDTRY;

  return error_flag;
}

static void *
mutex_test_try_lock_thread (void *args)
{
  // test out try_lock() failure cases
  Mutex_Test_Data *data = ACE_reinterpret_cast (Mutex_Test_Data *, args);

  RTCORBA::Mutex_ptr mutex = data->mutex;
  CORBA::Boolean result;

  ACE_TRY_NEW_ENV
    {
      // check that try_lock (0) returns false
      ACE_DEBUG ((LM_DEBUG,"attempting try_lock (0) - expect failure (but no exceptions) \n"));
      result = mutex->try_lock (0u ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (result)
        {
          ACE_ERROR ((LM_ERROR,
                      "try_lock succeeded, but expected a failure\n"));
          *data->error_flag = 1;
        }

      if (test_try_lock_flag)
        {
          ACE_High_Res_Timer timer;

          // Check that try_lock (timeout) returns false (and times
          // out)
          ACE_DEBUG ((LM_DEBUG,
                      "attempting try_lock (5 sec) - expect failure after 5 secs (but no exceptions)\n"));

          timer.start ();
          result = mutex->try_lock (50000000u /*5sec*/ ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          timer.stop ();

          if (result)
            {
              ACE_ERROR ((LM_ERROR,
                          "try_lock (timeout) succeeded, but expected a failure\n"));
              *data->error_flag = 1;
            }

          ACE_Time_Value measured;
          timer.elapsed_time (measured);
          ACE_DEBUG ((LM_DEBUG,
                      "try_lock returned after %u secs, %u usecs\n",
                      measured.sec(),
                      measured.usec()));

          if ((measured.sec() == 4 && measured.usec() >= 500000)
              || (measured.sec() == 5 && measured.usec() <= 500000))
            /* success */;
          else
            {
              ACE_ERROR ((LM_ERROR,
                          "try_lock timed out not as expected\n"));
              *data->error_flag = 1;
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Unexpected exception caught in mutex_test_try_lock_thread()");
      *data->error_flag = 1;
    }
  ACE_ENDTRY;

  return 0;
}

static int
test_mutex_try_lock (RTCORBA::RTORB_ptr rt_orb)
{
  Mutex_Test_Data test_data;
  CORBA::Boolean result;

  int shared_var = 0;
  int error_flag = 0;

  ACE_TRY_NEW_ENV
    {
      RTCORBA::Mutex_ptr mutex = rt_orb->create_mutex (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Test out try_lock and keep the lock so that the spawned task
      // can test out try_lock failure cases
      result = mutex->try_lock (0u ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (!result)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "try_lock failed\n"),
                          -1);

      test_data.mutex = mutex;
      test_data.shared_var = &shared_var;
      test_data.error_flag = &error_flag;

      ACE_DEBUG ((LM_DEBUG,
                  "Spawning the test thread\n"));
      if (ACE_Thread_Manager::instance ()->spawn (ACE_THR_FUNC (mutex_test_try_lock_thread),
                                                  (void *) &test_data,
                                                  THR_NEW_LWP | THR_DETACHED) == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("%p\n%a"),
                    ACE_TEXT ("thread create failed")));

      // Wait for the threads to exit.
      ACE_Thread_Manager::instance ()->wait ();

      CORBA::release (mutex);

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Unexpected exception caught in test_mutex_try_lock()");
      return -1;
    }
  ACE_ENDTRY;

  return error_flag;
}

#endif /* ACE_HAS_THREADS */

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      // ORB.
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Parse arguments.
      if (parse_args (argc, argv) != 0)
        return -1;

      // RTORB.
      CORBA::Object_var object =
        orb->resolve_initial_references ("RTORB" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      RTCORBA::RTORB_var rt_orb = RTCORBA::RTORB::_narrow (object.in ()
                                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (check_for_nil (rt_orb.in (), "RTORB") == -1)
        return -1;

      ACE_DEBUG ((LM_DEBUG,
                  "Running RTCORBA Mutex unit tests\n"));

      if (test_mutex_simple (rt_orb.in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "test_mutex_simple failed\n"),
                          -1);

#if (TAO_HAS_NAMED_RT_MUTEXES == 1)
      if (test_named_mutex_simple (rt_orb.in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "test_named_mutex_simple failed\n"),
                          -1);

      if (test_named_mutex_exception (rt_orb. in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "test_named_mutex_exception failed\n"),
                          -1);
#else
      ACE_DEBUG ((LM_DEBUG,
                  "Named RT_Mutex support is not enabled. "
                  "Skipping Named RT_Mutex tests.\n"));
#endif /* TAO_HAS_NAMED_RT_MUTEXES == 1 */

#if defined (ACE_HAS_THREADS)

      if (test_mutex_threads (rt_orb.in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "test_mutex_threads failed\n"),
                          -1);
      else if (test_mutex_try_lock (rt_orb.in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "test_mutex_try_lock failed\n"),
                          -1);

#endif /* ACE_HAS_THREADS */

      ACE_DEBUG ((LM_DEBUG, "Mutex test finished\n\n"));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Unexpected exception caught in Mutex test server:");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
