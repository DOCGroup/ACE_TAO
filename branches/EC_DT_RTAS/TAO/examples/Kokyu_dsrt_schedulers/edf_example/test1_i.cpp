//$Id$

#include "test1_i.h"
#include "ace/High_Res_Timer.h"
#include "ace/Task.h"
#include "ace/ACE.h"
#include "tao/debug.h"
#include "orbsvcs/orbsvcs/Time_Utilities.h"
#include "EDF_Scheduler.h"
#include "Task_Stats.h"

#include "dt_oneway_config.h"
#include "dt_oneway_dsui_families.h"
#include <dsui.h>

#if !defined(__ACE_INLINE__)
#include "test1_i.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(MT_Server, test1_i, "test1_i.cpp,v 1.2 2003/10/08 13:26:32 venkita Exp")

int Deadline_missed = 0;

void
Simple_Server1_i::test_method (CORBA::Long exec_duration, CORBA::Long id, CORBA::Long iter, unsigned long long deadline ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{

  DSUI_EVENT_LOG (TEST_ONE_FAM, START_SERVICE, id, 0, NULL);

  ACE_hthread_t thr_handle;
  ACE_Thread::self (thr_handle);
  int prio;
  int guid;

  ACE_OS::
    memcpy (&guid,
            this->current_->id (ACE_ENV_SINGLE_ARG_PARAMETER)->get_buffer (),
            sizeof (this->current_->id (ACE_ENV_SINGLE_ARG_PARAMETER)->length ()));

  ACE_High_Res_Timer timer;
  ACE_Time_Value elapsed_time;

//  ACE_DEBUG ((LM_DEBUG, "(%t|%T)Request in thread %t\n"));

  if (ACE_Thread::getprio (thr_handle, prio) == -1)
    {
      if (errno == ENOTSUP)
	{
	  ACE_DEBUG((LM_DEBUG,
		     ACE_TEXT ("getprio not supported on this platform\n")
		     ));
	  return;
	}
      ACE_ERROR ((LM_ERROR,
			 ACE_TEXT ("%p\n"),
			 ACE_TEXT ("getprio failed"))
			);
    }

timer.start();
timeval tv;

tv.tv_sec = exec_duration; 
tv.tv_usec = 0; 

CPULoad::run(tv);

/*
  static CORBA::ULong prime_number = 9619899;

  ACE_Time_Value compute_count_down_time (exec_duration, 0);
  ACE_Countdown_Time compute_count_down (&compute_count_down_time);
*/
  //Applicable only for CV based implementations
  //yield every 1 sec
/*  ACE_Time_Value yield_interval (3,0);

  ACE_Time_Value yield_count_down_time (yield_interval);
  ACE_Countdown_Time yield_count_down (&yield_count_down_time);
  
  timer.start ();
  int j=0;
  while (compute_count_down_time > ACE_Time_Value::zero)
    {
      ACE::is_prime (prime_number,
		     2,
		     prime_number / 2);
      
      ++j;
*/
#ifdef KOKYU_DSRT_LOGGING
//      if (j%1000 == 0) 
//        {
//          ACE_DEBUG ((LM_DEBUG, 
//            "(%t|%T) loop # = %d, load = %usec\n", j, exec_duration)); 
//        }
#endif
//      if (j%1000 == 0)
//        {
//          ACE_Time_Value run_time = ACE_OS::gettimeofday ();
//          task_stats_.sample (ACE_UINT64 (run_time.msec ()), guid);
//        }
      
/*      compute_count_down.update ();

      if (enable_yield_)
        {
          yield_count_down.update ();
          if (yield_count_down_time <= ACE_Time_Value::zero)
            {
              CORBA::Policy_var sched_param_policy = 
                CORBA::Policy::_duplicate (current_->
                                           scheduling_parameter(ACE_ENV_SINGLE_ARG_PARAMETER));
              
              const char * name = 0;
              
              CORBA::Policy_var implicit_sched_param = sched_param_policy;
              current_->update_scheduling_segment (name,
                                                   sched_param_policy.in (),
                                                   implicit_sched_param.in ()
                                                   ACE_ENV_ARG_PARAMETER);
              yield_count_down_time = yield_interval;
              yield_count_down.start ();
            } 
        }
    }
*/
  TimeBase::TimeT current;
  ORBSVCS_Time::Time_Value_to_TimeT (current, ACE_OS::gettimeofday ());
  unsigned long long cur_tv = (unsigned long long) current;
  if(cur_tv > deadline )
    Deadline_missed=Deadline_missed +1;

  timer.stop ();
  timer.elapsed_time (elapsed_time);
 
  ACE_DEBUG ((LM_DEBUG, "(%t|%T)" 
	      "task_id = %d, iter = %d, deadline = %Q, elapsed time = %umsec,"
	      "deadline_missed = %d\n", 
	      id, iter, deadline, elapsed_time.msec (), Deadline_missed ));
/*DTTIME:
  recording the finishing time on the server side. please also record the deadline_missed variable.
*/
  DSUI_EVENT_LOG (TEST_ONE_FAM, STOP_SERVICE, id,0,NULL);
}

void
Simple_Server1_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "shutdown request from client\n"));
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}
