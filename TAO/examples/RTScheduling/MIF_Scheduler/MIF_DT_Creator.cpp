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
  //ACE_Time_Value suspend (suspend_time);
  ACE_Time_Value now (ACE_OS::gettimeofday ());
  while (((now - *base_time_) < suspend_time) || (suspend_time == 1))
    {

      ACE_OS::sleep (1);
      CORBA::Policy_var sched_param;
      sched_param = CORBA::Policy::_duplicate (this->sched_param (100));
      const char * name = 0;
      CORBA::Policy_ptr implicit_sched_param = 0;
      current_->update_scheduling_segment (name,
					   sched_param.in (),
					   sched_param.in ()
					   ACE_ENV_ARG_DECL);
      ACE_CHECK;
      now = ACE_OS::gettimeofday ();
      if (suspend_time == 1)
	  break;

    }
}

int
MIF_DT_Creator::total_load (void)
{
  return 1000;
}

ACE_STATIC_SVC_DEFINE(MIF_DT_Creator,
                      ACE_TEXT ("MIF_DT_Creator"),
                      ACE_SVC_OBJ_T,
                      &ACE_SVC_NAME (MIF_DT_Creator),
                      ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                      0)

ACE_FACTORY_DEFINE (MIF_DT_Creator, MIF_DT_Creator)
