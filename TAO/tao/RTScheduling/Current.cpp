//$Id$
#include "Current.h"
#include "ORB_Core.h"
#include "Distributable_Thread.h"

TAO_Scheduler_Current::TAO_Scheduler_Current (TAO_ORB_Core* orb)
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
    ACE_THROW (CORBA::NO_IMPLEMENT ());
  
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
{
  ACE_DEBUG ((LM_DEBUG,
	      "TAO_RTScheduler_Current_i::TAO_RTScheduler_Current_i\n"));

  ACE_DECLARE_NEW_CORBA_ENV;
  this->orb_ = orb;
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
  
  RTScheduling::Current::IdType guid = 0;
  
  //DT pointer
  RTScheduling::DistributableThread_ptr dt;
  
  if (CORBA::is_nil (this->dt_.in ()))
    {
      //Generate GUID
      guid = ACE_OS::rand (); //Will be replaced by the ACE guid generator
      
      this->scheduler_->begin_new_scheduling_segment (guid,
						      name,
						      sched_param,
						      implicit_sched_param);

      //Create new DT.
      this->dt_ = TAO_DistributableThread_Factory::create_DT ();
      
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
}

void 
TAO_RTScheduler_Current_i::end_scheduling_segment (const char * name
						 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

RTScheduling::DistributableThread_ptr 
TAO_RTScheduler_Current_i::lookup(const RTScheduling::Current::IdType & id
				ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
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
