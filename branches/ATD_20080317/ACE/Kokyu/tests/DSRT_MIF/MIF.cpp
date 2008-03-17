// $Id$

#include "ace/ACE.h"
#include "ace/Auto_Ptr.h"
#include "ace/Task.h"
#include "ace/Sched_Params.h"
#include "ace/Atomic_Op.h"
#include "ace/High_Res_Timer.h"
#include "ace/Barrier.h"
#include "ace/Lock_Adapter_T.h"
#include "ace/Countdown_Time.h"

#include "Kokyu_dsrt.h"

ACE_Atomic_Op<ACE_SYNCH_MUTEX, long> guid=0;

struct mif_scheduler_traits
{
  typedef int Guid_t;

  struct QoSDescriptor_t
  {
    typedef long Importance_t;

    long importance_;
  };

  /*
  static Time_t now()
  {
    ACE_Time_Value now = ACE_OS::gettimeofday ();
    return now.sec () * 10000000 + now.usec () * 10;
  }
  */

  typedef Kokyu::MIF_Comparator<QoSDescriptor_t> QoSComparator_t;

  struct Guid_Hash
  {
    u_long operator () (const Guid_t& guid)
    {
      return guid;
    }
  };
};


class MyTask : public ACE_Task_Base
{
public:

  MyTask (ACE_Barrier& bar,
          Kokyu::DSRT_Dispatcher<mif_scheduler_traits>* dispatcher,
          mif_scheduler_traits::QoSDescriptor_t& qos,
          int exec_duration)
    :barrier_ (bar),
     dispatcher_ (dispatcher),
     qos_ (qos),
     guid_ (++guid),
     exec_duration_ (exec_duration)
  {}

  int svc (void);

 private:
  ACE_Barrier& barrier_;
  Kokyu::DSRT_Dispatcher<mif_scheduler_traits>* dispatcher_;
  mif_scheduler_traits::QoSDescriptor_t qos_;
  mif_scheduler_traits::Guid_t guid_;
  int exec_duration_;
};

int MyTask::svc (void)
{
  ACE_hthread_t thr_handle;
  ACE_Thread::self (thr_handle);

  ACE_DEBUG ((LM_DEBUG, "(%t|%T): task activated\n"));
  ACE_ASSERT (dispatcher_ != 0);

  (void) dispatcher_->schedule (guid_, qos_);

  barrier_.wait ();

  long prime_number = 9619899;

  ACE_High_Res_Timer timer;
  ACE_Time_Value elapsed_time;
  ACE_Time_Value seconds_tracker(0,0);

  ACE_Time_Value one_second (1,0);
  ACE_Time_Value compute_count_down_time (exec_duration_, 0);
  ACE_Countdown_Time compute_count_down (&compute_count_down_time);

  timer.start ();
  while (compute_count_down_time > ACE_Time_Value::zero)
    {
      ACE::is_prime (prime_number,
                     2,
                     prime_number / 2);

      compute_count_down.update ();
      timer.stop ();
      timer.elapsed_time (elapsed_time);
      seconds_tracker += elapsed_time;
      if (seconds_tracker >= one_second)
      {
        seconds_tracker.set (0,0);
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%t) Currently running guid=%d")
                    ACE_TEXT (", qos_.importance=%d \n"),
                    guid_, qos_.importance_));
      }
      timer.reset ();
      timer.start ();
    }

  dispatcher_->cancel_schedule (this->guid_);
  return 0;
}

int ACE_TMAIN (int,ACE_TCHAR**)
{
  Kokyu::DSRT_ConfigInfo config_info;

  //  config_info.scheduler_type_ = Kokyu::SCHED_MIF;
  config_info.impl_type_ = Kokyu::DSRT_OS_BASED;

  ACE_Barrier bar (3);

  ACE_DEBUG ((LM_DEBUG, "before create_dispatcher\n" ));

  config_info.sched_strategy_ = Kokyu::DSRT_MIF;

  Kokyu::DSRT_Dispatcher_Factory<mif_scheduler_traits>::DSRT_Dispatcher_Auto_Ptr
    disp (Kokyu::DSRT_Dispatcher_Factory<mif_scheduler_traits>::
          create_DSRT_dispatcher (config_info));

  ACE_DEBUG ((LM_DEBUG, "after create_dispatcher\n" ));

  ACE_ASSERT (disp.get () != 0);

  mif_scheduler_traits::QoSDescriptor_t qos1, qos2, qos3;

  qos1.importance_ = 1;
  qos2.importance_ = 2;
  qos3.importance_ = 3;

  MyTask mytask1 (bar, disp.get (), qos1, 15);
  MyTask mytask2 (bar, disp.get (), qos2, 6);
  MyTask mytask3 (bar, disp.get (), qos3, 4);

  long flags = THR_BOUND | THR_SCHED_FIFO;

  if (mytask1.activate (flags) == -1)
    {
      flags = THR_BOUND;
      if (mytask1.activate (flags) == -1)
        ACE_ERROR ((LM_ERROR,
                        "EC (%P|%t) cannot activate task\n"));
    }

  if (mytask2.activate (flags) == -1)
    {
      flags = THR_BOUND;
      if (mytask2.activate (flags) == -1)
        ACE_ERROR ((LM_ERROR,
                        "EC (%P|%t) cannot activate task\n"));
    }

  if (mytask3.activate (flags) == -1)
    {
      flags = THR_BOUND;
      if (mytask3.activate (flags) == -1)
        ACE_ERROR ((LM_ERROR,
                        "EC (%P|%t) cannot activate task\n"));
    }

  disp->shutdown ();

  ACE_DEBUG ((LM_DEBUG, "main thread exiting\n"));

  return 0;
}

