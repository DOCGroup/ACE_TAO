//$Id$

#include "Request_Interceptor.h"

void 
Client_Interceptor::send_request (PortableInterceptor::ClientRequestInfo_ptr ri
				  ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException, 
		   PortableInterceptor::ForwardRequest))
{
  // Temporary current.
  TAO_RTScheduler_Current_i *new_current = 0;
  
  // If this is a one way request
  if (ri.oneway) {

    // Generate GUID.
    IdType guid = ACE::new_guid ();

    // Create new DT.
    DistributableThread *dt =
      new DistributableThread ();

    // Add new DT to map.
    int result =
      orb->dt_map_->add (guid, dt);

    // Create new temporary current. Note that
    // the new <sched_param> is the current
    // <implicit_sched_param> and there is no
    // segment name.
    new_current
      = new Current_i (guid,
                       0,
                       implicit_sched_param_,
                       0,
                       this,
                       dt);

    // Install new current in the ORB.
    orb->rt_current_ =
      new_current;
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
