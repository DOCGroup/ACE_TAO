//$Id$

#include "Thread_Task.h"

#include "ace/Atomic_Op.h"

ACE_Atomic_Op<ACE_Thread_Mutex, long> thread_count = 0;

Thread_Task::Thread_Task (RTScheduling::Current_ptr current)
{
  this->current_ = RTScheduling::Current::_narrow (current);
}

int
Thread_Task::activate_task (CORBA::Policy_ptr sched_param,
			    long flags)
{
  this->sched_param_ = CORBA::Policy::_duplicate (sched_param);

  if (this->ACE_Task <ACE_SYNCH>::activate (flags,
					    1) == -1)
    {
      if (ACE_OS::last_error () == EPERM)
	ACE_ERROR_RETURN ((LM_ERROR,
			   ACE_TEXT ("Insufficient privilege to run this test.\n")),
			  -1);
    }
  return 0;
}


Thread_Task::svc (void)
{

  thread_count++;

  const char * name = 0;
  CORBA::Policy_ptr implicit_sched_param = 0;
  
  this->current_->begin_scheduling_segment (name,
					    this->sched_param_,
					    implicit_sched_param
					    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  for (int i = 0; i < 500; i++)
    {
      ACE_DEBUG ((LM_DEBUG,
				  "%d\n", thread_count));

	  ACE_OS::sleep (2);
		
    }
  /*
  //Start - Nested Scheduling Segment
  this->current_->begin_scheduling_segment (name,
					    sched_param,
					    implicit_sched_param
					    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  this->current_->end_scheduling_segment (name
					  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  //End - Nested Scheduling Segment

  */

  this->current_->end_scheduling_segment (name
					  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  return 0;
}



