//$Id$

#ifndef REQUEST_INTERCEPTOR_H
#define REQUEST_INTERCEPTOR_H

class Client_Interceptor:
public TAO_RTScheduler_Export ClientRequestInterceptor
{
  virtual void send_request (PortableInterceptor::ClientRequestInfo_ptr ri
			     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException, 
		     PortableInterceptor::ForwardRequest));
  
  virtual void send_poll (PortableInterceptor::ClientRequestInfo_ptr ri
			  ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));
  
  virtual void receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri
			      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));
  
  virtual void receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri
				  ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException, 
		     PortableInterceptor::ForwardRequest));
  
  virtual void receive_other (PortableInterceptor::ClientRequestInfo_ptr ri
			      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException, 
		     PortableInterceptor::ForwardRequest));
};

class TAO_RTScheduler_Export Server_Interceptor:
public ServerRequestInterceptor
{
  virtual void receive_request_service_contexts (PortableInterceptor::ServerRequestInfo_ptr ri
						 ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException, 
		     PortableInterceptor::ForwardRequest));
  
  virtual void receive_request (PortableInterceptor::ServerRequestInfo_ptr ri
				ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException, 
		     PortableInterceptor::ForwardRequest));
  
  virtual void send_reply (PortableInterceptor::ServerRequestInfo_ptr ri
			   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));
  
  virtual void send_exception (PortableInterceptor::ServerRequestInfo_ptr ri
			       ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
		     PortableInterceptor::ForwardRequest));
  
  virtual void send_other (PortableInterceptor::ServerRequestInfo_ptr ri
			   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
		     PortableInterceptor::ForwardRequest));
};

#endif /*REQUEST_INTERCEPTOR_H*/


