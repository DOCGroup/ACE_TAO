//$Id$

#include "Request_Interceptor.h"
#include "Current.h"

Client_Interceptor::Client_Interceptor (RTScheduling::Current_ptr current)
  : current_ (RTScheduling::Current::_duplicate (current))
{
} 

void 
Client_Interceptor::send_request (PortableInterceptor::ClientRequestInfo_ptr ri
				  ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException, 
		   PortableInterceptor::ForwardRequest))
{
  ACE_DEBUG ((LM_DEBUG,
	      "Client_Interceptor::send_request\n"));

  // Temporary current.
  TAO_RTScheduler_Current_i *new_current = 0;
  
  // If this is a one way request
  if (!ri->response_expected ()) 
    {
      
      // Generate GUID.
      RTScheduling::Current::IdType guid;
      guid.length (sizeof(long));
      
      long temp = ++guid_counter;
      ACE_OS::memcpy (guid.get_buffer (),
		      &temp,
		    sizeof(long));
      
      int id;
      ACE_OS::memcpy (&id,
		      guid.get_buffer (),
		      guid.length ());
      
      ACE_DEBUG ((LM_DEBUG,
		  "The Guid is %d %d\n",
		  id,
		  guid_counter));
      
      // Create new DT.
      RTScheduling::DistributableThread_var dt = = TAO_DistributableThread_Factory::create_DT ();
      
      TAO_RTScheduler_Current_i* prev_current = this->current_->implementation ();

      // Add new DT to map.
      int result = prev_current->dt_hash ()->add (guid, dt);
      
      // Create new temporary current. Note that
      // the new <sched_param> is the current
      // <implicit_sched_param> and there is no
      // segment name.
      TAO_RTScheduler_Current_i* new_current;
      ACE_NEW_RETURN (new_current,
		      Current_i (guid,
				 0,
				 prev_current->implicit_sched_param (),
				 0,
				 prev_current,
				 dt));
		      
      
      // Install new current in the ORB.
      this->current_->implementation (new_current);
    }
  
  // Scheduler populates the service context with
  // scheduling parameters.
  prev_current->scheduler ()->send_request (ri);
  
  // If this is a one way request
  if (!ri->response_expected ()) 
    {
      // Cleanup temporary DT.
      new_current->cleanup_DT ();

      new_current->cleanup_current ();
    }
}

void 
Client_Interceptor::send_poll (PortableInterceptor::ClientRequestInfo_ptr ri
			       ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void 
Client_Interceptor::receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri
				   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}
  
void 
Client_Interceptor::receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri
				       ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException, 
		   PortableInterceptor::ForwardRequest))
{
}
  
void 
Client_Interceptor::receive_other (PortableInterceptor::ClientRequestInfo_ptr ri
				   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException, 
		   PortableInterceptor::ForwardRequest))
{
}

void 
Server_Interceptor::receive_request_service_contexts (PortableInterceptor::ServerRequestInfo_ptr ri
						      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException, 
		   PortableInterceptor::ForwardRequest))
{
}
  
void 
Server_Interceptor::receive_request (PortableInterceptor::ServerRequestInfo_ptr ri
				     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException, 
		   PortableInterceptor::ForwardRequest))
{
}

void 
Server_Interceptor::send_reply (PortableInterceptor::ServerRequestInfo_ptr ri
				ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}
  
void 
Server_Interceptor::send_exception (PortableInterceptor::ServerRequestInfo_ptr ri
				    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   PortableInterceptor::ForwardRequest))
{
}
  
void 
Server_Interceptor::send_other (PortableInterceptor::ServerRequestInfo_ptr ri
				ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   PortableInterceptor::ForwardRequest))
{
}
