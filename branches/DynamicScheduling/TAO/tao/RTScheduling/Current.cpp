//$Id$
#include "Current.h"
#include "tao/ORB_Core.h"
#include "Distributable_Thread.h"



TAO_RTScheduler_Current::TAO_RTScheduler_Current (TAO_ORB_Core* orb)
{
	this->orb_ = orb;
}

void
TAO_RTScheduler_Current::rt_current (RTCORBA::Current_ptr rt_current)
{
	this->rt_current_ = RTCORBA::Current::_duplicate (rt_current);


}

void
TAO_RTScheduler_Current::begin_scheduling_segment(const char * name,
						  CORBA::Policy_ptr sched_param,
						  CORBA::Policy_ptr implicit_sched_param
						  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   RTScheduling::Current::UNSUPPORTED_SCHEDULING_DISCIPLINE))
{
  ACE_DEBUG ((LM_DEBUG,
	      "TAO_RTScheduler_Current::begin_scheduling_segment\n"));

  TAO_RTScheduler_Current_i *impl = this->implementation ();
  
	  if (impl == 0)
		ACE_THROW (CORBA::NO_IMPLEMENT ());
  
  impl->begin_scheduling_segment (name,
				  sched_param,
				  implicit_sched_param
				  ACE_ENV_ARG_PARAMETER);
}


void 
TAO_RTScheduler_Current::update_scheduling_segment (const char * name,
						    CORBA::Policy_ptr sched_param,
						    CORBA::Policy_ptr implicit_sched_param
						    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   RTScheduling::Current::UNSUPPORTED_SCHEDULING_DISCIPLINE))
{
  TAO_RTScheduler_Current_i *impl = this->implementation ();
  
  if (impl == 0)
    ACE_THROW (CORBA::NO_IMPLEMENT ());
  
  impl->update_scheduling_segment (name,
				   sched_param,
				   implicit_sched_param
				   ACE_ENV_ARG_PARAMETER);
  
}

void 
TAO_RTScheduler_Current::end_scheduling_segment (const char * name
						 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  ACE_DEBUG ((LM_DEBUG,
		      "TAO_RTScheduler_Current::end_scheduling_segment\n"));

  TAO_RTScheduler_Current_i *impl = this->implementation ();
  
  if (impl == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
		  "Probably No Matching begin_scheduling_segment/n"));
		  
      ACE_THROW (CORBA::NO_IMPLEMENT ());
    }
  
  impl->end_scheduling_segment (name
				ACE_ENV_ARG_PARAMETER);
}

RTScheduling::DistributableThread_ptr 
TAO_RTScheduler_Current::lookup(const RTScheduling::Current::IdType & id
				ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_RTScheduler_Current_i *impl = this->implementation ();
  
  if (impl == 0)
    ACE_THROW (CORBA::NO_IMPLEMENT ());
  
  return impl->lookup (id
		       ACE_ENV_ARG_PARAMETER);
}

// returns a null reference if
// the distributable thread is
// not known to the local scheduler

RTScheduling::DistributableThread_ptr
TAO_RTScheduler_Current::spawn (RTScheduling::ThreadAction_ptr start,
				CORBA::ULong stack_size,
				RTCORBA::Priority base_priority
				ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_RTScheduler_Current_i *impl = this->implementation ();
  
  if (impl == 0)
    ACE_THROW (CORBA::NO_IMPLEMENT ());
  
  return impl->spawn (start,
					stack_size,
					base_priority
		       ACE_ENV_ARG_PARAMETER);
}

RTScheduling::Current::IdType * 
TAO_RTScheduler_Current::id (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_RTScheduler_Current_i *impl = this->implementation ();
  
  if (impl == 0)
    ACE_THROW (CORBA::NO_IMPLEMENT ());
  
  return impl->id (ACE_ENV_ARG_PARAMETER);
}

