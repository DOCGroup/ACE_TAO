// -*- C++ -*-
//
// $Id$

#include "ServerORBInitializer2.h"
#include "ServerRequest_Interceptor2.h"
#include "tao/ORB_Constants.h"
#include "tao/Exception.h"


void
Server_ORBInitializer2::pre_init (
    PortableInterceptor::ORBInitInfo_ptr)
{

}

void
Server_ORBInitializer2::post_init (
    PortableInterceptor::ORBInitInfo_ptr info)
{
  this->register_server_request_interceptors (info);
}

void
Server_ORBInitializer2::register_server_request_interceptors (
    PortableInterceptor::ORBInitInfo_ptr info)
{
  PortableInterceptor::ServerRequestInterceptor_ptr sri =
    PortableInterceptor::ServerRequestInterceptor::_nil ();

  ACE_NEW_THROW_EX (sri,
                    ServerRequest_Interceptor2,
                    CORBA::NO_MEMORY ());

  PortableInterceptor::ServerRequestInterceptor_var
    server_interceptor = sri;

  info->add_server_request_interceptor (server_interceptor.in ());
}


