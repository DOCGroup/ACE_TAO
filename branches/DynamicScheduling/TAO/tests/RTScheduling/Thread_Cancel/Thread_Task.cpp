//$Id$

#include "Thread_Task.h"

int
Thread_Task::activate_task (CORBA::ORB_ptr orb)
{
  this->orb_ = CORBA::ORB::_duplicate (orb);
  
  CORBA::Object_ptr current_obj = this->orb_->resolve_initial_references ("RTScheduler_Current");
  
  this->current_ = RTScheduling::Current::_narrow (current_obj
												   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
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


Thread_Task::svc (void)
{
  ACE_TRY
    {
      const char * name = 0;
      CORBA::Policy_ptr sched_param = 0;
      CORBA::Policy_ptr implicit_sched_param = 0;
      
      
      this->current_->begin_scheduling_segment ("Chamber of Secrets",
						sched_param,
						implicit_sched_param
						ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
  

      //Start - Nested Scheduling Segment
      this->current_->begin_scheduling_segment ("Potter",
						sched_param,
						implicit_sched_param
						ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      
      //Start - Nested Scheduling Segment
      this->current_->begin_scheduling_segment ("Harry",
						sched_param,
						implicit_sched_param
						ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

	  
      RTScheduling::Current::NameList* name_list = this->current_->current_scheduling_segment_names (ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      for (int i = 0; i < name_list->length ();++i)
	{
	  ACE_DEBUG ((LM_DEBUG,
		      "Scheduling Segment Name - %s\n",
		      (*name_list) [i].in ()));
	}
  
      ACE_OS::sleep (50);

      this->current_->end_scheduling_segment (name
					      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      //  End - Nested Scheduling Segment

      
      
      this->current_->end_scheduling_segment (name
					      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      //  End - Nested Scheduling Segment

      this->current_->end_scheduling_segment (name
					      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
      return 1;
    }
  ACE_ENDTRY;
  return 0;
}