CORBA::Policy_ptr 
TAO_RTScheduler_Current::scheduling_parameter (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_RTScheduler_Current_i *impl = this->implementation ();
  
  if (impl == 0)
    ACE_THROW (CORBA::NO_IMPLEMENT ());
  
  return impl->scheduling_parameter (ACE_ENV_ARG_PARAMETER);
}

CORBA::Policy_ptr 
TAO_RTScheduler_Current::implicit_scheduling_parameter (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_RTScheduler_Current_i *impl = this->implementation ();
  
  if (impl == 0)
    ACE_THROW (CORBA::NO_IMPLEMENT ());
  
  return impl->implicit_scheduling_parameter (ACE_ENV_ARG_PARAMETER);
}

RTScheduling::Current::NameList * 
TAO_RTScheduler_Current::current_scheduling_segment_names (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_RTScheduler_Current_i *impl = this->implementation ();
  
  if (impl == 0)
    ACE_THROW (CORBA::NO_IMPLEMENT ());
  
  return impl->current_scheduling_segment_names (ACE_ENV_ARG_PARAMETER);
}

RTCORBA::Priority 
TAO_RTScheduler_Current::the_priority (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
	return this->rt_current_->the_priority ();
}

void 
TAO_RTScheduler_Current::the_priority (RTCORBA::Priority the_priority
                             ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
	this->rt_current_->the_priority(the_priority);
}

TAO_RTScheduler_Current_i*
TAO_RTScheduler_Current::implementation (TAO_RTScheduler_Current_i* new_current)
{
  TAO_TSS_Resources *tss =
    TAO_TSS_RESOURCES::instance ();
  
  TAO_RTScheduler_Current_i *old =
    ACE_static_cast (TAO_RTScheduler_Current_i *,
                     tss->rtscheduler_current_impl_);
  tss->rtscheduler_current_impl_ = new_current;
  return old;
}

TAO_RTScheduler_Current_i*
TAO_RTScheduler_Current::implementation (void)
{
  TAO_TSS_Resources *tss =
    TAO_TSS_RESOURCES::instance ();

  TAO_RTScheduler_Current_i* impl = ACE_static_cast (TAO_RTScheduler_Current_i *,
						     tss->rtscheduler_current_impl_);

  if (impl == 0)
    {
      ACE_NEW_THROW_EX (impl,
			TAO_RTScheduler_Current_i (this->orb_),
			CORBA::NO_MEMORY (
					  CORBA::SystemException::_tao_minor_code (
					  TAO_DEFAULT_MINOR_CODE,
					  ENOMEM),
					  CORBA::COMPLETED_NO));
      ACE_CHECK;

      tss->rtscheduler_current_impl_ = impl;
    }

  return impl;

}

