//$Id$
#include "MIF_Task.h"
#include "test.h"

MIF_Task::MIF_Task (int importance,
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
  this->job_name_ = CORBA::string_dup (job_name);

  // create the stat object.
  ACE_NEW (task_stats_, Task_Stats);
  task_stats_->init (iter_);

}

MIF_Task::~MIF_Task (void)
{
  delete task_stats_;
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
MIF_Task::activate_task (RTScheduling::Current_ptr current,
			 CORBA::Policy_ptr sched_param,
			 long flags,
			 ACE_Time_Value* base_time
			 ACE_ENV_ARG_DECL)
{
  
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
		"Thread_Task::activate %d\n",
		importance_));
  
  char msg [BUFSIZ];
  ACE_OS::sprintf (msg, "Thread_Task::activate task\n");
  dt_creator_->log_msg (msg);
   
  base_time_ = base_time;

  current_ = RTScheduling::Current::_narrow (current
					     ACE_ENV_ARG_PARAMETER);	
  ACE_CHECK_RETURN (-1);

  sched_param_ = CORBA::Policy::_duplicate (sched_param);

  pre_activate ();

  if (this->activate (flags,
		      1) == -1)
    {
      if (ACE_OS::last_error () == EPERM)
	ACE_ERROR_RETURN ((LM_ERROR,
			   ACE_TEXT ("Insufficient privilege to run this test.\n")),
			  -1);
    }
  
  post_activate ();
  return 0;
}

int
MIF_Task::perform_task (void)
{
  ACE_TRY_NEW_ENV
    {
      
      char msg [BUFSIZ];
      ACE_OS::sprintf (msg, "MIF_Task::perform_task %d\n", count_);
      dt_creator_->log_msg (msg);

      static CORBA::ULong prime_number = 9619;
      CORBA::Policy_var sched_param;
      sched_param = CORBA::Policy::_duplicate (dt_creator_->sched_param (this->importance_));
      const char * name = 0;

      for (int i = 0; i < this->iter_; i++)
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

          current_->update_scheduling_segment (name,
                                               sched_param.in (),
                                               sched_param.in ()
                                               ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      if (this->dist_)
        {
          this->job_->work (this->load_,
                            this->importance_);

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

              current_->update_scheduling_segment (name,
                                                   sched_param.in (),
                                                   sched_param.in ()
                                                   ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

            }

        }

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
      return -1;
    }
  ACE_ENDTRY;
  
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
		"Thread %d\n",
		this->count_));

  if (dist_)
    job_->shutdown ();

  return 0;
}
