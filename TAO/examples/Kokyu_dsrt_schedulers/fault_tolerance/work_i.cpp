//$Id$

#include "work_i.h"
#include "ace/High_Res_Timer.h"
#include "ace/Task.h"
#include "ace/ACE.h"
#include "tao/debug.h"
#include "EDF_Scheduler.h"
#include <sys/types.h>
#include <sys/mman.h>

#if defined (ACE_HAS_DSUI)
#include <ace/Counter.h>
#include "ft_config.h"
#include "ft_dsui_families.h"
#include <dsui.h>
#endif // ACE_HAS_DSUI

#if !defined(__ACE_INLINE__)
#include "work_i.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(MT_Server, work_i, "work_i.cpp,v 1.2 2003/10/08 13:26:32 venkita Exp")

void
Complex_Server_i::test_method2 (CORBA::Long exec_duration, CORBA::Long need_ft ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Policy_ptr sched_policy =
    this->current_->scheduling_parameter(ACE_ENV_SINGLE_ARG_PARAMETER);

  EDF_Scheduling::SchedulingParameterPolicy_var sched_param_policy =
  EDF_Scheduling::SchedulingParameterPolicy::_narrow (sched_policy);

  EDF_Scheduling::SchedulingParameter_var sched_param = sched_param_policy->value ();

  ACE_hthread_t thr_handle;
  ACE_Thread::self (thr_handle);
  int prio;
  int guid;

  ACE_OS::
    memcpy (&guid,
            this->current_->id (ACE_ENV_SINGLE_ARG_PARAMETER)->get_buffer (),
            sizeof (this->current_->id (ACE_ENV_SINGLE_ARG_PARAMETER)->length ()));

  Object_ID oid;
  oid.id=sched_param->id;
  oid.pid=sched_param->pid;
  oid.tid=sched_param->tid;
  oid.task_id=sched_param->task_id;
  oid.guid=guid;
  DSUI_EVENT_LOG (TEST_ONE_FAM, START_SERVICE, 0, sizeof(Object_ID), (char*)&oid);

  ACE_High_Res_Timer timer;
  ACE_Time_Value elapsed_time;

  ACE_DEBUG ((LM_DEBUG, "Request in thread %t\n"));

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
			 ACE_TEXT ("getprio failed")));
      return;
    }

  ACE_DEBUG ((LM_DEBUG, 
              "Request in thread %t, prio = %d,"
              "exec duration = %u\n", prio, exec_duration));
  if (need_ft == 2 ) {
    int wflags = O_RDWR;
    int wfd;
    unsigned char *ptr;
    const char *wshfile = "shfile.txt";

    wfd = shm_open(wshfile, wflags, S_IRWXU);
    ptr = (unsigned char *)mmap(NULL, 1, PROT_READ|PROT_WRITE, MAP_SHARED, wfd, 0);
    close(wfd);

    *ptr = *ptr - 1;

  #ifdef KOKYU_DSRT_LOGGING
    ACE_DEBUG((LM_DEBUG,"NOW count is equal to %d\n", *ptr));
  #endif

    if((*ptr)!=0) return;

    else *ptr = 2;
  }
  static CORBA::ULong prime_number = 9619899;

  ACE_Time_Value compute_count_down_time (exec_duration, 0);
  ACE_Countdown_Time compute_count_down (&compute_count_down_time);

  //Applicable only for CV based implementations
  //yield every 1 sec
  ACE_Time_Value yield_interval (50,0);

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

#ifdef KOKYU_DSRT_LOGGING
      if (j%1000 == 0) 
        {
          ACE_DEBUG ((LM_DEBUG, 
            "(%t|%T) loop # = %d, load = %usec\n", j, exec_duration)); 
        }
#endif
      if (j%1000 == 0)
        {
//          ACE_Time_Value run_time = ACE_OS::gettimeofday ();
//          task_stats_.sample (ACE_UINT64 (run_time.msec ()), guid);
        }
      
      compute_count_down.update ();

      if (enable_yield_)
        {
          yield_count_down.update ();
          if (yield_count_down_time <= ACE_Time_Value::zero)
            {
              CORBA::Policy_var sched_param_policy = 
                CORBA::Policy::_duplicate (current_->
                                           scheduling_parameter(ACE_ENV_SINGLE_ARG_PARAMETER));
              
              const char * name = 0;
              
              CORBA::Policy_ptr implicit_sched_param = 0;
              current_->update_scheduling_segment (name,
                                                   sched_param_policy.in (),
                                                   implicit_sched_param
                                                   ACE_ENV_ARG_PARAMETER);
              yield_count_down_time = yield_interval;
              yield_count_down.start ();
            }
        }
    }

  timer.stop ();
  timer.elapsed_time (elapsed_time);
  
  ACE_DEBUG ((LM_DEBUG, 
	      "Request processing in thread %t done, "
	      "prio = %d, load = %d, elapsed time = %umsec\n", 
	      prio, exec_duration, elapsed_time.msec () ));
  DSUI_EVENT_LOG (TEST_ONE_FAM, STOP_SERVICE, 0, sizeof(Object_ID), (char*)&oid);
  
}

void
Complex_Server_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "shutdown request from client\n"));
  int wflags = O_RDWR;
  int wfd;
  unsigned char *ptr;
  const char *wshfile = "shfile.txt";

  wfd = shm_open(wshfile, wflags, S_IRWXU);
  ptr = (unsigned char *)mmap(NULL, 1, PROT_READ|PROT_WRITE, MAP_SHARED, wfd, 0);
  close(wfd);

  *ptr = *ptr - 1;

  ACE_DEBUG((LM_DEBUG,"NOW count is equal to %d\n", *ptr));

  if((*ptr)!=0) return;

  else
    this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}
