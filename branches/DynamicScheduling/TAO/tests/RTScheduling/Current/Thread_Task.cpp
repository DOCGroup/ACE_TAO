//$Id$

#include "Thread_Task.h"

int
Thread_Task::activate_task (CORBA::ORB_ptr orb)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  this->orb_ = CORBA::ORB::_duplicate (orb);
  
  CORBA::Object_ptr current_obj = this->orb_->resolve_initial_references ("RTScheduler_Current");
  
  this->current_ = RTScheduling::Current::_narrow (current_obj
						   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  
  long flags = THR_NEW_LWP | THR_JOINABLE;
  if (this->ACE_Task <ACE_SYNCH>::activate (flags,
					    4) == -1)
    {
      if (ACE_OS::last_error () == EPERM)
	ACE_ERROR_RETURN ((LM_ERROR,
			   ACE_TEXT ("Insufficient privilege to run this test.\n")),
			  -1);
    }
  return 0;
}

int
Thread_Task::svc (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  const char * name = 0;
  CORBA::Policy_ptr sched_param = 0;
  CORBA::Policy_ptr implicit_sched_param = 0;
  
  this->current_->begin_scheduling_segment (name,
					    sched_param,
					    implicit_sched_param
					    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  //Start - Nested Scheduling Segment
  this->current_->begin_scheduling_segment (name,
					    sched_param,
					    implicit_sched_param
					    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  
  this->current_->end_scheduling_segment (name
					  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  //End - Nested Scheduling Segment

  this->current_->end_scheduling_segment (name
					  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}



