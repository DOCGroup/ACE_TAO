//$Id$

#include "FP_DT_Creator.h"
#include "test.h"
#include "FP_Task.h"

FP_DT_Creator::FP_DT_Creator (void)
{
  DT_TEST::instance ()->dt_creator (this);

}

Thread_Task* 
FP_DT_Creator::create_thr_task (int importance,
				int start_time,
				int load,
				int iter,
				int dist,
				char *job_name)
{
  FP_Task* task;
  ACE_NEW_RETURN (task, 
		  FP_Task (importance,
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
FP_DT_Creator::sched_param (int importance)
{
  return DT_TEST::instance ()->scheduler ()->create_segment_scheduling_parameter (importance);
}


void
FP_DT_Creator::yield (int suspend_time,
		      Thread_Task*)
{
  ACE_OS::sleep (suspend_time);
}

int
FP_DT_Creator::total_load (void)
{
  return BUFSIZ;
}

void
FP_DT_Creator::wait (void)
{
}

ACE_STATIC_SVC_DEFINE(FP_DT_Creator,
                      ACE_TEXT ("FP_DT_Creator"),
                      ACE_SVC_OBJ_T,
                      &ACE_SVC_NAME (FP_DT_Creator),
                      ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                      0)

ACE_FACTORY_DEFINE (FP_DT_Creator, FP_DT_Creator)


