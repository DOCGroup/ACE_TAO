#include "Server_ORBInitializer.h"

ACE_RCSID (Request_Interceptor_Flow,
           Server_ORBInitializer,
           "$Id$")

#include "tao/Exception.h"

#include "Server_Request_Interceptor.h"

void
Server_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Server_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  // Create and register the test request interceptors.
  PortableInterceptor::ServerRequestInterceptor_ptr si =
    PortableInterceptor::ServerRequestInterceptor::_nil ();
  ACE_NEW_THROW_EX (si,
                    Server_Request_Interceptor ("SERVER A"),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  PortableInterceptor::ServerRequestInterceptor_var si_interceptor =
    si;

  info->add_server_request_interceptor (si_interceptor.in ()
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_NEW_THROW_EX (si,
                    Server_Request_Interceptor ("SERVER B"),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  si_interceptor = si;

  info->add_server_request_interceptor (si_interceptor.in ()
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_NEW_THROW_EX (si,
                    Server_Request_Interceptor ("SERVER C"),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  si_interceptor = si;

  info->add_server_request_interceptor (si_interceptor.in ()
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}
