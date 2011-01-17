// -*- C++ -*-
//
// $Id$

#include "ServerORBInitializer.h"
#include "ServerRequest_Interceptor.h"
#include "tao/ORB_Constants.h"
#include "tao/Exception.h"


void
Server_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr)
{

}

void
Server_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info)
{
  this->register_server_request_interceptors (info);
}

void
Server_ORBInitializer::register_server_request_interceptors (
    PortableInterceptor::ORBInitInfo_ptr info)
{
  PortableInterceptor::ServerRequestInterceptor_ptr sri =
    PortableInterceptor::ServerRequestInterceptor::_nil ();

  ACE_NEW_THROW_EX (sri,
                    TAO249_ServerRequest_Interceptor,
                    CORBA::NO_MEMORY ());

  PortableInterceptor::ServerRequestInterceptor_var
    server_interceptor = sri;

  info->add_server_request_interceptor (server_interceptor.in ());
}


