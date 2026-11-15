#include "Periodic_Task.h"

#include "ace/High_Res_Timer.h"
#include "tao/debug.h"

#include <memory>
#include <new>
#include <utility>

int
Periodic_Task::init_task (ACE_Arg_Shifter& arg_shifter)
{
  ACE_TCHAR const *current_arg {};

  while (arg_shifter.is_anything_left ())
    {
      if ((current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-JobName"))) != nullptr)
        {
          name_ = ACE_TEXT_ALWAYS_CHAR(current_arg);
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-Priority"))) != nullptr)
        {
          task_priority_ = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-Period"))) != nullptr)
        {
          period_ = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-ExecTime"))) != nullptr)
        {
          exec_time_ = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-Phase"))) != nullptr)
        {
          phase_ = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-Iter"))))
        {
          iter_ = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();

          // create the stat object.
          std::unique_ptr<Task_Stats> task_stats (
            new (std::nothrow) Task_Stats (iter_));
          if (!task_stats)
          {
            return -1;
          }

          if (task_stats->init () == -1)
          {
            return -1;
          }

          task_stats_ = std::move (task_stats);
        }
      else if ((current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-Load"))) != nullptr)
        {
          load_ = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();

          return 0;
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("parse Task unknown option %s\n"),
                      arg_shifter.get_current ()));
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("name %C, priority %d, period %QuS, ")
                        ACE_TEXT ("exec_time %QuS, phase %quS, iter %d, load %d\n"),
                        name_.c_str (), task_priority_,
                        static_cast<ACE_UINT64> (period_),
                        static_cast<ACE_UINT64> (exec_time_),
                        static_cast<ACE_INT64> (phase_), iter_, load_));
          break;
        }
    }
  return 0;
}

char const *
Periodic_Task::job () const
{
  return name_.c_str ();
}

void
Periodic_Task::job (Job_ptr job)
{
  job_ = Job::_duplicate (job);
}

void
Periodic_Task::dump_stats (ACE_TCHAR const *msg)
{
  ACE_TCHAR buf[BUFSIZ];
  ACE_OS::snprintf (buf, sizeof (buf) / sizeof (buf[0]),
                    ACE_TEXT ("%s%s"),
                    ACE_TEXT_CHAR_TO_TCHAR (name_.c_str ()),
                    ACE_TEXT (".dat"));

  ACE_TString fname (buf);

  ACE_OS::snprintf (buf, sizeof (buf) / sizeof (buf[0]),
                    ACE_TEXT ("#%s #name %s, priority %d, period %lu, ")
                    ACE_TEXT ("exec_time %lu, phase %ld, iter_ %d, load_ %d"),
                    msg, ACE_TEXT_CHAR_TO_TCHAR (name_.c_str ()), task_priority_,
                    period_, exec_time_, phase_, iter_, load_);

  task_stats_->dump_samples (fname.c_str (), buf,
                             ACE_High_Res_Timer::global_scale_factor ());
}
