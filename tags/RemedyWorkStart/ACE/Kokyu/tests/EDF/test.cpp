// $Id$

#include "ace/Auto_Ptr.h"

#include "Kokyu.h"
#include "ace/Task.h"
#include "ace/Sched_Params.h"
#include "ace/SString.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_sys_time.h"

ACE_CString sched_policy_str = "fifo";

int parse_args (int argc, ACE_TCHAR *argv[]);

class MyCommand : public Kokyu::Dispatch_Command
{
public:
  MyCommand(int i)
    :Kokyu::Dispatch_Command(1),id_(i)
  {
  }
  int execute();

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

  Kokyu::Priority_t min_prio =
    ACE_Sched_Params::priority_min (sched_policy);

  config_info[0].preemption_priority_ = 1;
  config_info[0].thread_priority_ = min_prio;
  config_info[0].dispatching_type_ = Kokyu::DEADLINE_DISPATCHING;

  ACE_DEBUG ((LM_DEBUG, "before create_dispatcher\n" ));

  attrs.config_info_set_ = config_info;
  auto_ptr<Kokyu::Dispatcher>
    disp (Kokyu::Dispatcher_Factory::create_dispatcher (attrs));

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

  ACE_DEBUG ((LM_DEBUG, "Deadline of command1 is %d\n",
              qos1.deadline_.sec ()));
  disp->dispatch (&cmd1, qos1);

  ACE_DEBUG ((LM_DEBUG, "Deadline of command2 is %d\n",
              qos2.deadline_.sec ()));
  disp->dispatch (&cmd2, qos2);

  ACE_DEBUG ((LM_DEBUG, "Deadline of command3 is %d\n",
              qos3.deadline_.sec ()));
  disp->dispatch (&cmd3, qos3);

  disp->activate ();

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
