// -*- C++ -*-
//
// $Id$
//

#include "Server_ORBInitializer.h"
#include "interceptors.h"

ACE_RCSID (Service_Context_Manipulation, Server_ORBInitializer, "$Id$")

Server_ORBInitializer::Server_ORBInitializer (void)
{
}

void
Server_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Server_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableInterceptor::ServerRequestInterceptor_ptr interceptor =
    PortableInterceptor::ServerRequestInterceptor::_nil ();

  // Install the Echo server request interceptor
  ACE_NEW_THROW_EX (interceptor,
                    Echo_Server_Request_Interceptor,
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  PortableInterceptor::ServerRequestInterceptor_var
    server_interceptor = interceptor;

  info->add_server_request_interceptor (server_interceptor.in ()
                                        TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

