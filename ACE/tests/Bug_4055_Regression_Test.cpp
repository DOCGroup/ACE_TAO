//=============================================================================
/**
 *  @file    Bug_4055_Regression_Test.cpp
 *
 *  $Id$
 *
 *  @author Johnny Willemsen  (jwillemsen@remedy.nl)
 */
//=============================================================================

#include "test_config.h"
#include "ace/Reactor.h"
#include "ace/Timer_Queue.h"
#include "ace/Time_Value_T.h"
#include "ace/Monotonic_Time_Policy.h"
#include "ace/Condition_Thread_Mutex.h"
#include "ace/Condition_Attributes.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/High_Res_Timer.h"
#include "ace/Get_Opt.h"
#include "ace/Timer_Heap_T.h"
#include "ace/Event_Handler_Handle_Timeout_Upcall.h"
#include "ace/TP_Reactor.h"
#include "ace/Task_T.h"
#include "ace/Truncate.h"

# if defined (ACE_WIN32) || \
     (defined (_POSIX_MONOTONIC_CLOCK) && !defined (ACE_LACKS_MONOTONIC_TIME)) || \
     defined (ACE_HAS_CLOCK_GETTIME_MONOTONIC)

# if defined (ACE_HAS_THREADS)

#   if defined (ACE_WIN32)
#     include "ace/Date_Time.h"
#   endif

// Create timer queue with hr support
ACE_Timer_Queue *
create_timer_queue (void)
{
  ACE_Timer_Queue * tmq = 0;

  typedef ACE_Timer_Heap_T<ACE_Event_Handler *,
                           ACE_Event_Handler_Handle_Timeout_Upcall,
                           ACE_SYNCH_RECURSIVE_MUTEX,
                           ACE_HR_Time_Policy> timer_queue_type;
  ACE_NEW_RETURN (tmq, timer_queue_type (), 0);

  return tmq;
}

class MyTask : public ACE_Task<ACE_MT_SYNCH>
{
public:
  MyTask () : my_reactor_ (0), my_tq_ (0) {}

  virtual ~MyTask () { stop (); }

  virtual int svc (void);

  int start (int num_threads);
  int stop (void);
  ACE_Reactor* get_reactor ();
  int  create_reactor (void);

private:
  int  delete_reactor (void);

  ACE_SYNCH_RECURSIVE_MUTEX lock_;
  ACE_Reactor *my_reactor_;
  ACE_Timer_Queue *my_tq_;
};

ACE_Reactor*
MyTask::get_reactor ()
{
  return my_reactor_;
}

int
MyTask::create_reactor (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX,
                    monitor,
                    this->lock_,
                    -1);

  ACE_TEST_ASSERT (this->my_reactor_ == 0);

  this->my_tq_ = create_timer_queue ();

  ACE_TP_Reactor * pImpl = 0;
  ACE_NEW_RETURN (pImpl,ACE_TP_Reactor (0, this->my_tq_), -1);

  ACE_NEW_RETURN (my_reactor_,
                   ACE_Reactor (pImpl ,1),
                   -1);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT (" (%t) Create TP_Reactor\n")));

  this->reactor (my_reactor_);

  return 0;
}

int
MyTask::delete_reactor (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX,
                    monitor,
                    this->lock_,
                    -1);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT (" (%t) Delete TP_Reactor\n")));

  this->reactor (0);
  delete this->my_reactor_;
  this->my_reactor_ = 0;
  delete this->my_tq_;
  this->my_tq_ = 0;
  return 0;
}

int
MyTask::start (int num_threads)
{
  if (this->activate (THR_NEW_LWP, num_threads) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p.\n"),
                       ACE_TEXT ("unable to activate thread pool")),
                      -1);

  return 0;
}


int
MyTask::stop (void)
{
  if (this->my_reactor_ != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("End TP_Reactor event loop\n")));

      this->my_reactor_->end_reactor_event_loop ();
    }

  if (this->wait () == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p.\n"),
                ACE_TEXT ("unable to stop thread pool")));

  if (this->delete_reactor () == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p.\n"),
                ACE_TEXT ("unable to delete reactor")));

  return 0;
}

