//$Id$

#include "Request_Interceptor.h"
#include "Current.h"
#include "Distributable_Thread.h"

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
  TAO_RTScheduler_Current_i *prev_current = 0;
  
  TAO_RTScheduler_Current_var current = TAO_RTScheduler_Current::_narrow (this->current_
									  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  prev_current = current->implementation ();
  
  if (!CORBA::is_nil (current.in ()))
    {
      
      
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
	  RTScheduling::DistributableThread_var dt = TAO_DistributableThread_Factory::create_DT ();
      
 

	  // Add new DT to map.
	  int result = prev_current->dt_hash ()->bind (guid, dt);
      
	  // Create new temporary current. Note that
	  // the new <sched_param> is the current
	  // <implicit_sched_param> and there is no
	  // segment name.

	  ACE_NEW (new_current,
		   TAO_RTScheduler_Current_i (prev_current->orb (),
					      prev_current->dt_hash (),
					      guid,
					      0,
					      prev_current->implicit_scheduling_parameter (),
					      0,
					      dt.in (),
					      prev_current));
      
      
	  // Install new current in the ORB.
	  current->implementation (new_current);
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
  ACE_DEBUG ((LM_DEBUG,
	      "No Scheduling Segment Context\n"));
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

char* 
Client_Interceptor::name (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

void 
Client_Interceptor::destroy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
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

char* 
Server_Interceptor::name (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

void 
Server_Interceptor::destroy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}
