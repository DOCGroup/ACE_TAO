//$Id$
#include "FP_Task.h"
#include "test.h"
#include "tao/RTCORBA/Priority_Mapping_Manager.h"
#include "tao/RTCORBA/Priority_Mapping.h"
#include "ace/High_Res_Timer.h"


FP_Task::FP_Task (int importance,
		  int start_time,
		  int load,
		  int iter,
		  int dist,
		  char *job_name,
		  DT_Creator *dt_creator)
{
  this->load_ = load;
  this->iter_ = iter;
  this->start_time_ = start_time;
  this->importance_ = importance;
  this->dt_creator_ = dt_creator;
  this->dist_ = dist;
  this->job_name_ =  CORBA::string_dup (job_name);

}

void
FP_Task::pre_activate (void)
{
  ACE_TRY_NEW_ENV
    {
      CORBA::Object_var object = DT_TEST::instance ()->orb ()->resolve_initial_references ("PriorityMappingManager"
											   ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      RTCORBA::PriorityMappingManager_var mapping_manager =
	RTCORBA::PriorityMappingManager::_narrow (object.in ()
						  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      RTCORBA::PriorityMapping *pm =
	mapping_manager->mapping ();

      CORBA::Short native_priority;
      if (pm->to_native (this->importance_, native_priority) == 0)
	ACE_ERROR ((LM_ERROR,
		    "Cannot convert native priority %d to corba priority\n",
		    native_priority));
      if (TAO_debug_level > 0)
	ACE_DEBUG ((LM_DEBUG,
		    "Native Priority = %d\n",
		    native_priority));
      char msg [BUFSIZ];
      ACE_OS::sprintf (msg, "Native Priority %d\n",
		       native_priority);
      dt_creator_->log_msg (msg);

      this->importance_ = native_priority;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
    }
  ACE_ENDTRY;
}

int
FP_Task::perform_task (void)
{

  //ACE_Time_Value base_time = ACE_OS::gettimeofday ();

  //   ACE_Time_Value run_time = ACE_OS::gettimeofday () - *base_time_;
  //   TASK_STATS::instance ()->sample (ACE_UINT64 (run_time.sec ()),
  // 				   count_);

  //        ACE_hrtime_t now = ACE_OS::gethrtime () - DT_TEST::instance ()->base_t;
  //        TASK_STATS::instance ()->sample (now,
  //         				       importance_);


  static CORBA::ULong prime_number = 9619;

  for (int j = 0; j < this->iter_; j++)
    {
      ACE_Time_Value run_time = ACE_OS::gettimeofday () - *base_time_;
      TASK_STATS::instance ()->sample (ACE_UINT64 (run_time.sec ()),
       				       count_);

      ACE_Time_Value count_down_time (1);
      ACE_Countdown_Time count_down (&count_down_time);

      while (count_down_time > ACE_Time_Value::zero)
	{
	  ACE::is_prime (prime_number,
			 2,
			 prime_number / 2);

	  count_down.update ();
	}


      //ACE_hrtime_t now = ACE_OS::gethrtime ();
      // convert to microseconds

//  #if !defined ACE_LACKS_LONGLONG_T

//        ACE_UINT32 elapsed_microseconds = ACE_UINT32((now - base_hr_time_) / gsf);

//  #else  /* ! ACE_LACKS_LONGLONG_T */

//        ACE_UINT32 elapsed_microseconds = (now-base_hr_time_) / gsf;

//  #endif /* ! ACE_LACKS_LONGLONG_T */

//  #if defined (ACE_WIN32)
//        elapsed_microseconds*=1000; // convert to uSec on Win32
//  #endif /* ACE_WIN32 */

//        TASK_STATS::instance ()->sample (elapsed_microseconds,
//         				       count_);

    }

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
		"%d\n",
		count_));

  if (this->dist_)
    {
      job_->work (this->load_, this->importance_);


      for (int j = 0; j < this->iter_; j++)
	{
	  ACE_Time_Value run_time = ACE_OS::gettimeofday () - *base_time_;
	  TASK_STATS::instance ()->sample (ACE_UINT64 (run_time.sec ()),
					   count_);

	  ACE_Time_Value count_down_time (1);
	  ACE_Countdown_Time count_down (&count_down_time);

	  while (count_down_time > ACE_Time_Value::zero)
	    {
	      if (TAO_debug_level > 0)
		ACE_DEBUG ((LM_DEBUG,
			    "Waiting on count down\n"));


	      ACE::is_prime (prime_number,
			     2,
			     prime_number / 2);
	      count_down.update ();
	    }

	  //ACE_hrtime_t now = ACE_OS::gethrtime ();
	  // convert to microseconds
	  //  #if !defined ACE_LACKS_LONGLONG_T

	  //  	  ACE_UINT32 elapsed_microseconds = ACE_UINT32((now - base_hr_time_) / gsf);

	  //  #else  /* ! ACE_LACKS_LONGLONG_T */

	  //  	  ACE_UINT32 elapsed_microseconds = (now-base_hr_time_) / gsf;

	  //  #endif /* ! ACE_LACKS_LONGLONG_T */

	  //  #if defined (ACE_WIN32)
	  //  	  elapsed_microseconds*=1000; // convert to uSec on Win32
	  //  #endif /* ACE_WIN32 */

	  //  	  TASK_STATS::instance ()->sample (elapsed_microseconds,
	  //  					   count_);


	}

      if (TAO_debug_level > 0)
	ACE_DEBUG ((LM_DEBUG,
		    "%d\n",
		    count_));
    }

  if (dist_)
    job_->shutdown ();
  return 0;
}
