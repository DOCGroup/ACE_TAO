
//=============================================================================
/**
 *  @file    Monotonic_Task_Test.cpp
 *
 *  $Id$
 *
 *    This is a test that verifies the time policy features of the
 *    ACE_Task template.
 *    A template instantiation based on the ACE_Monotonic_Time_Policy
 *    is used to demonstrate the ability for making the task message queue
 *    timeouts independent from system time changes (time shift).
 *    Also demonstrated is how to instantiate a thread manager which is time
 *    shift independent (for its wait timeouts) using time policy support for
 *    conditions.
 *
 *  @author Martin Corino <mcorino@remedy.nl>
 */
//=============================================================================


#include "test_config.h"
#include "ace/Reactor.h"
#include "ace/Timer_Queue.h"
#include "ace/Thread_Manager.h"
#include "ace/Message_Queue.h"
#include "ace/Monotonic_Time_Policy.h"
#include "ace/Synch_Traits.h"
#include "ace/Timer_Heap_T.h"
#include "ace/Event_Handler_Handle_Timeout_Upcall.h"
#include "ace/TP_Reactor.h"
#include "ace/Task_T.h"
#include "ace/Truncate.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_unistd.h"

#if defined (ACE_WIN32) || \
    (defined (_POSIX_MONOTONIC_CLOCK) && !defined (ACE_LACKS_MONOTONIC_TIME)) || \
    defined (ACE_HAS_CLOCK_GETTIME_MONOTONIC)

# if defined (ACE_WIN32)
#   include "ace/Date_Time.h"
# endif

# if defined (ACE_HAS_THREADS)

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

class MyTask : public ACE_Task<ACE_MT_SYNCH, ACE_Monotonic_Time_Policy>
{
public:
  MyTask ()
    : stop_ (false),
      tm_ (ACE_Condition_Attributes_T<ACE_Monotonic_Time_Policy> ()),
      cond_ (lock_, ACE_Condition_Attributes_T<ACE_Monotonic_Time_Policy> ()),
      status_ (0)
  {
    // set monotonic timer aware thread manager for this task
    this->thr_mgr (&this->tm_);
  }

  virtual ~MyTask () { stop (); }

  virtual int svc (void);

  int start ();

  int put_message ();

  int stop ();

  int status () { return this->status_; }

private:
  bool stop_;
  ACE_Thread_Manager tm_;
  ACE_Thread_Mutex lock_;
  ACE_Condition<ACE_Thread_Mutex> cond_;
  int status_;
};

int
MyTask::start ()
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT (" (%P|%t) Starting MyTask\n")));

  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1);

  if (this->activate (THR_NEW_LWP, 1) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p.\n"),
                       ACE_TEXT ("unable to activate thread")),
                      -1);

  if (this->cond_.wait () != 0) // wait for task to start
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Failed waiting for thread")),
                        -1);
    }

  return 0;
}

int
MyTask::put_message ()
{
  const char S1[] = "message";

  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1);

  // Now (according to task time policy = monotonic time)
  ACE_Time_Value_T<ACE_Monotonic_Time_Policy> tv (this->gettimeofday ());
  tv += ACE_Time_Value (5,0); // Now + 5 sec

  // wait for signal; should not come causing us to sleep 5 sec (monotonic)
  if (this->cond_.wait (&tv) == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%P|%t) MyTask::put_message - Received signal from thread before timeout\n")),
                        -1);
    }

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) MyTask::put_message - queueing message\n"));

  ACE_Message_Block* mb1 = new ACE_Message_Block(S1, sizeof S1);

  if (this->putq (mb1) < 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%P|%t) MyTask::put_message - Failed to queue message\n")),
                        -1);
    }

  return 0;
}

int
MyTask::stop ()
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT (" (%P|%t) Stopping MyTask\n")));

  {
    ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1);

    this->stop_ = true;

    if (this->cond_.signal () != 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Failed signalling thread\n")),
                          -1);
      }
  }

  return this->wait ();
}

