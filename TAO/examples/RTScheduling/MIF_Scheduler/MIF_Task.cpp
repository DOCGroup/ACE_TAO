//$Id$
#include "MIF_Task.h"
#include "test.h"


MIF_Task::MIF_Task (int importance,
		  int start_time,
		  int load,
		  DT_Creator *dt_creator)
{
  this->load_ = load;
  this->start_time_ = start_time;
  this->importance_ = importance;
  this->dt_creator_ = dt_creator;
}

void
MIF_Task::pre_activate (void)
{
  DT_TEST::instance ()->scheduler ()->incr_thr_count ();
}

void
MIF_Task::post_activate (void)
{
  DT_TEST::instance ()->scheduler ()->wait ();
}

int 
MIF_Task::perform_task (void)
{
  char msg [BUFSIZ];
  ACE_OS::sprintf (msg, "MIF_Task::perform_task %d\n", count_);
  dt_creator_->log_msg (msg);

  ACE_Time_Value run_time;
  
  static CORBA::ULong prime_number = 9619;
  
  for (int i = 0; i < this->load_; i++)
    {
      for (int j = 0; j < 500; j++)
	{
	  ACE::is_prime (prime_number,
			 2,
			 prime_number / 2);
	}

      run_time = ACE_OS::gettimeofday () - *base_time_;
      TASK_STATS::instance ()->sample (ACE_UINT64 (run_time.sec ()),
				       count_);
      CORBA::Policy_var sched_param;
      sched_param = CORBA::Policy::_duplicate (dt_creator_->sched_param (this->importance_));
      const char * name = 0;
      CORBA::Policy_ptr implicit_sched_param = 0;
      current_->update_scheduling_segment (name,
					   sched_param.in (),
					   implicit_sched_param
					   ACE_ENV_ARG_DECL);
      ACE_CHECK;
    }
  return 0;
}

