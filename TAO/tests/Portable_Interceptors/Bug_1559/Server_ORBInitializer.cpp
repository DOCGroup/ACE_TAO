// -*- C++ -*-
#include "Server_ORBInitializer.h"

Server_ORBInitializer::Server_ORBInitializer ()
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
  // Install the Echo server request interceptor
  ACE_NEW_THROW_EX (this->server_interceptor_,
                    Echo_Server_Request_Interceptor,
                    CORBA::NO_MEMORY ());

  PortableInterceptor::ServerRequestInterceptor_var interceptor =
    this->server_interceptor_;

  info->add_server_request_interceptor (interceptor.in ());
}

Echo_Server_Request_Interceptor *
Server_ORBInitializer::server_interceptor ()
{
  return this->server_interceptor_;
}