TAO_RTScheduler_Current_i::TAO_RTScheduler_Current_i (TAO_ORB_Core* orb)
  :orb_ (orb),
   dt_ (RTScheduling::DistributableThread::_nil ()),
   previous_current_ (0)
{
  ACE_DEBUG ((LM_DEBUG,
	      "TAO_RTScheduler_Current_i::TAO_RTScheduler_Current_i\n"));

  ACE_DECLARE_NEW_CORBA_ENV;

  CORBA::Object_ptr scheduler_obj = this->orb_->object_ref_table ().resolve_initial_references ("RTScheduler"
												ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  this->scheduler_ = RTScheduling::Scheduler::_narrow (scheduler_obj
						       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

TAO_RTScheduler_Current_i::TAO_RTScheduler_Current_i (TAO_ORB_Core* orb,
			   RTScheduling::Current::IdType guid,
			   const char * name,
			   CORBA::Policy_ptr sched_param,
			   CORBA::Policy_ptr implicit_sched_param,
			   RTScheduling::DistributableThread_ptr dt,
			   TAO_RTScheduler_Current_i* prev_current)
  : orb_ (orb),
    guid_ (guid),
    name_ (name),
    sched_param_ (sched_param),
    implicit_sched_param_ (implicit_sched_param),
    dt_ (RTScheduling::DistributableThread::_duplicate (dt)),
    previous_current_ (prev_current)
{
  CORBA::Object_ptr scheduler_obj = this->orb_->object_ref_table ().resolve_initial_references ("RTScheduler"
												ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  this->scheduler_ = RTScheduling::Scheduler::_narrow (scheduler_obj
						       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

}

void
TAO_RTScheduler_Current_i::begin_scheduling_segment(const char * name,
						    CORBA::Policy_ptr sched_param,
						    CORBA::Policy_ptr implicit_sched_param
						    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   RTScheduling::Current::UNSUPPORTED_SCHEDULING_DISCIPLINE))
{
    
  if (CORBA::is_nil (this->dt_.in ())) // Check if it is a new Scheduling Segmnet
    {
      //Generate GUID
      //char buf [BUFSIZ];
      //ACE_OS::itoa (ACE_OS::rand (), buf, 10);

      int guid = ACE_OS::rand (); //Will be replaced by the ACE guid generator
      
      this->guid_.length (sizeof guid);
	
      ACE_OS::memcpy (this->guid_.get_buffer (),
		      &guid,
		      sizeof guid);

      ACE_DEBUG ((LM_DEBUG,
		  "The Guid is %d\n",
		  (const int*) this->guid_.get_buffer ()));

      this->scheduler_->begin_new_scheduling_segment (this->guid_,
						      name,
						      sched_param,
						      implicit_sched_param
						      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      //Create new DT.
      this->dt_ = TAO_DistributableThread_Factory::create_DT ();
      
      //Add new DT to map
      int result = this->orb_->dt_hash ()->bind (this->guid_,
						 this->dt_);

      if (result != 0)
	{
	  this->cancel_thread (ACE_ENV_ARG_PARAMETER);
	  ACE_CHECK;
	}
      else if (result == 1)
	{
	  ACE_DEBUG ((LM_DEBUG,
		      "Entry Exists\n"));
	}
      
      this->name_ = name;
      this->sched_param_ = sched_param;
      this->implicit_sched_param_ = implicit_sched_param;      
      
    }
  else //Nested segment
    {
      if (this->dt_->state () == RTScheduling::DistributableThread::CANCELLED)
	{
	  this->cancel_thread (ACE_ENV_ARG_PARAMETER);
	  ACE_CHECK;
	}
      
      // Inform scheduler of start of nested
      // scheduling segment.
      this->scheduler_->begin_nested_scheduling_segment
	(this->guid_,
	 name,
	 sched_param,
	 implicit_sched_param
	 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      
      TAO_TSS_Resources *tss =
	TAO_TSS_RESOURCES::instance ();
      
      TAO_RTScheduler_Current_i* new_current;
	ACE_NEW_THROW_EX (new_current,
			  TAO_RTScheduler_Current_i (this->orb_,
						     this->guid_,
						     name,
						     sched_param,
						     implicit_sched_param,
						     this->dt_,
						     this),
			  CORBA::NO_MEMORY (
					    CORBA::SystemException::_tao_minor_code (
					  TAO_DEFAULT_MINOR_CODE,
					  ENOMEM),
					  CORBA::COMPLETED_NO));
      ACE_CHECK;

      tss->rtscheduler_current_impl_ = new_current;
    }
}


void 
TAO_RTScheduler_Current_i::update_scheduling_segment (const char * name,
						      CORBA::Policy_ptr sched_param,
						      CORBA::Policy_ptr implicit_sched_param
						      ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   RTScheduling::Current::UNSUPPORTED_SCHEDULING_DISCIPLINE))
{
  // Check if DT has been cancelled
  if (this->dt_->state () == RTScheduling::DistributableThread::CANCELLED)
    {
      this->cancel_thread (ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  
    
  // Let scheduler know of the updates.
  this->scheduler_->update_scheduling_segment (this->guid_,
					       name,
					       sched_param,
					       implicit_sched_param
					       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  // Remember the new values.
  this->sched_param_ = sched_param;
  this->implicit_sched_param_ = implicit_sched_param;
  this->name_ = name;
}

void 
TAO_RTScheduler_Current_i::end_scheduling_segment (const char * name
						   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (!CORBA::is_nil (this->dt_.in ()))
    {
      // Check if DT has been cancelled
      if (this->dt_->state () == RTScheduling::DistributableThread::CANCELLED)
	{
	  this->cancel_thread (ACE_ENV_ARG_PARAMETER);
	  ACE_CHECK;
	}

      if (this->previous_current_ == 0)
	{
	  // Let the scheduler know that the DT is
	  // terminating.
	  this->scheduler_->end_scheduling_segment(this->guid_,
						   name
						   ACE_ENV_ARG_PARAMETER);
	  ACE_CHECK;
	  
	  // Cleanup DT.
	  this->cleanup_DT ();

	  // Cleanup current.
	  this->cleanup_current ();

	} else { // A Nested segment.
	  
	  // Inform scheduler of end of nested
	  // scheduling segment.
	  this->scheduler_->end_nested_scheduling_segment (this->guid_,
							   name,
							   this->previous_current_->sched_param_
							   ACE_ENV_ARG_PARAMETER);
	  ACE_CHECK;
	  
	  // Cleanup current.
	  this->cleanup_current ();
	}
    }
  else ACE_DEBUG ((LM_DEBUG,
		   "No matching begin_scheduling_segment\n"));
}

RTScheduling::DistributableThread_ptr 
TAO_RTScheduler_Current_i::lookup(const RTScheduling::Current::IdType & id
				ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Object_ptr DT_obj;
  this->orb_->dt_hash ()->find (id,
								DT_obj);
  
  RTScheduling::DistributableThread_var DT = RTScheduling::DistributableThread::_narrow (DT_obj
											 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  if (!CORBA::is_nil (DT.in ()))
    return RTScheduling::DistributableThread::_duplicate (DT.in());
  
  return 0;
}

// returns a null reference if
// the distributable thread is
// not known to the local scheduler

RTScheduling::DistributableThread_ptr
TAO_RTScheduler_Current_i::spawn (RTScheduling::ThreadAction_ptr start,
				CORBA::ULong stack_size,
				RTCORBA::Priority base_priority
				ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
		return 0;
}

RTScheduling::Current::IdType * 
TAO_RTScheduler_Current_i::id (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
		return 0;
}


CORBA::Policy_ptr 
TAO_RTScheduler_Current_i::scheduling_parameter (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
		return 0;
}

CORBA::Policy_ptr 
TAO_RTScheduler_Current_i::implicit_scheduling_parameter (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
		return 0;
}

RTScheduling::Current::NameList * 
TAO_RTScheduler_Current_i::current_scheduling_segment_names (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
		return 0;
}

void
TAO_RTScheduler_Current_i::cancel_thread (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
	      "Distributable Thread - %d is cancelled\n",
	      (const int*) this->guid_.get_buffer ()));
 
  // Let the scheduler know that the thread has
  // been cancelled.
  this->scheduler_->cancel (this->guid_
			    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  this->cleanup_DT ();
  
  // Remove all related nested currents.
  this->delete_all_currents ();
  
  // Throw exception.
  ACE_THROW (CORBA::THREAD_CANCELLED ());
}

void
TAO_RTScheduler_Current_i::cleanup_DT (void)
{
  // Remove DT from map.
  this->orb_->dt_hash ()->unbind (this->guid_);
}

void
TAO_RTScheduler_Current_i::cleanup_current (void)
{
  TAO_TSS_Resources *tss =
    TAO_TSS_RESOURCES::instance ();
  
  tss->rtscheduler_current_impl_ = this->previous_current_;  

  // Delete this current.
  delete this;
}

void
TAO_RTScheduler_Current_i::delete_all_currents (void)
{
  TAO_RTScheduler_Current_i* current = this;
  
  while (current != 0)
    {
      TAO_RTScheduler_Current_i* prev_current = this->previous_current_;
      current->cleanup_current ();
      current = prev_current;
    }
}

