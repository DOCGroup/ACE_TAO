//$Id$
#include "Current.h"

void
TAO_RTScheduler_Current::begin_scheduling_segment(const char * name,
						  CORBA::Policy_ptr sched_param,
						  CORBA::Policy_ptr implicit_sched_param
						  ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   RTScheduling::Current::UNSUPPORTED_SCHEDULING_DISCIPLINE))
{
}

   
void 
TAO_RTScheduler_Current::update_scheduling_segment (const char * name,
						    CORBA::Policy_ptr sched_param,
						    CORBA::Policy_ptr implicit_sched_param
						    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   RTScheduling::Current::UNSUPPORTED_SCHEDULING_DISCIPLINE))
{
}

void 
TAO_RTScheduler_Current::end_scheduling_segment (const char * name
						 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

RTScheduling::DistributableThread_ptr 
TAO_RTScheduler_Current::lookup(const RTScheduling::Current::IdType & id
				ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
	return 0;
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
		return 0;
}

RTScheduling::Current::IdType * 
TAO_RTScheduler_Current::id (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
		return 0;
}

CORBA::Policy_ptr 
TAO_RTScheduler_Current::scheduling_parameter (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
		return 0;
}

CORBA::Policy_ptr 
TAO_RTScheduler_Current::implicit_scheduling_parameter (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
		return 0;
}

RTScheduling::Current::NameList * 
TAO_RTScheduler_Current::current_scheduling_segment_names (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
		return 0;
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
