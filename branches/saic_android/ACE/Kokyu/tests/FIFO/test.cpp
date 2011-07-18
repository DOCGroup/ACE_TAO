// $Id$

#include "ace/Auto_Ptr.h"

#include "Kokyu.h"
#include "ace/Task.h"
#include "ace/SString.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_strings.h"

ACE_CString sched_policy_str = "fifo";

int parse_args (int argc, ACE_TCHAR *argv[]);

class MyCommand : public Kokyu::Dispatch_Command
{
public:
  MyCommand(int i)
    :Kokyu::Dispatch_Command(1),id_(i)
  {
  }
  int execute ();

private:
  int id_;
};

int MyCommand::execute()
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
                ACE_TEXT ("(%t|prio=%d) | command %d executed\n"),
                prio, id_));
    return 0;
}

int ACE_TMAIN (int argc, ACE_TCHAR** argv)
{
  Kokyu::ConfigInfoSet config_info(3);

  int  hi_prio, me_prio, lo_prio;
  int sched_policy=ACE_SCHED_FIFO;

  Kokyu::Dispatcher_Attributes attrs;

  if (parse_args (argc, argv) == -1)
    return 0;

  if (ACE_OS::strcasecmp(sched_policy_str.c_str(), "fifo") == 0)
    {
      sched_policy = ACE_SCHED_FIFO;
    }
  else if (ACE_OS::strcasecmp(sched_policy_str.c_str(), "other") == 0)
    {
      sched_policy = ACE_SCHED_OTHER;
    }
  else if (ACE_OS::strcasecmp(sched_policy_str.c_str(), "rr") == 0)
    {
      sched_policy = ACE_SCHED_RR;
    }

  attrs.sched_policy (sched_policy);

  hi_prio = ACE_Sched_Params::priority_max (sched_policy);
  me_prio = ACE_Sched_Params::previous_priority (sched_policy,
                                                 hi_prio);
  lo_prio = ACE_Sched_Params::previous_priority (sched_policy,
                                                 me_prio);

  config_info[0].preemption_priority_ = 1;
  config_info[0].thread_priority_ = hi_prio ;
  config_info[0].dispatching_type_ = Kokyu::FIFO_DISPATCHING;

  config_info[1].preemption_priority_ = 2;
  config_info[1].thread_priority_ = me_prio;
  config_info[1].dispatching_type_ = Kokyu::FIFO_DISPATCHING;

  config_info[2].preemption_priority_ = 3;
  config_info[2].thread_priority_ = lo_prio;
  config_info[2].dispatching_type_ = Kokyu::FIFO_DISPATCHING;

  attrs.config_info_set_ = config_info;

  ACE_DEBUG ((LM_DEBUG, "before create_dispatcher\n" ));
  auto_ptr<Kokyu::Dispatcher>
    disp (Kokyu::Dispatcher_Factory::create_dispatcher (attrs));

  ACE_ASSERT (disp.get() != 0);

  MyCommand cmd1(1), cmd2(2), cmd3(3);

  Kokyu::QoSDescriptor qos1, qos2, qos3;

  qos1.preemption_priority_ = 2;
  ACE_DEBUG ((LM_DEBUG, "Priority of command1 is %d\n",
              qos1.preemption_priority_));

  qos2.preemption_priority_ = 3;
  ACE_DEBUG ((LM_DEBUG, "Priority of command2 is %d\n",
              qos2.preemption_priority_));

  qos3.preemption_priority_ = 1;
  ACE_DEBUG ((LM_DEBUG, "Priority of command3 is %d\n",
              qos3.preemption_priority_));

  if (disp->dispatch (&cmd1, qos1) == -1 ||
      disp->dispatch (&cmd2, qos2) == -1 ||
      disp->dispatch (&cmd3, qos3) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "Error in dispatching command object\n"), -1);

  if (disp->activate () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Error activating dispatcher. ")
                         ACE_TEXT ("You might not have superuser privileges ")
                         ACE_TEXT ("to run FIFO class. Try \"-p other\"\n")), -1);
    }

  disp->shutdown ();

  ACE_DEBUG ((LM_DEBUG, "after shutdown\n"));
  return 0;
}

int parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("p:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'p':
        sched_policy_str = ACE_TEXT_ALWAYS_CHAR(get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s %s"
                           "\n",
                           argv [0],
                           "-p <fifo|rr|other>"),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}
