//$Id$
#include "MIF_Task.h"


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

int 
MIF_Task::perform_task (void)
{
  char msg [BUFSIZ];
  ACE_OS::sprintf (msg, "MIF_Task::perform_task %d\n", count_);
  dt_creator_->log_msg (msg);

  ACE_Time_Value run_time;
  
  static CORBA::ULong prime_number = 9619;
  
  //ACE_Time_Value base_time = ACE_OS::gettimeofday ();
  for (int i = 0; i < 100; i++)
    {
      for (int j = 0; j < this->load_; j++)
	{
	  ACE::is_prime (prime_number,
			 2,
			 prime_number / 2);
	}
      
      //        ACE_DEBUG ((LM_DEBUG,
      //  		  "%d\n",
      //  		  thr_id));

      run_time = ACE_OS::gettimeofday () - *base_time_;
      TASK_STATS::instance ()->sample (ACE_UINT64 (run_time.sec ()),
				       count_);
      CORBA::Policy_var sched_param;
      sched_param = CORBA::Policy::_duplicate (dt_creator_->sched_param (this->importance_));
      const char * name = 0;
      CORBA::Policy_ptr implicit_sched_param = 0;
      current_->update_scheduling_segment (name,
					   sched_param,
					   implicit_sched_param
					   ACE_ENV_ARG_DECL);
      ACE_CHECK;
    }
  return 0;
}

