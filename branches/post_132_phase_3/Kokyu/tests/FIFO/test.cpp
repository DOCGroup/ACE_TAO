// $Id$

#include "ace/Auto_Ptr.h"

#include "Kokyu.h"
#include "ace/Task.h"

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

  config_info[0].preemption_priority_ = 1;
  config_info[0].thread_priority_ = 15;
  config_info[0].dispatching_type_ = Kokyu::FIFO_DISPATCHING;

  config_info[1].preemption_priority_ = 2;
  config_info[1].thread_priority_ = 2;
  config_info[1].dispatching_type_ = Kokyu::FIFO_DISPATCHING;

  config_info[2].preemption_priority_ = 3;
  config_info[2].thread_priority_ = 0;
  config_info[2].dispatching_type_ = Kokyu::FIFO_DISPATCHING;

  ACE_DEBUG ((LM_DEBUG, "before create_dispatcher\n" ));
  auto_ptr<Kokyu::Dispatcher> disp (Kokyu::Dispatcher_Factory::create_dispatcher (config_info));

  ACE_ASSERT (disp.get() != 0);

  MyCommand cmd1(1), cmd2(2), cmd3(3);

  Kokyu::QoSDescriptor qos1, qos2, qos3;

  qos1.preemption_priority_ = 2;
  qos2.preemption_priority_ = 3;
  qos3.preemption_priority_ = 1;

  disp->dispatch (&cmd1, qos1);
  disp->dispatch (&cmd2, qos2);
  disp->dispatch (&cmd3, qos3);

  while(1){}

  return 0;
}
