// $Id$

#include "ace/Auto_Ptr.h"

#include "Kokyu.h"
#include "ace/Task.h"
#include "ace/Sched_Params.h"

class MyTask : public ACE_Task_Base
{
public:

  MyTask (ACE_Barrier& barrier,
          Kokyu::DSRT_Dispatcher* dispatcher,
          Kokyu::QoSDescriptor& qos)
    :barrier_ (barrier),
     dispatcher_ (dispatcher),
     qos_ (qos)
  {}

  int svc (void)
  {
    dispatcher_->schedule (0, qos_);
    barrier_.wait ();

    ACE_hthread_t thr_handle;
    ACE_Thread::self (thr_handle);
    int prio;

    if (ACE_Thread::getprio (thr_handle, prio) == -1)
      {
        if (errno == ENOTSUP)
          {
            ACE_DEBUG((LM_DEBUG,
                       ACE_TEXT ("getprior not supported on this platform\n")
                       ));
            return 0;
          }
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("getprio failed")),
                          -1);
      }

    ACE_DEBUG ((LM_DEBUG, "(%t) Thread prio is %d\n", prio));
    return 0;
  }

private:
  ACE_Barrier& barrier_;
  Kokyu::DSRT_Dispatcher* dispatcher_;
  Kokyu::QoSDescriptor& qos_;
};

int main (int,char**)
{
  Kokyu::DSRT_ConfigInfo config_info;

  //  config_info.scheduler_type_ = Kokyu::SCHED_MIF;

  ACE_Barrier barrier (3);

  ACE_DEBUG ((LM_DEBUG, "before create_dispatcher\n" ));
  auto_ptr<Kokyu::DSRT_Dispatcher>
    disp (Kokyu::Dispatcher_Factory::create_DSRT_dispatcher (config_info));

  ACE_ASSERT (disp.get () != 0);

  Kokyu::QoSDescriptor qos1, qos2, qos3;

  qos1.importance_ = Kokyu::LOW_IMPORTANCE;
  qos2.importance_ = Kokyu::HIGH_IMPORTANCE;
  qos3.importance_ = Kokyu::VERY_HIGH_IMPORTANCE;

  MyTask mytask1 (barrier, disp.get (), qos1);
  MyTask mytask2 (barrier, disp.get (), qos2);
  MyTask mytask3 (barrier, disp.get (), qos3);

  long flags = THR_BOUND | THR_SCHED_FIFO;

  mytask1.activate(flags);
  mytask2.activate(flags);
  mytask3.activate(flags);

  while(1){}

  return 0;
}
