//$Id$

#include "Scheduler.h"

CORBA::PolicyList* 
TAO_Scheduler::scheduling_policies (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
	return 0;
}
    
void 
TAO_Scheduler::scheduling_policies (const CORBA::PolicyList &
				    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}
      
CORBA::PolicyList* 
TAO_Scheduler::poa_policies (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
	return 0;
}
    
char * 
TAO_Scheduler::scheduling_discipline_name (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
	return 0;
}
    
RTScheduling::ResourceManager_ptr 
TAO_Scheduler::create_resource_manager (const char *,
					CORBA::Policy_ptr
					ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
	return 0;
}
    
void 
TAO_Scheduler::set_scheduling_parameter (PortableServer::Servant &,
					 const char *,
					 CORBA::Policy_ptr
					 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}
  
void 
TAO_Scheduler::begin_new_scheduling_segment (const RTScheduling::Current::IdType &,
					     const char *,
					     CORBA::Policy_ptr,
					     CORBA::Policy_ptr
					     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   RTScheduling::Current::UNSUPPORTED_SCHEDULING_DISCIPLINE))
{
}
    
void 
TAO_Scheduler::begin_nested_scheduling_segment (const RTScheduling::Current::IdType &,
						const char *,
						CORBA::Policy_ptr,
						CORBA::Policy_ptr
						ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException, 
		   RTScheduling::Current::UNSUPPORTED_SCHEDULING_DISCIPLINE))
{
}
    
void 
TAO_Scheduler::update_scheduling_segment (const RTScheduling::Current::IdType &,
					  const char *,
					  CORBA::Policy_ptr,
					  CORBA::Policy_ptr
					  ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
		     RTScheduling::Current::UNSUPPORTED_SCHEDULING_DISCIPLINE))
{
}
    
void 
TAO_Scheduler::end_scheduling_segment (const RTScheduling::Current::IdType &,
				       const char *
				       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void 
TAO_Scheduler::end_nested_scheduling_segment (const RTScheduling::Current::IdType &,
					      const char *,
					      CORBA::Policy_ptr
					      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}
  

void 
TAO_Scheduler::send_request (PortableInterceptor::ClientRequestInfo_ptr
			     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   PortableInterceptor::ForwardRequest))
{
}

void 
TAO_Scheduler::receive_request (PortableInterceptor::ServerRequestInfo_ptr,
				const RTScheduling::Current::IdType &,
				const char *,
				CORBA::Policy_ptr,
				CORBA::Policy_ptr
				ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   PortableInterceptor::ForwardRequest))
{
}

void 
TAO_Scheduler::send_reply (PortableInterceptor::ServerRequestInfo_ptr
			   ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void 
TAO_Scheduler::send_exception (PortableInterceptor::ServerRequestInfo_ptr
			       ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   PortableInterceptor::ForwardRequest))
{
}

void 
TAO_Scheduler::send_other (PortableInterceptor::ServerRequestInfo_ptr
			   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   PortableInterceptor::ForwardRequest))
{
}

void 
TAO_Scheduler::receive_reply (PortableInterceptor::ClientRequestInfo_ptr
			      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}
    
void 
TAO_Scheduler::receive_exception (PortableInterceptor::ClientRequestInfo_ptr
				  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   PortableInterceptor::ForwardRequest))
{
}

void 
TAO_Scheduler::receive_other (PortableInterceptor::ClientRequestInfo_ptr
			      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   PortableInterceptor::ForwardRequest))
{
}
    
void 
TAO_Scheduler::cancel (const RTScheduling::Current::IdType &
		       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}
