// $Id$

#include "Crash_Injector_Interceptor.h"
#include "Crash_Injector.h"

Crash_Injector_Interceptor::Crash_Injector_Interceptor ()
{
}

Crash_Injector_Interceptor::~Crash_Injector_Interceptor ()
{
}


char *
Crash_Injector_Interceptor::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("Crash_Injector_Interceptor");
}

void
Crash_Injector_Interceptor::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Crash_Injector_Interceptor::receive_request_service_contexts (
  PortableInterceptor::ServerRequestInfo_ptr ri
  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
  PortableInterceptor::ForwardRequest))
{


}




void
Crash_Injector_Interceptor::receive_request (PortableInterceptor::ServerRequestInfo_ptr ri
                                            ACE_ENV_ARG_DECL)
                                            ACE_THROW_SPEC ((CORBA::SystemException,
                                            PortableInterceptor::ForwardRequest))
{
  CORBA::String_var operation = ri->operation (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  Crash_Injector::instance()->check_on_receive_request(operation.in());
}

void
Crash_Injector_Interceptor::send_reply (PortableInterceptor::ServerRequestInfo_ptr ri
                                       ACE_ENV_ARG_DECL)
                                       ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::String_var operation = ri->operation (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  Crash_Injector::instance()->check_on_sending_reply(operation.in());
}

void
Crash_Injector_Interceptor::send_exception (
  PortableInterceptor::ServerRequestInfo_ptr ri
  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
  PortableInterceptor::ForwardRequest))
{
}

void
Crash_Injector_Interceptor::send_other (PortableInterceptor::ServerRequestInfo_ptr
                                       ACE_ENV_ARG_DECL_NOT_USED)
                                       ACE_THROW_SPEC ((CORBA::SystemException,
                                       PortableInterceptor::ForwardRequest))
{
}