int
MyTask::svc (void)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT (" (%P|%t) MyTask started\n")));

  while (this->my_reactor_->reactor_event_loop_done () == 0)
    this->my_reactor_->run_reactor_event_loop ();

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT (" (%t) MyTask finished\n")));
  return 0;
}

class TestHandler
  : public ACE_Event_Handler
{
public:
  TestHandler (ACE_Reactor* reactor)
    : reactor_ (reactor),
      timeout_triggered_ (false)
  {}

  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg);

  bool trigger_in(const ACE_Time_Value &delay);

  bool timeout_triggered () { return this->timeout_triggered_; }

private:
  ACE_Reactor* reactor_;
  bool timeout_triggered_;
};

int TestHandler::handle_timeout (const ACE_Time_Value &,
                                 const void *)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) TestHandler::handle_timeout - timeout triggered\n"));
  this->timeout_triggered_ = true;
  return 0;
}

bool TestHandler::trigger_in(const ACE_Time_Value &delay)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) TestHandler::trigger_in - scheduling timer\n"));
  return -1 != reactor_->schedule_timer (this, 0, delay, ACE_Time_Value (0));
}

bool test_timer (ACE_Condition_Thread_Mutex& condition_, ACE_Time_Value& waittime, bool monotonic = false)
{
  bool status = true;
  MyTask task1;
  task1.create_reactor ();
  task1.start (1);
  TestHandler test_handler (task1.get_reactor ());

  // The second reactor that uses a hrtimer will trigger a timeout in
  // 5 seconds. At the same moment we calculate a timeout for the condition
  // 3 seconds in the future. Than we set the time 4 seconds back.
  // at the moment now the condition timeouts the trigger should have not
  // been executed. This is because with hrtime the trigger will call in 5
  // seconds and the condition in 3 seconds, independent of any time change
  // meaning that the timeout should trigger
  if (!test_handler.trigger_in (ACE_Time_Value (5, 0)))
    ACE_ERROR_RETURN ((LM_ERROR,
                        "(%P|%t) Unable to schedule trigger.\n"),
                      false);

  waittime += ACE_Time_Value (3,0);

  // reset system clock 4 seconds backwards
  ACE_Time_Value curtime = ACE_OS::gettimeofday ();
  curtime -= ACE_Time_Value (4, 0);
# if defined (ACE_WIN32)
  ACE_Date_Time curdt (curtime);
  SYSTEMTIME sys_time;
  sys_time.wDay = ACE_Utils::truncate_cast <WORD> (curdt.day ());
  sys_time.wMonth = ACE_Utils::truncate_cast <WORD> (curdt.month ());
  sys_time.wYear = ACE_Utils::truncate_cast <WORD> (curdt.year ());
  sys_time.wHour = ACE_Utils::truncate_cast <WORD> (curdt.hour ());
  sys_time.wMinute = ACE_Utils::truncate_cast <WORD> (curdt.minute ());
  sys_time.wSecond = ACE_Utils::truncate_cast <WORD> (curdt.second ());
  sys_time.wMilliseconds = ACE_Utils::truncate_cast <WORD> (curdt.microsec () / 1000);
  if (!::SetLocalTime (&sys_time))
# else
  timespec_t curts;
  curts = curtime;
  if (ACE_OS::clock_settime (CLOCK_REALTIME, &curts) != 0)
# endif
    {
      ACE_DEBUG((LM_INFO,
                  "(%P|%t) Unable to reset OS time. Insufficient privileges or not supported.\n"));
    }
  else
    {
      ACE_DEBUG((LM_INFO,
                  "(%P|%t) Going to wait on condition until %#T.\n", &waittime));
      if (condition_.wait (&waittime) != -1 || errno != ETIME)
        {
          ACE_ERROR ((LM_ERROR, "ERROR: No errno or return -1\n"));
          status = 1;
        }
      ACE_DEBUG((LM_INFO,
                  "(%P|%t) Condition wait returned at %#T.\n", &waittime));

      if (test_handler.timeout_triggered ())
        {
          if (monotonic)
            {
              ACE_ERROR ((LM_ERROR, "(%P|%t) ERROR: timer handler shouldn't have "
              "triggered because we used monotonic condition timing!\n"));
              status = false;
            }
          else
            ACE_DEBUG ((LM_INFO, "(%P|%t) timer handler "
            "triggered because we used non-monotonic condition timing!\n"));
        }
      else
        {
          if (!monotonic)
            {
              ACE_ERROR ((LM_ERROR, "(%P|%t) ERROR: timer handler should have "
              "triggered because we used non-monotonic condition timing!\n"));
              status = false;
            }
          else
            ACE_DEBUG ((LM_INFO, "(%P|%t) timer handler has not "
            "triggered because we used monotonic condition timing!\n"));
        }

      // reset system clock to correct time
      curtime = ACE_OS::gettimeofday ();
      curtime += ACE_Time_Value (4, 0);
# if defined (ACE_WIN32)
      curdt.update (curtime);
      SYSTEMTIME sys_time;
      sys_time.wDay = ACE_Utils::truncate_cast <WORD> (curdt.day ());
      sys_time.wMonth = ACE_Utils::truncate_cast <WORD> (curdt.month ());
      sys_time.wYear = ACE_Utils::truncate_cast <WORD> (curdt.year ());
      sys_time.wHour = ACE_Utils::truncate_cast <WORD> (curdt.hour ());
      sys_time.wMinute = ACE_Utils::truncate_cast <WORD> (curdt.minute ());
      sys_time.wSecond = ACE_Utils::truncate_cast <WORD> (curdt.second ());
      sys_time.wMilliseconds = ACE_Utils::truncate_cast <WORD> (curdt.microsec () / 1000);
      if (!::SetLocalTime (&sys_time))
# else
      curts = curtime;
      if (ACE_OS::clock_settime (CLOCK_REALTIME, &curts) != 0)
# endif
        {
          ACE_DEBUG((LM_INFO,
                      "(%P|%t) Unable to reset OS time. Insufficient privileges or not supported.\n"));
        }
    }

  ACE_DEBUG((LM_INFO,
              "(%P|%t) Asking worker thread to finish.\n"));
  task1.stop ();

  ACE_Thread_Manager::instance ()->wait ();

  return status;
}
# endif

