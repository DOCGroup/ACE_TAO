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
  //  DT_TEST::instance ()->scheduler ()->incr_thr_count ();
}

void
MIF_Task::post_activate (void)
{
  //  DT_TEST::instance ()->scheduler ()->wait ();
}

int 
MIF_Task::perform_task (void)
{
  char msg [BUFSIZ];
  ACE_OS::printf ("MIF_Task::perform_task %d\n", count_);
  ACE_OS::sprintf (msg, "MIF_Task::perform_task %d\n", count_);
  dt_creator_->log_msg (msg);

  ACE_Time_Value run_time;
  
  static CORBA::ULong prime_number = 9619;

  if (this->start_time_ != 0)
    {
      ACE_DEBUG ((LM_DEBUG, "(%t|%d) about to sleep\n", count_));
      ACE_OS::sleep (this->start_time_);
    }

  ACE_hthread_t thr_handle;
  ACE_Thread::self (thr_handle);
  int prio;

  if (ACE_Thread::getprio (thr_handle, prio) == -1)
    {
      if (errno == ENOTSUP)
	{
	  ACE_DEBUG((LM_DEBUG,
		     ACE_TEXT ("getprio not supported on this platform\n")
		     ));
	  return 0;
	}
      ACE_ERROR_RETURN ((LM_ERROR,
			 ACE_TEXT ("%p\n"),
			 ACE_TEXT ("getprio failed")),
			-1);
    }
  
  ACE_DEBUG ((LM_DEBUG, "(%t) Thread %d prio is %d\n", count_, prio));

  for (int i = 0; i < this->load_; i++)
    {
      //ACE_OS::printf ("MIF_Task:: for loop i=%d\n", i);
      //ACE_DEBUG ((LM_DEBUG, "(%t|%d) prio = %d, for loop i=%d\n", count_, prio, i));

  if (ACE_Thread::getprio (thr_handle, prio) == -1)
    {
      if (errno == ENOTSUP)
	{
	  ACE_DEBUG((LM_DEBUG,
		     ACE_TEXT ("getprio not supported on this platform\n")
		     ));
	  return 0;
	}
      ACE_ERROR_RETURN ((LM_ERROR,
			 ACE_TEXT ("%p\n"),
			 ACE_TEXT ("getprio failed")),
			-1);
    }

      ACE_OS::sprintf( msg, "count = %d, prio = %d, for loop i=%d\n", count_, prio, i);
      dt_creator_->log_msg (msg);
      
      for (int j = 0; j < 500; j++)
	{
	  ACE::is_prime (prime_number,
			 2,
			 prime_number / 2);
	}

      run_time = ACE_OS::gettimeofday () - *base_time_;
      TASK_STATS::instance ()->sample (ACE_UINT64 (run_time.sec ()),
				       count_);
      /*
      CORBA::Policy_var sched_param;
      sched_param = CORBA::Policy::_duplicate (dt_creator_->sched_param (this->importance_));
      const char * name = 0;
      CORBA::Policy_ptr implicit_sched_param = 0;
      current_->update_scheduling_segment (name,
					   sched_param.in (),
					   implicit_sched_param
					   ACE_ENV_ARG_DECL);
      ACE_CHECK;
      */
    }
  return 0;
}