int
MyTask::svc (void)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT (" (%P|%t) MyTask::svc started\n")));

  // Now (according to task time policy = monotonic time)
  ACE_Time_Value_T<ACE_Monotonic_Time_Policy> tv (this->gettimeofday ());

  {
    ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1);

    ACE_DEBUG ((LM_DEBUG, ACE_TEXT (" (%P|%t) MyTask::svc - signalling waiter\n")));

    this->cond_.signal (); // signal waiter we have started
    // waiter will shift system time back 4 sec after this which would mess
    // up the first wait for a message if we were not using monotonic time
  }

  if (!this->msg_queue ()->is_empty ())
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("New task queue is not empty!\n")));
      this->status_ = -1;
    }
  else
    {
      ACE_Message_Block *b;
      tv += ACE_Time_Value (3,0); // Now + 3 sec

      if (this->getq (b, &tv) != -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Dequeued before timeout elapsed!\n")));
          this->status_ = -1;
        }
      else if (errno != EWOULDBLOCK)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("%p\n"),
                      ACE_TEXT ("Dequeue timeout should be EWOULDBLOCK, got")));
          this->status_ = -1;
        }
      else
        {
          ACE_Time_Value_T<ACE_Monotonic_Time_Policy> tv_now (this->gettimeofday ());

          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("First getq timed out at %#T (timeout was %#T)\n"), &tv_now, &tv));

          tv =  this->gettimeofday () + ACE_Time_Value (4,0); // Now (monotonic time) + 3 sec
          if (this->getq (b, &tv) != -1)
            {
              tv_now = tv.now ();

              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("Second getq succeeded at %#T\n"), &tv_now));
              delete b;
            }
          else
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("Second getq timed out!\n")));
              this->status_ = -1;
            }
        }
    }

  {
    ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1);

    ACE_DEBUG ((LM_DEBUG, ACE_TEXT (" (%P|%t) MyTask::svc - waiting for stop\n")));

    if (!this->stop_)
      this->cond_.wait ();
  }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT (" (%t) MyTask finished\n")));
  return 0;
}

# endif /* ACE_HAS_THREADS */

int
run_main (int , ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Monotonic_Task_Test"));

  int status = 0;

# if defined (ACE_HAS_THREADS)
  MyTask my_task;

  if (my_task.start () == 0)
    {
      // shift back in time 4 sec; this would mess up timeouts if
      // monotonic timer was not used
      ACE_Time_Value tv_shift (4, 0);
      set_system_time (ACE_OS::gettimeofday () - tv_shift);

      if (my_task.put_message () == 0)
        {
          // task should now have finished dequeueing and started waiting for stop signal
          // wait (2sec) on thread manager should timeout

                                                 // use the time policy aware gettimeofday()
                                                 // method of the task to get current time
          ACE_Time_Value_T<ACE_Monotonic_Time_Policy> tv (my_task.gettimeofday ());
          tv += ACE_Time_Value (2, 0);

          // shift another 3 sec back in time; without monotonic timer support in
          // thread manager this would mess up the timed wait
          tv_shift += ACE_Time_Value (3, 0);
          set_system_time (ACE_OS::gettimeofday () - ACE_Time_Value (3,0));

          if (my_task.thr_mgr ()->wait (&tv) == 0)
            {
              ACE_ERROR ((LM_ERROR, ACE_TEXT ("Thread manager did not time out\n")));
              status = 1;
            }
          else
            {
              ACE_Time_Value_T<ACE_Monotonic_Time_Policy> tv_now (my_task.gettimeofday ());

              ACE_DEBUG ((LM_INFO, ACE_TEXT ("Thread manager timed out at %#T\n"), &tv_now));
            }
        }
      else
        status = 1;

      // ok, now stop task
      if (my_task.stop () != 0)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("Failed to stop task\n")));
          status = 1;
        }

      // restore time
      set_system_time (ACE_OS::gettimeofday () + tv_shift);
    }
  else
    status = 1;

# endif /* ACE_HAS_THREADS */

  ACE_END_TEST;
  return status;
}

#else

int
run_main (int , ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Monotonic_Task_Test"));
  ACE_DEBUG((LM_INFO,
              "(%P|%t) ACE not compiled with monotonic time.\n"));
  ACE_END_TEST;
  return 0;
}

#endif
