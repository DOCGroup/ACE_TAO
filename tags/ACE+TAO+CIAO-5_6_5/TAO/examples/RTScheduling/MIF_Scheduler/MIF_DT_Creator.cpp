//$Id$

#include "MIF_DT_Creator.h"
#include "test.h"
#include "MIF_Task.h"

MIF_DT_Creator::MIF_DT_Creator (void)
{
  DT_TEST::instance ()->dt_creator (this);
}

Thread_Task*
MIF_DT_Creator::create_thr_task (int importance,
                                 int start_time,
                                 int load,
                                 int iter,
                                 int dist,
                                 char *job_name)
{
  MIF_Task* task;
  ACE_NEW_RETURN (task,
                  MIF_Task (importance,
                            start_time,
                            load,
                            iter,
                            dist,
                            job_name,
                            this),
                  0);
  return task;
}

CORBA::Policy_ptr
MIF_DT_Creator::sched_param (int importance)
{
  return DT_TEST::instance ()->scheduler ()->create_segment_scheduling_parameter (importance);
}

void
MIF_DT_Creator::yield (int suspend_time,
                       Thread_Task*)
{
  try
    {
      CORBA::Object_var object =
        this->orb_->resolve_initial_references ("RTScheduler_Current");
      RTScheduling::Current_var current =
        RTScheduling::Current::_narrow (object.in ());

      ACE_Time_Value const sus_time_value (suspend_time);
      ACE_Time_Value now (ACE_OS::gettimeofday ());
      while (((now - *base_time_) < sus_time_value) || (suspend_time == 1))
        {

          ACE_OS::sleep (1);
          CORBA::Policy_var sched_param;
          sched_param = CORBA::Policy::_duplicate (this->sched_param (100));
          const char * name = 0;
          current->update_scheduling_segment (name,
                                              sched_param.in (),
                                              sched_param.in ());
          now = ACE_OS::gettimeofday ();
          if (suspend_time == 1)
            break;
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
    }
}

int
MIF_DT_Creator::total_load (void)
{
  return 1000;
}

void
MIF_DT_Creator::wait (void)
{
  while (active_dt_count_ > 0 || active_job_count_ > 0)
    {
      yield(1,0);
    }
}

ACE_STATIC_SVC_DEFINE(MIF_DT_Creator,
                      ACE_TEXT ("MIF_DT_Creator"),
                      ACE_SVC_OBJ_T,
                      &ACE_SVC_NAME (MIF_DT_Creator),
                      ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                      0)

ACE_FACTORY_DEFINE (ACE_Local_Service, MIF_DT_Creator)
