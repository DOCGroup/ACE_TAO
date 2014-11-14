
//=============================================================================
/**
 *  @file    Monotonic_Message_Queue_Test.cpp
 *
 *  $Id$
 *
 *    This is a test that verifies the time policy features of the
 *    ACE_Message_Queue template.
 *    A template instantiation based on the ACE_Monotonic_Time_Policy
 *    is used to demonstrate the ability for making the message queue
 *    timeouts independent from system time changes (time shift).
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
#include "ace/OS_NS_time.h"
#include "ace/OS_NS_unistd.h"

#if defined (ACE_WIN32) || \
    (defined (ACE_HAS_CLOCK_GETTIME) && \
     !defined (ACE_LACKS_MONOTONIC_TIME) && \
     !defined (ACE_LACKS_CONDATTR) && \
     (defined (_POSIX_MONOTONIC_CLOCK) || defined (ACE_HAS_CLOCK_GETTIME_MONOTONIC)) && \
     defined (_POSIX_CLOCK_SELECTION) && !defined (ACE_LACKS_CONDATTR_SETCLOCK))

# if defined (ACE_WIN32)
#   include "ace/Date_Time.h"
# endif

# if defined (ACE_HAS_THREADS)
typedef ACE_Message_Queue<ACE_MT_SYNCH, ACE_Monotonic_Time_Policy> SYNCH_QUEUE;

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
  TestHandler (ACE_Reactor* reactor, SYNCH_QUEUE &mq)
    : reactor_ (reactor),
      mq_ (mq)
  {}

  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg);

  bool trigger_in(const ACE_Time_Value &delay);

private:
  ACE_Reactor* reactor_;
  SYNCH_QUEUE& mq_;
};

int TestHandler::handle_timeout (const ACE_Time_Value &,
                                 const void *)
{
  const char S1[] = "message";

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) TestHandler::handle_timeout - timeout triggered\n"));

  ACE_Message_Block* mb1 = new ACE_Message_Block(S1, sizeof S1);

  this->mq_.enqueue_tail (mb1);

  return 0;
}

bool TestHandler::trigger_in(const ACE_Time_Value &delay)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) TestHandler::trigger_in - scheduling timer\n"));
  return -1 != reactor_->schedule_timer (this, 0, delay, ACE_Time_Value (0));
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

// Ensure that the timedout dequeue_head() keeps working in case of timeshift when using monotonic timer.

static bool
timeout_test (void)
{
  bool status = true;
  SYNCH_QUEUE mq;
  MyTask task1;
  task1.create_reactor ();
  task1.start (1);
  TestHandler test_handler (task1.get_reactor (), mq);

  // The reactor of taks1 that uses a hrtimer will trigger a timeout in
  // 5 seconds which will enqueue a message block in the queue. At the
  // same moment we calculate a timeout for the dequeue operation for
  // 3 seconds in the future. Than we set the system time 4 seconds back.
  // The condition should timeout because the queue is empty and the trigger
  // only fires after the condition has timed out.
  // Next we start another dequeue for 3 seconds in the future which should
  // return before timing out because by then the trigger should have fired.
  // In case of using regular system time policy for message queue and
  // dequeue timeouts the first dequeue would not have timed out because
  // between calculating the timeout and starting the dequeue the system time
  // shifted back 4 sec causing the trigger to fire before the timeout elapsed.
  // In case timeshifting does not work because of priority problems or such
  // the test should succeed.

  if (!test_handler.trigger_in (ACE_Time_Value (5, 0)))
    ACE_ERROR_RETURN ((LM_ERROR,
                        "(%P|%t) Unable to schedule trigger.\n"),
                      false);

  if (!mq.is_empty ())
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("New queue is not empty!\n")));
      status = false;
    }
  else
    {
      ACE_Message_Block *b;
      ACE_Time_Value_T<ACE_Monotonic_Time_Policy> tv;
      tv = (tv.now () + ACE_Time_Value (3,0)); // Now (monotonic time) + 3 sec

      // shift back in time 4 sec
      set_system_time (ACE_OS::gettimeofday () - ACE_Time_Value (4, 0));

      if (mq.dequeue_head (b, &tv) != -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Dequeued before timeout elapsed!\n")));
          status = false;
        }
      else if (errno != EWOULDBLOCK)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("%p\n"),
                      ACE_TEXT ("Dequeue timeout should be EWOULDBLOCK, got")));
          status = false;
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("First dequeue timed out: OK\n")));

          tv = (tv.now () + ACE_Time_Value (3,0)); // Now (monotonic time) + 3 sec
          if (mq.dequeue_head (b, &tv) != -1)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("Second dequeue succeeded: OK\n")));
              delete b;
            }
          else
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("Second dequeue timed out!\n")));
              status = false;
            }
        }

      // restore time
      set_system_time (ACE_OS::gettimeofday () + ACE_Time_Value (4, 0));
    }

  ACE_DEBUG((LM_INFO,
              "(%P|%t) Asking worker thread to finish.\n"));
  task1.stop ();

  ACE_Thread_Manager::instance ()->wait ();

  return status;
}
# endif /* ACE_HAS_THREADS */

int
run_main (int , ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Monotonic_Message_Queue_Test"));

  int status = 0;

# if defined (ACE_HAS_THREADS)
  if (!timeout_test ())
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%p\n"),
                  ACE_TEXT ("test failed")));
      status = 1;
    }
# endif /* ACE_HAS_THREADS */

  ACE_END_TEST;
  return status;
}

#else

int
run_main (int , ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Monotonic_Message_Queue_Test"));
  ACE_DEBUG((LM_INFO,
              "(%P|%t) ACE not compiled with monotonic time.\n"));
  ACE_END_TEST;
  return 0;
}

#endif
