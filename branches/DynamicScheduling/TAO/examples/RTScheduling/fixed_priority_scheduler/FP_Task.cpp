//$Id$
#include "FP_Task.h"

int 
FP_Task::perform_task (int load,
		       int thr_id)
{
  ACE_hrtime_t run_time;

  static CORBA::ULong prime_number = 9619;
  
  for (int i = 0; i < 100; i++)
    {
      for (int j = 0; j < load; j++)
	{
	  
	  ACE::is_prime (prime_number,
			 2,
			 prime_number / 2);
	}
      
      run_time = ACE_OS::gethrtime ();
      TASK_STATS::instance ()->sample (run_time,
				       thr_id);
    }
  return 0;
}
