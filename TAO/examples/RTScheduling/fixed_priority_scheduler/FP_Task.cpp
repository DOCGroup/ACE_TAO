//$Id$
#include "FP_Task.h"


FP_Task::FP_Task (int importance,
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
FP_Task::perform_task (void)
{
  char msg [BUFSIZ];
  ACE_OS::sprintf (msg, "FP_Task::perform_task %d\n", count_);
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
    }
  return 0;
}

