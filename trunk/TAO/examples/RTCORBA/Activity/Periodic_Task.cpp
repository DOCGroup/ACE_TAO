//$Id$

#include "Periodic_Task.h"

#include "ace/High_Res_Timer.h"
#include "tao/debug.h"

#include "Task_Stats.h"

Periodic_Task::Periodic_Task (void)
  :barrier_ (0),
   task_priority_ (0),
   period_ (0),
   exec_time_ (0),
   phase_ (0),
   iter_ (0),
   load_ (0),
   task_stats_ (0)
{
}

Periodic_Task::~Periodic_Task ()
{
  delete task_stats_;
}

int
Periodic_Task::init_task (ACE_Arg_Shifter& arg_shifter)
{
  const ACE_TCHAR *current_arg = 0;

  while (arg_shifter.is_anything_left ())
    {
      if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-JobName"))))
        {
          name_ = ACE_TEXT_ALWAYS_CHAR(current_arg);
          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-Priority"))))
        {
          task_priority_ = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-Period"))))
        {
          period_ = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-ExecTime"))))
        {
          exec_time_ = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-Phase"))))
        {
          phase_ = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-Iter"))))
        {
          iter_ = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();

          // create the stat object.
          ACE_NEW_RETURN (task_stats_, Task_Stats (iter_), -1);

          if (task_stats_->init () == -1)
            return -1;
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-Load"))))
        {
          load_ = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();

          return 0;
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "parse Task unknown option %s\n",
                      arg_shifter.get_current ()));
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG, "name %s, priority %d, period %duS, exec_time %duS, phase %duS, iter %d, load %d\n",
                        name_.c_str(), task_priority_, period_, exec_time_, phase_, iter_, load_));
          break;
        }
    }
  return 0;
}

const char*
Periodic_Task::job (void)
{
  return name_.c_str ();
}

void
Periodic_Task::job (Job_ptr job)
{
  job_ = Job::_duplicate (job);
}

void
Periodic_Task::dump_stats (ACE_TCHAR* msg)
{
  ACE_TCHAR buf[BUFSIZ];
  ACE_OS::sprintf (buf, ACE_TEXT("%s%s"), name_.c_str (), ACE_TEXT(".dat"));

  ACE_TString fname (buf);

  ACE_OS::sprintf (buf,ACE_TEXT("#%s #name %s, priority %d, period %ld, exec_time %ld, phase %ld, iter_ %d , load_ %d"),
                   msg, name_.c_str(), task_priority_, period_, exec_time_, phase_, iter_, load_);

  task_stats_->dump_samples (fname.c_str (), buf,
                             ACE_High_Res_Timer::global_scale_factor ());
}
