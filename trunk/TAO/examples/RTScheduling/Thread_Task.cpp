//$Id$

#include "Thread_Task.h"
#include "DT_Creator.h"
#include "Task_Stats.h"

#include "tao/ORB_Core.h"

#include "ace/High_Res_Timer.h"
#include "ace/Atomic_Op.h"

Thread_Task::Thread_Task (void)
 : start_time_ (0),
   load_ (0),
   iter_ (0),
   count_ (0),
   importance_ (0),
   dt_creator_ (0),
   base_time_ (0),
   dist_ (0),
   job_name_ (0),
   task_stats_ (0)
{
}

int
Thread_Task::svc (void)
{
  try
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Thread_Task::svc %d\n",
                      importance_));
        }

      const char * name = 0;
      CORBA::Policy_var implicit_sched_param = CORBA::Policy::_duplicate (sched_param_.in ());;
      this->current_->begin_scheduling_segment (name,
                                                sched_param_.in (),
                                                implicit_sched_param.in ());

      ACE_OS::memcpy (&count_,
                      this->current_->id ()->get_buffer (),
                      this->current_->id ()->length ());

      char msg [BUFSIZ];
      ACE_OS::sprintf (msg,
                       "Thread_Task::svc Imp=%d Guid="
                       ACE_SIZE_T_FORMAT_SPECIFIER
                       "\n",
                       importance_,
                       count_);

      dt_creator_->log_msg (msg);

      this->perform_task ();

      this->current_->end_scheduling_segment (name);

      dt_creator_->dt_ended () ;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
    }
  return 0;
}

int
Thread_Task::importance (void)
{
  return this->importance_;
}

int
Thread_Task::start_time (void)
{
  return this->start_time_;
}

int
Thread_Task::perform_task (void)
{
  return 0;
}


const char*
Thread_Task::job (void)
{
  return job_name_;
}

void
Thread_Task::job (Job_ptr job)
{
  job_ = Job::_duplicate (job);
}

int
Thread_Task::dist (void)
{
  return this->dist_;
}

void
Thread_Task::dump_stats (void)
{
  char fname [BUFSIZ];
  ACE_OS::sprintf (fname,
                   "DT_"
                   ACE_SIZE_T_FORMAT_SPECIFIER
                   ".dat",count_);

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
    "File name %s\n",
    fname));

  char msg [BUFSIZ];
  ACE_OS::sprintf (msg,
                   "#Schedule Output for DT "
                   ACE_SIZE_T_FORMAT_SPECIFIER,
                   count_);

  task_stats_->dump_samples (fname,
                             msg,
                             ACE_High_Res_Timer::global_scale_factor ());
}
