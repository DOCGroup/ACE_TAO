//$Id$
#include "Current.h"
#include "ORB_Core.h"

TAO_Scheduler_Current::TAO_Scheduler_Current (void)
{
  this->rt_current_ = 
}

void
TAO_RTScheduler_Current::begin_scheduling_segment(const char * name,
						  CORBA::Policy_ptr sched_param,
						  CORBA::Policy_ptr implicit_sched_param
						  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   RTScheduling::Current::UNSUPPORTED_SCHEDULING_DISCIPLINE))
{
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
	return RTCORBA::Current::the_priority ();
}

void 
TAO_RTScheduler_Current::the_priority (RTCORBA::Priority the_priority
                             ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
	this->RTScheduling::Current::the_priority(the_priority);
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

TAO_RTScheduler_Current_i::TAO_RTScheduler_Current_i (void)
{
  ACE_DEBUG ((LM_DEBUG,
			  "TAO_RTScheduler_Current_i::TAO_RTScheduler_Current_i\n"));
}

void
TAO_RTScheduler_Current_i::begin_scheduling_segment(const char * name,
						  CORBA::Policy_ptr sched_param,
						  CORBA::Policy_ptr implicit_sched_param
						  ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   RTScheduling::Current::UNSUPPORTED_SCHEDULING_DISCIPLINE))
{
  
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
