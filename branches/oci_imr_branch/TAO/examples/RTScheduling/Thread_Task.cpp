//$Id$

#include "Thread_Task.h"
#include "ace/Atomic_Op.h"
#include "Task_Stats.h"
#include "tao/ORB_Core.h"
#include "ace/High_Res_Timer.h"

Thread_Task::Thread_Task (void)
{
}

int
Thread_Task::svc (void)
{
  ACE_TRY_NEW_ENV
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "Thread_Task::svc %d\n",
                    importance_));
  
      const char * name = 0;
      CORBA::Policy_var implicit_sched_param = CORBA::Policy::_duplicate (sched_param_.in ());;
      this->current_->begin_scheduling_segment (name,
                                                sched_param_.in (),
                                                implicit_sched_param.in ()
                                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_OS::memcpy (&count_,
                      this->current_->id ()->get_buffer (),
                      this->current_->id ()->length ());
  
      char msg [BUFSIZ];
      ACE_OS::sprintf (msg, "Thread_Task::svc Imp=%d Guid=%d\n", importance_, count_);
      dt_creator_->log_msg (msg);
  
      this->perform_task ();

      this->current_->end_scheduling_segment (name
                                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      dt_creator_->dt_ended () ;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
    }
  ACE_ENDTRY;
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
  ACE_OS::sprintf (fname, "DT_%d.dat",count_);
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
		"File name %s\n",
		fname));
  
  char msg [BUFSIZ];
  ACE_OS::sprintf (msg, "#Schedule Output for DT %d", count_);
  
  task_stats_->dump_samples (fname,
			     msg,
			     ACE_High_Res_Timer::global_scale_factor ());
}
