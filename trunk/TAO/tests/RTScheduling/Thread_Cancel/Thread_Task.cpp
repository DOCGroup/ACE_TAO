//$Id$

#include "Thread_Task.h"
#include "ace/Atomic_Op.h"

ACE_Atomic_Op<TAO_SYNCH_MUTEX, long> guid_index;

RTScheduling::Current::IdType*
Thread_Task::guids (void)
{
  return this->guid_;
}

int
Thread_Task::activate_task (CORBA::ORB_ptr orb)
{
  ACE_NEW_RETURN (shutdown_lock_,
		  ACE_Lock_Adapter <TAO_SYNCH_MUTEX>,
		  -1);

  ACE_NEW_RETURN (lock_,
		  ACE_Lock_Adapter <TAO_SYNCH_MUTEX>,
		  -1);
   
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
  active_thread_count_ = 4;
  return 0;
}


int
Thread_Task::svc (void)
{
  ACE_TRY
    {
      const char * name = 0;
      CORBA::Policy_ptr sched_param = 0;
      CORBA::Policy_ptr implicit_sched_param = 0;
      
      //Start - Nested Scheduling Segment      
      this->current_->begin_scheduling_segment ("Chamber of Secrets",
						sched_param,
						implicit_sched_param
						ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
  
      int count ;
      ACE_OS::memcpy (&count,
		      current_->id ()->get_buffer (),
		      current_->id ()->length ());


      this->current_->begin_scheduling_segment ("Potter",
						sched_param,
						implicit_sched_param
						ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->guid_[guid_index++] = *(this->current_->id ());
      
      //Start - Nested Scheduling Segment
      this->current_->begin_scheduling_segment ("Harry",
						sched_param,
						implicit_sched_param
						ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;


      {
	ACE_GUARD_RETURN (ACE_Lock, ace_mon, *shutdown_lock_,-1); 	  
	RTScheduling::Current::NameList* name_list = this->current_->current_scheduling_segment_names (ACE_ENV_ARG_PARAMETER);
	ACE_TRY_CHECK;
	
	ACE_DEBUG ((LM_DEBUG,
		    "Scheduling Segments for DT %d :\n",
		    count));

	for (unsigned int i = 0; i < name_list->length ();++i)
	  {
	    ACE_DEBUG ((LM_DEBUG,
			"Scheduling Segment Name - %s\n",
			(*name_list) [i].in ()));
	  }
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
      {
	ACE_GUARD_RETURN (ACE_Lock, ace_mon, *shutdown_lock_,-1); 
	--active_thread_count_;
	if (active_thread_count_ == 0)
	  orb_->shutdown ();
	return 0;
      }

      return 1;
    }
  ACE_ENDTRY;
  return 0;
}




