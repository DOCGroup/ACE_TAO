// $Id$

#include "ace/Auto_Ptr.h"

#include "Kokyu.h"
#include "ace/Task.h"
#include "ace/Sched_Params.h"

class MyCommand : public Kokyu::Dispatch_Command
{
public:
  MyCommand(int i)
    :Kokyu::Dispatch_Command(1),id_(i)
  {
  }

  int execute()
  {
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

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT (("(%t|prio=%d) | command %d executed\n")),
                prio, id_));
    return 0;
  }

private:
  int id_;
};


int main (int,char**)
{
  Kokyu::ConfigInfoSet config_info(3);

  Kokyu::Priority_t min_prio =
    ACE_Sched_Params::priority_min (ACE_SCHED_FIFO);

  config_info[0].preemption_priority_ = 1;
  config_info[0].thread_priority_ = min_prio;
  config_info[0].dispatching_type_ = Kokyu::DEADLINE_DISPATCHING;

  ACE_DEBUG ((LM_DEBUG, "before create_dispatcher\n" ));
  auto_ptr<Kokyu::Dispatcher>
    disp (Kokyu::Dispatcher_Factory::create_dispatcher (config_info));

  ACE_ASSERT (disp.get() != 0);

  MyCommand cmd1(1), cmd2(2), cmd3(3);

  Kokyu::QoSDescriptor qos1, qos2, qos3;

  // Get the current time.
  ACE_Time_Value current_time = ACE_OS::gettimeofday ();

  ACE_Time_Value deadline1, deadline2, deadline3;

  deadline1 = current_time + ACE_Time_Value(150,0);
  deadline2 = current_time + ACE_Time_Value(200,0);
  deadline3 = current_time + ACE_Time_Value(100,0);

  qos1.preemption_priority_ = 1;
  qos1.deadline_ = deadline1;
  qos2.preemption_priority_ = 1;
  qos2.deadline_ = deadline2;
  qos3.preemption_priority_ = 1;
  qos3.deadline_ = deadline3;

  disp->dispatch (&cmd1, qos1);
  printf("inserted 1\n");
  disp->dispatch (&cmd2, qos2);
  printf("inserted 2\n");
  disp->dispatch (&cmd3, qos3);
  printf("inserted 3\n");

  while(1){}

  return 0;
}
