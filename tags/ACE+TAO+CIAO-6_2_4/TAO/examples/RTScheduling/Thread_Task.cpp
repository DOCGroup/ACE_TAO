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
   job_name_ (),
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

      RTScheduling::Current::IdType_var guid = this->current_->id ();
      ACE_OS::memcpy (&count_,
                      guid->get_buffer (),
                      guid->length ());

      ACE_TCHAR msg [BUFSIZ];
      ACE_OS::sprintf (msg,
                       ACE_TEXT("Thread_Task::svc Imp=%d Guid=")
                       ACE_SIZE_T_FORMAT_SPECIFIER
                       ACE_TEXT("\n"),
                       importance_,
                       count_);

      dt_creator_->log_msg (ACE_TEXT_ALWAYS_CHAR(msg));

      this->perform_task ();

      this->current_->end_scheduling_segment (name);

      dt_creator_->dt_ended () ;
    }
  catch (const CORBA::THREAD_CANCELLED &)
    {
      // Threads can be cancelled.
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

time_t
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
  return job_name_.in ();
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
  ACE_TCHAR fname [BUFSIZ];
  ACE_OS::sprintf (fname,
                   ACE_TEXT("DT_")
                   ACE_SIZE_T_FORMAT_SPECIFIER
                   ACE_TEXT(".dat"),
                   count_);

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
    "File name %s\n",
    fname));

  ACE_TCHAR msg [BUFSIZ];
  ACE_OS::sprintf (msg,
                   ACE_TEXT("#Schedule Output for DT ")
                   ACE_SIZE_T_FORMAT_SPECIFIER,
                   count_);

  task_stats_->dump_samples (fname, msg);
}
