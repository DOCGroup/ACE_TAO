//$Id$
#include "FP_Task.h"

int 
FP_Task::perform_task (int load,
		       int thr_id)
{
  ACE_Time_Value run_time;

  static CORBA::ULong prime_number = 9619;
  
  ACE_Time_Value base_time = ACE_OS::gettimeofday ();
  for (int i = 0; i < load; i++)
    {
      for (int j = 0; j < 1000; j++)
	{
	  ACE::is_prime (prime_number,
			 2,
			 prime_number / 2);
	}
      
      //        ACE_DEBUG ((LM_DEBUG,
      //  		  "%d\n",
      //  		  thr_id));

      run_time = ACE_OS::gettimeofday () - base_time;
      TASK_STATS::instance ()->sample (ACE_UINT64 (run_time.sec ()),
				       thr_id);
    }
  return 0;
}

