// $Id$

#include "Crash_Injector_Initializer.h"
#include "Crash_Injector_Interceptor.h"

Crash_Injector_Initializer::Crash_Injector_Initializer ()
{
}

void
Crash_Injector_Initializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Crash_Injector_Initializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  /*
  PortableInterceptor::ClientRequestInterceptor_var client_interceptor;
  PortableInterceptor::ClientRequestInterceptor_ptr ctmp;

  ACE_NEW_THROW_EX(ctmp,
                TAO_Set_Update_Interceptor,
                CORBA::NO_MEMORY());

  client_interceptor = ctmp;

  info->add_client_request_interceptor (client_interceptor.in()
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  */

  PortableInterceptor::ServerRequestInterceptor_var  server_interceptor;
  PortableInterceptor::ServerRequestInterceptor_ptr  stmp;

  ACE_NEW_THROW_EX(stmp,
                Crash_Injector_Interceptor,
                CORBA::NO_MEMORY());
  server_interceptor = stmp;

  info->add_server_request_interceptor (server_interceptor.in()
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}