int
run_main (int , ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_4055_Regression_Test"));
# if defined (ACE_HAS_THREADS)
  int status = 1;

  ACE_Thread_Mutex mutex_;
  ACE_Condition_Thread_Mutex condition_ (mutex_);
  ACE_Condition_Attributes_T<ACE_Monotonic_Time_Policy> monotonic_cond_attr_;
  ACE_Condition_Thread_Mutex monotonic_condition_ (mutex_, monotonic_cond_attr_);

  if (mutex_.acquire () != 0)
  {
    ACE_ERROR ((LM_ERROR, "(%P|%t) ERROR: Failed to acquire mutex.\n"));
  }
  else
  {
    ACE_Time_Value waittime;
    waittime = waittime.now ();
    if (test_timer (condition_, waittime))
    {
      ACE_Time_Value_T<ACE_Monotonic_Time_Policy> monotonic_waittime;
      monotonic_waittime = monotonic_waittime.now ();
      if (test_timer (monotonic_condition_, monotonic_waittime, true))
        status = 0;
    }
  }
# else
  int status = 0;
# endif
  ACE_END_TEST;
  return status;
}

#else

int
run_main (int , ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_4055_Regression_Test"));
  ACE_DEBUG((LM_INFO,
              "(%P|%t) ACE not compiled with monotonic time.\n"));
  ACE_END_TEST;
  return 0;
}

#endif
