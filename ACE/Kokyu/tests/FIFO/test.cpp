#include "Kokyu.h"
#include "ace/Task.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_strings.h"
#include <memory>
#include <string>

int parse_args (int argc, ACE_TCHAR *argv[], std::string &sched_policy);

class MyCommand : public Kokyu::Dispatch_Command
{
public:
  explicit MyCommand (int i)
    : Kokyu::Dispatch_Command (true), id_ (i)
  {
  }
  MyCommand (MyCommand const &) = delete;
  MyCommand (MyCommand &&) = delete;
  MyCommand &operator= (MyCommand const &) = delete;
  MyCommand &operator= (MyCommand &&) = delete;

  int execute () override;

private:
  int const id_;
};

int MyCommand::execute ()
{
  ACE_hthread_t thr_handle {};
  ACE_Thread::self (thr_handle);
  int prio {};

  if (ACE_Thread::getprio (thr_handle, prio) == -1)
    {
      if (errno == ENOTSUP)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("getprior not supported on this platform\n")));
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

int ACE_TMAIN (int argc, ACE_TCHAR **argv)
{
  Kokyu::ConfigInfoSet config_info (3);

  int sched_policy {ACE_SCHED_FIFO};
  std::string sched_policy_str {"fifo"};

  Kokyu::Dispatcher_Attributes attrs;

  if (parse_args (argc, argv, sched_policy_str) == -1)
    {
      return 1;
    }

  if (ACE_OS::strcasecmp (sched_policy_str.c_str (), "fifo") == 0)
    {
      sched_policy = ACE_SCHED_FIFO;
    }
  else if (ACE_OS::strcasecmp (sched_policy_str.c_str (), "other") == 0)
    {
      sched_policy = ACE_SCHED_OTHER;
    }
  else if (ACE_OS::strcasecmp (sched_policy_str.c_str (), "rr") == 0)
    {
      sched_policy = ACE_SCHED_RR;
    }

  attrs.sched_policy (sched_policy);

  int const hi_prio = ACE_Sched_Params::priority_max (sched_policy);
  int const me_prio = ACE_Sched_Params::previous_priority (sched_policy,
                                                           hi_prio);
  int const lo_prio = ACE_Sched_Params::previous_priority (sched_policy,
                                                           me_prio);

  config_info[0].preemption_priority_ = 1;
  config_info[0].thread_priority_ = hi_prio;
  config_info[0].dispatching_type_ = Kokyu::FIFO_DISPATCHING;

  config_info[1].preemption_priority_ = 2;
  config_info[1].thread_priority_ = me_prio;
  config_info[1].dispatching_type_ = Kokyu::FIFO_DISPATCHING;

  config_info[2].preemption_priority_ = 3;
  config_info[2].thread_priority_ = lo_prio;
  config_info[2].dispatching_type_ = Kokyu::FIFO_DISPATCHING;

  attrs.config_info_set_ = config_info;

  ACE_DEBUG ((LM_DEBUG, "before create_dispatcher\n"));
  std::unique_ptr<Kokyu::Dispatcher> disp (
    Kokyu::Dispatcher_Factory::create_dispatcher (attrs));
  if (!disp)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "Unable to create dispatcher\n"), 1);
    }

  MyCommand cmd1 {1}, cmd2 {2}, cmd3 {3};

  Kokyu::QoSDescriptor qos1 {}, qos2 {}, qos3 {};

  qos1.preemption_priority_ = 2;
  ACE_DEBUG ((LM_DEBUG, "Priority of command1 is %q\n",
              static_cast<ACE_INT64> (qos1.preemption_priority_)));

  qos2.preemption_priority_ = 3;
  ACE_DEBUG ((LM_DEBUG, "Priority of command2 is %q\n",
              static_cast<ACE_INT64> (qos2.preemption_priority_)));

  qos3.preemption_priority_ = 1;
  ACE_DEBUG ((LM_DEBUG, "Priority of command3 is %q\n",
              static_cast<ACE_INT64> (qos3.preemption_priority_)));

  if (disp->dispatch (&cmd1, qos1) == -1 ||
      disp->dispatch (&cmd2, qos2) == -1 ||
      disp->dispatch (&cmd3, qos3) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "Error in dispatching command object\n"), -1);
    }

  if (disp->activate () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Error activating dispatcher. ")
                         ACE_TEXT ("You might not have superuser privileges ")
                         ACE_TEXT ("to run FIFO class. Try \"-p other\"\n")), -1);
    }

  if (disp->shutdown () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "Error shutting down dispatcher\n"), -1);
    }

  ACE_DEBUG ((LM_DEBUG, "after shutdown\n"));
  return 0;
}

int parse_args (int argc, ACE_TCHAR *argv[], std::string &sched_policy)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("p:"));

  for (int c = get_opts (); c != -1; c = get_opts ())
    {
      switch (c)
        {
        case 'p':
          sched_policy = ACE_TEXT_ALWAYS_CHAR (get_opts.opt_arg ());
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
    }
  return 0;
}
