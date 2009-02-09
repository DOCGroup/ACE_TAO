//$Id$
#include "FP_Task.h"
#include "test.h"
#include "tao/RTCORBA/Priority_Mapping_Manager.h"
#include "tao/RTCORBA/Priority_Mapping.h"
#include "ace/High_Res_Timer.h"
#include "ace/OS_NS_errno.h"
#include "ace/Countdown_Time.h"

FP_Task::FP_Task (int importance,
                  time_t start_time,
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

int
FP_Task::activate_task (RTScheduling::Current_ptr current,
                        CORBA::Policy_ptr sched_param,
                        long flags,
                        ACE_Time_Value* base_time)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "Thread_Task::activate %d\n",
                importance_));

  char msg [BUFSIZ];
  ACE_OS::sprintf (msg, "Thread_Task::activate task\n");
  dt_creator_->log_msg (msg);

  base_time_ = base_time;

  current_ = RTScheduling::Current::_narrow (current);

  sched_param_ = CORBA::Policy::_duplicate (sched_param);

  pre_activate ();

  if (this->activate (flags,
                      1,
                      0,
                      this->importance_) == -1)
    {
      if (ACE_OS::last_error () == EPERM)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Insufficient privilege to run this test.\n")),
                          -1);
    }

  return 0;
}

void
FP_Task::pre_activate (void)
{
  try
    {
      CORBA::Object_var object = DT_TEST::instance ()->orb ()->resolve_initial_references ("PriorityMappingManager");

      RTCORBA::PriorityMappingManager_var mapping_manager =
        RTCORBA::PriorityMappingManager::_narrow (object.in ());

      RTCORBA::PriorityMapping *pm = mapping_manager->mapping ();

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
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
    }
}

int
FP_Task::perform_task (void)
{
  static CORBA::ULong prime_number = 9619;

  for (int j = 0; j < this->iter_; j++)
    {
      ACE_Time_Value run_time = ACE_OS::gettimeofday () - *base_time_;
      TASK_STATS::instance ()->sample (run_time.sec (), count_);

      ACE_Time_Value count_down_time (1);
      ACE_Countdown_Time count_down (&count_down_time);

      while (count_down_time > ACE_Time_Value::zero)
        {
          ACE::is_prime (prime_number,
                         2,
                         prime_number / 2);

          count_down.update ();
        }
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
          TASK_STATS::instance ()->sample (run_time.sec (), count_);

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
