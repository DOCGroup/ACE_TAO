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
#include "ace/Time_Value.h"
#include "ace/Condition_Thread_Mutex.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/High_Res_Timer.h"
#include "ace/Get_Opt.h"
#include "ace/Timer_Heap_T.h"
#include "ace/Event_Handler_Handle_Timeout_Upcall.h"
#include "ace/TP_Reactor.h"
#include "ace/Task_T.h"

#if (defined (ACE_HAS_HI_RES_TIMER) || \
     defined (ACE_HAS_AIX_HI_RES_TIMER) || \
     defined (ACE_WIN32) || \
     (defined (ghs) && defined (ACE_HAS_PENTIUM)) || \
     ((defined (__GNUG__) || defined (__INTEL_COMPILER)) && !defined(ACE_VXWORKS) && defined (ACE_HAS_PENTIUM)) || \
     (defined (linux) && defined (ACE_HAS_ALPHA_TIMER)) || \
     (defined (ACE_HAS_POWERPC_TIMER) && (defined (ghs) || defined (__GNUG__))) || \
     (defined (ACE_HAS_CLOCK_GETTIME) && defined (ACE_HAS_CLOCK_GETTIME_MONOTONIC)))

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
  MyTask (ACE_Condition_Thread_Mutex& condition): my_reactor_ (0), condition_ (condition) {}

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
  ACE_Condition_Thread_Mutex& condition_;
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

  ACE_TP_Reactor * pImpl = 0;

  ACE_NEW_RETURN (pImpl,ACE_TP_Reactor (0, create_timer_queue ()), -1);

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

int
run_main (int , ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_4055_Regression_Test"));
  int status = 0;

  ACE_Thread_Mutex mutex_;
  ACE_Condition_Thread_Mutex condition_ (mutex_);
  MyTask task1 (condition_);
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
                      1);

  ACE_Time_Value waittime = ACE_OS::gettimeofday ();
  waittime += ACE_Time_Value (3,0);

  // reset system clock 4 seconds backwords
  timespec_t curts;
  ACE_Time_Value curtime = ACE_OS::gettimeofday ();
  curtime -= ACE_Time_Value (4, 0);
  curts = curtime;
  if (ACE_OS::clock_settime (CLOCK_REALTIME, &curts) != 0)
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
          ACE_ERROR ((LM_ERROR, "(%P|%t) ERROR: timer handler shouldn't have "
          "triggered, conditions are dependent on user time!\n"));
          status = 1;
        }

      // reset system clock to correct time
      curtime = ACE_OS::gettimeofday ();
      curtime += ACE_Time_Value (4, 0);
      curts = curtime;
      ACE_OS::clock_settime (CLOCK_REALTIME, &curts);
    }

  ACE_DEBUG((LM_INFO,
              "(%P|%t) Asking worker thread to finish.\n"));
  task1.stop ();

  ACE_Thread_Manager::instance ()->wait ();
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