// -*- C++ -*-
//
// $Id$
//

#include "Server_ORBInitializer.h"
#include "server_interceptor.h"

Server_ORBInitializer::Server_ORBInitializer (void)
{
}

void
Server_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr)
{
}

void
Server_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info)
{
  PortableInterceptor::ServerRequestInterceptor_ptr interceptor =
    PortableInterceptor::ServerRequestInterceptor::_nil ();

  // Install the Echo server request interceptor
  ACE_NEW_THROW_EX (interceptor,
                    Echo_Server_Request_Interceptor,
                    CORBA::NO_MEMORY ());

  PortableInterceptor::ServerRequestInterceptor_var
    server_interceptor = interceptor;

  info->add_server_request_interceptor (server_interceptor.in ());
}

