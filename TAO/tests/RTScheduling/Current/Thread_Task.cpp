//$Id$

#include "Thread_Task.h"


int
Thread_Task::activate_task (CORBA::ORB_ptr orb,
			    int thr_count)
{
  
  ACE_TRY_NEW_ENV
    {
      ACE_DECLARE_NEW_CORBA_ENV;
      ACE_NEW_RETURN (shutdown_lock_,
		      TAO_SYNCH_MUTEX,
		      -1);

      ACE_NEW_RETURN (lock_,
		      TAO_SYNCH_MUTEX,
		      -1);
 
      active_thread_count_ = thr_count;

      this->orb_ = CORBA::ORB::_duplicate (orb);
  
      CORBA::Object_ptr current_obj = this->orb_->resolve_initial_references ("RTScheduler_Current");
  
      this->current_ = RTScheduling::Current::_narrow (current_obj
						       ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
  
      const char * name = 0;
      CORBA::Policy_ptr sched_param = 0;
      CORBA::Policy_ptr implicit_sched_param = 0;
  
      ACE_TRY_EX(yamuna_give_this_a_better_name_1)
	{
	  ACE_DEBUG ((LM_DEBUG,
		      "Making an end_scheduling_segment call without first calling begin_scheduling_segment\n"));
	  this->current_->end_scheduling_segment (name
						  ACE_ENV_ARG_PARAMETER);
	  ACE_TRY_CHECK_EX(yamuna_give_this_a_better_name_1);
	}
      ACE_CATCHANY
	{
	  ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
			       "End Scheduling Segment is out of context");
	}
      ACE_ENDTRY;

      ACE_TRY_EX(yamuna_give_this_a_better_name_2)
	{
	  ACE_DEBUG ((LM_DEBUG,
		      "Making an update_scheduling_segment call without first calling begin_scheduling_segment\n"));

	  this->current_->update_scheduling_segment (name,
						     sched_param,
						     implicit_sched_param
						     ACE_ENV_ARG_PARAMETER);
	  ACE_TRY_CHECK_EX(yamuna_give_this_a_better_name_2);
	}
      ACE_CATCHANY
	{
	  ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
			       "Update Scheduling Segment is out of context:");
	}
      ACE_ENDTRY;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
			   "Exception:");
    }
  ACE_ENDTRY;
  
  long flags = THR_NEW_LWP | THR_JOINABLE;
  if (this->activate (flags,
		      thr_count) == -1)
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
  ACE_TRY_NEW_ENV
    {

      const char * name = 0;
      CORBA::Policy_ptr sched_param = 0;
      CORBA::Policy_ptr implicit_sched_param = 0;
      
      this->current_->begin_scheduling_segment ("Fellowship of the Rings",
						sched_param,
						implicit_sched_param
						ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      int count ;
      ACE_OS::memcpy (&count,
		      this->current_->id ()->get_buffer (),
		      this->current_->id ()->length ());
      
      ACE_DEBUG ((LM_DEBUG,
		  "Starting Distributable Thread %d with 3 nested scheduling segments....\n",
		  count));
      
      //Start - Nested Scheduling Segment
      this->current_->begin_scheduling_segment ("Two Towers",
						sched_param,
						implicit_sched_param
						ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      
      //Start - Nested Scheduling Segment
      this->current_->begin_scheduling_segment ("The Return of the King",
						sched_param,
						implicit_sched_param
						ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;


      RTScheduling::Current::NameList* segment_name_list = 
	this->current_->current_scheduling_segment_names (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      
      {
	ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, *lock_, -1);
	ACE_DEBUG ((LM_DEBUG,
		    "Segment Names for DT %d :\n",
		    count));
	
	for (unsigned int i = 0; i < segment_name_list->length (); i ++)
	  {
	    ACE_DEBUG ((LM_DEBUG,
			"%s\n",
			CORBA::string_dup ((*segment_name_list) [i])));
	  }
      }
      
      this->current_->end_scheduling_segment (name
					      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      //End - Nested Scheduling Segment
  
      this->current_->end_scheduling_segment (name
					      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      
      this->current_->end_scheduling_segment (name
					      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      //End - Nested Scheduling Segment

      ACE_DEBUG ((LM_DEBUG,
		  "DT %d terminated ...\n",
		  count));
      
      {
	ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, *shutdown_lock_,-1); 
	--active_thread_count_;
	if (active_thread_count_ == 0)
	  orb_->shutdown ();
	return 0;
      }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
			   "Caught exception:");
      return -1;
    }
  ACE_ENDTRY;
}

Thread_Task::~Thread_Task (void)
{
  delete shutdown_lock_;
  delete lock_;
}




