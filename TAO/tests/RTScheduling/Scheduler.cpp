//$Id$

#include "Scheduler.h"
#include "ace/Atomic_Op.h"

ACE_Atomic_Op<ACE_Thread_Mutex, long> server_guid_counter;

TAO_Scheduler::TAO_Scheduler (void)
{
}

TAO_Scheduler::~TAO_Scheduler (void)
{
}

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
				RTScheduling::Current::IdType_out guid_out,
				CORBA::String_out name,
				CORBA::Policy_out sched_param,
				CORBA::Policy_out implicit_sched_param
				ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   PortableInterceptor::ForwardRequest))
{
  //***************************************
  RTScheduling::Current::IdType* guid;
  ACE_NEW (guid,
	   RTScheduling::Current::IdType);
	   
		
  // Generate GUID.
  guid->length (sizeof(long));
  
  long temp = ++server_guid_counter;
  ACE_OS::memcpy (guid->get_buffer (),
		  &temp,
		  sizeof(long));
      
  int id;
  ACE_OS::memcpy (&id,
		  guid->get_buffer (),
		  guid->length ());
  
  ACE_DEBUG ((LM_DEBUG,
	      "The Guid is %d %d\n",
	      id,
	      server_guid_counter.value_i ()));

  guid_out.ptr () = guid;
  //***************************************
  
  
}

void 
TAO_Scheduler::send_reply (PortableInterceptor::ServerRequestInfo_ptr
			   ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void 
TAO_Scheduler::send_exception (PortableInterceptor::ServerRequestInfo_ptr
			       ACE_ENV_ARG_DECL)
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
