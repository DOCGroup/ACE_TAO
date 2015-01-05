
//=============================================================================
/**
 *  @file    test_timeout.cpp
 *
 *  $Id: test_timeout.cpp
 *
 *
 *  This example application shows how to write event loops that
 *  handle events for some fixed amount of time. Note that any
 *  thread in the Proactor thread pool can call back the handler. On
 *  POSIX4 systems, this test works only with POSIX_SIG_Proactor,
 *  which can work with multiple threads.
 *
 *
 *  @author Irfan Pyarali and Alexander Babu Arulanthu
 */
//=============================================================================


#include "ace/Proactor.h"
#include "ace/Task.h"
#include "ace/Atomic_Op.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_main.h"



#if defined (ACE_HAS_WIN32_OVERLAPPED_IO) || defined (ACE_HAS_AIO_CALLS)
  // This only works on Win32 platforms and on Unix platforms supporting
  // POSIX aio calls.

/**
 * @class Timeout_Handler
 *
 * @brief Generic timeout handler.
 */
class Timeout_Handler : public ACE_Handler
{
public:
  Timeout_Handler (void)
    : start_time_ (ACE_OS::gettimeofday ())
    {
    }

  virtual void handle_time_out (const ACE_Time_Value &tv,
                                const void *arg)
    {
      // Print out when timeouts occur.
      ACE_DEBUG ((LM_DEBUG, "(%t) %d timeout occurred for %s @ %d.\n",
                  ++count_,
                  (char *) arg,
                  (tv - this->start_time_).sec ()));

      // Sleep for a while
      ACE_OS::sleep (4);
    }

private:
  /// Number of the timer event.
  ACE_Atomic_Op <ACE_SYNCH_MUTEX, int> count_;

  /// Starting time of the test.
  ACE_Time_Value start_time_;
};

class Worker : public ACE_Task <ACE_NULL_SYNCH>
{
public:
  int svc (void)
    {
      // Handle events for 13 seconds.
      ACE_Time_Value run_time (13);

      ACE_DEBUG ((LM_DEBUG, "(%t):Starting svc routine\n"));

      if (ACE_Proactor::run_event_loop(run_time) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "(%t):%p.\n", "Worker::svc"), -1);

      ACE_DEBUG ((LM_DEBUG, "(%t) work complete\n"));

      return 0;
    }
};

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  Timeout_Handler handler;

  // Register a 2 second timer.
  ACE_Time_Value foo_tv (2);
  if (ACE_Proactor::instance ()->schedule_timer (handler,
                                                 (void *) "Foo",
                                                 ACE_Time_Value::zero,
                                                 foo_tv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "schedule_timer"), -1);

  // Register a 3 second timer.
  ACE_Time_Value bar_tv (3);
  if (ACE_Proactor::instance ()->schedule_timer (handler,
                                                 (void *) "Bar",
                                                 ACE_Time_Value::zero,
                                                 bar_tv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "schedule_timer"), -1);

  Worker worker;

  if (worker.activate (THR_NEW_LWP, 10) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p.\n", "main"), -1);

  ACE_Thread_Manager::instance ()->wait ();

  return 0;
}

#else /* ACE_HAS_WIN32_OVERLAPPED_IO || ACE_HAS_AIO_CALLS */

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_DEBUG ((LM_DEBUG,
              "This example is multithreaded version of test_timeout_st.cpp\n"
              "This doesnt work on this platform !!!\n"));
  return 1;
}

#endif /*  ACE_HAS_WIN32_OVERLAPPED_IO || ACE_HAS_AIO_CALLS */
