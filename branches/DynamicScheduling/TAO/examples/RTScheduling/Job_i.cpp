//$Id$
#include "Job_i.h"

#include "tao/debug.h"
#include "ace/Arg_Shifter.h"
#include "DT_Creator.h"
#include "Task_Stats.h"
#include "ace/High_Res_Timer.h"

//#include "Activity.h"

Job_i::Job_i (DT_Creator* dt_creator)
  : dt_creator_ (dt_creator),
    guid_ (0)
{
  // create the stat object.
  ACE_NEW (task_stats_, Task_Stats);
  task_stats_->init (100);

}

const ACE_CString&
Job_i::name (void)
{
  return job_name_;
}

const ACE_CString&
Job_i::poa (void)
{
  return POA_name_;
}

int
Job_i::init (ACE_Arg_Shifter& arg_shifter)
{
  job_name_ = arg_shifter.get_current (); // Read the name of the Job
  arg_shifter.consume_arg ();

  POA_name_ = arg_shifter.get_current (); // Read the name of the POA
  arg_shifter.consume_arg ();

  return 0;
}

void
Job_i::work (CORBA::ULong work,
	     CORBA::Short importance
	     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
/*
  int priority;
  ACE_hthread_t current;
  ACE_Thread::self (current);
  if (ACE_Thread::getprio (current, priority) == -1)
    return;

*/
  static CORBA::ULong prime_number = 9619;
  //    ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
		"test_i::method: %d units of work\n",
		work));

  if (guid_ == 0)
    ACE_OS::memcpy (&guid_,
		    dt_creator_->current ()->id (ACE_ENV_SINGLE_ARG_PARAMETER)->get_buffer (),
		    sizeof (dt_creator_->current ()->id (ACE_ENV_SINGLE_ARG_PARAMETER)->length ()));



  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
		"%t Guid is %d, Importance is %d\n",
		guid_,
		importance));

  char msg [BUFSIZ];
  //ACE_OS::sprintf (msg,"Thread Priority is %d \n",priority);
  ACE_OS::sprintf (msg,"Guid is %d\n", guid_);
  dt_creator_->log_msg (msg);

  for (; work != 0; work--)
    {
      //    ACE_hrtime_t now = ACE_OS::gethrtime ();

      ACE_Time_Value run_time = ACE_OS::gettimeofday () - *(dt_creator_->base_time ());
      TASK_STATS::instance ()->sample (ACE_UINT64 (run_time.sec ()),
       				       guid_);

      ACE_Time_Value count_down_time (1);
      ACE_Countdown_Time count_down (&count_down_time);

      while (count_down_time > ACE_Time_Value::zero)
	{
	  ACE::is_prime (prime_number,
			 2,
			 prime_number / 2);
	  count_down.update ();
	}

      run_time = ACE_OS::gettimeofday () - *(dt_creator_->base_time ());
      TASK_STATS::instance ()->sample (ACE_UINT64 (run_time.sec ()),
       				       guid_);

      CORBA::Policy_var sched_param;
      sched_param = CORBA::Policy::_duplicate (dt_creator_->sched_param (importance));
      const char * name = 0;
      dt_creator_->current ()->update_scheduling_segment (name,
							  sched_param.in (),
							  sched_param.in ()
							  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;


      // convert to microseconds
      //  #if !defined ACE_LACKS_LONGLONG_T

      //        ACE_UINT32 elapsed_microseconds = ACE_UINT32((now - dt_creator_->base_hr_time ()) / gsf);

      //  #else  /* ! ACE_LACKS_LONGLONG_T */

      //        ACE_UINT32 elapsed_microseconds = (now - dt_creator_->base_hr_time ()) / gsf;

      //  #endif /* ! ACE_LACKS_LONGLONG_T */

      //  #if defined (ACE_WIN32)
      //        elapsed_microseconds*=1000; // convert to uSec on Win32
      //  #endif /* ACE_WIN32 */

      //        TASK_STATS::instance ()->sample (elapsed_microseconds,
      //  				       guid_);


    }

  //  task_stats_->sample (ACE_UINT64 (run_time.sec ()),
  //  		       guid_);

}

void
Job_i::post_work (int /*guid*/,
		  int /*importance*/)
{
}

int
Job_i::guid (void)
{
  return this->guid_;
}

void
Job_i::shutdown (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  dt_creator_->job_ended ();
}

void
Job_i::dump_stats (void)
{
  char fname [BUFSIZ];
  ACE_OS::sprintf (fname, "Job_%d.dat",guid_);
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
		"File name %s\n",
		fname));


  char msg [BUFSIZ];
  ACE_OS::sprintf (msg, "#Schedule Output for DT %d", guid_);

  task_stats_->dump_samples (fname,
			     msg,
			     ACE_High_Res_Timer::global_scale_factor ());

}
