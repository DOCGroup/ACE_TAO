// $Id$

#include "ace/Auto_Ptr.h"

#include "Kokyu_dsrt.h"
#include "ace/Task.h"
#include "ace/Sched_Params.h"
#include "ace/Atomic_Op.h"

ACE_Atomic_Op<ACE_Thread_Mutex, long> guid=0;

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

  MyTask (ACE_Barrier& barrier,
          Kokyu::DSRT_Dispatcher<mif_scheduler_traits>* dispatcher,
          mif_scheduler_traits::QoSDescriptor_t& qos)
    :barrier_ (barrier),
     dispatcher_ (dispatcher),
     qos_ (qos),
     guid_ (++guid)
  {}

  int svc (void)
  {
    ACE_hthread_t thr_handle;
    ACE_Thread::self (thr_handle);
    int prio;

    ACE_ASSERT (dispatcher_ != 0);
    prio = dispatcher_->schedule (guid_, qos_);
    barrier_.wait ();

    if (ACE_Thread::getprio (thr_handle, prio) == -1)
      {
        if (errno == ENOTSUP)
          {
            ACE_DEBUG((LM_DEBUG,
                       ACE_TEXT ("getprio not supported on this platform\n")
                       ));
            return 0;
          }
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("getprio failed")),
                          -1);
      }

    ACE_DEBUG ((LM_DEBUG, "(%t) Thread prio=%d, guid=%d, qos_.importance=%d \n", prio, guid_, qos_.importance_));
    dispatcher_->cancel_schedule (this->guid_);
    return 0;
  }

private:
  ACE_Barrier& barrier_;
  Kokyu::DSRT_Dispatcher<mif_scheduler_traits>* dispatcher_;
  mif_scheduler_traits::QoSDescriptor_t qos_;
  mif_scheduler_traits::Guid_t guid_;
};

int main (int,char**)
{
  Kokyu::DSRT_ConfigInfo config_info;

  //  config_info.scheduler_type_ = Kokyu::SCHED_MIF;

  ACE_Barrier barrier (3);

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
  qos3.importance_ = 1;

  MyTask mytask1 (barrier, disp.get (), qos1);
  MyTask mytask2 (barrier, disp.get (), qos2);
  MyTask mytask3 (barrier, disp.get (), qos3);

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

  ACE_OS::sleep (5);

  disp->shutdown ();

  ACE_OS::sleep (5);

  ACE_DEBUG ((LM_DEBUG, "main thread exiting\n"));

  return 0;
}
