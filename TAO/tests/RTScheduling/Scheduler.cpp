//$Id$

#include "Scheduler.h"
#include "ace/Atomic_Op.h"
#include "tao/RTScheduling/Request_Interceptor.h"

ACE_Atomic_Op<ACE_Thread_Mutex, long> server_guid_counter;

TAO_Scheduler::TAO_Scheduler (CORBA::ORB_ptr orb)
{
  CORBA::Object_ptr current_obj = orb->resolve_initial_references ("RTScheduler_Current");
  
  current_ = RTScheduling::Current::_narrow (current_obj
					     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
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
TAO_Scheduler::send_request (PortableInterceptor::ClientRequestInfo_ptr request_info
			     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   PortableInterceptor::ForwardRequest))
{
  IOP::ServiceContext* srv_con = new IOP::ServiceContext;
  srv_con->context_id = Client_Interceptor::SchedulingInfo;
  srv_con->context_data.length (sizeof (long));
  ACE_OS::memcpy (srv_con->context_data.get_buffer (),
		  current_->id (ACE_ENV_ARG_PARAMETER)->get_buffer (),
		  sizeof (long));
  ACE_CHECK;
  request_info->add_request_service_context (*srv_con,
					     0);
}

void 
TAO_Scheduler::receive_request (PortableInterceptor::ServerRequestInfo_ptr request_info,
				RTScheduling::Current::IdType_out guid_out,
				CORBA::String_out /*name*/,
				CORBA::Policy_out /*sched_param*/,
				CORBA::Policy_out /*implicit_sched_param*/
				ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   PortableInterceptor::ForwardRequest))
{
  IOP::ServiceContext* serv_cxt = 0;
  
  ACE_TRY
    {
      serv_cxt = request_info->get_request_service_context (Server_Interceptor::SchedulingInfo);
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG,
		  "Invalid Service Context\n"));
    }
  ACE_ENDTRY;

  if (serv_cxt != 0)
    {
      int gu_id;
      ACE_OS::memcpy (&gu_id,
		      serv_cxt->context_data.get_buffer (),
		      serv_cxt->context_data.length ());
      
      ACE_DEBUG ((LM_DEBUG,
		  "The Guid is %d\n",
		  gu_id));

      RTScheduling::Current::IdType* guid;
      ACE_NEW (guid,
	       RTScheduling::Current::IdType);

      guid->length (sizeof (long));
      ACE_OS::memcpy (guid->get_buffer (),
		      serv_cxt->context_data.get_buffer (),
		      sizeof (long));

      guid_out.ptr () = guid;
    }
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
