// -*- C++ -*-
//
// $Id$

#include "ServerORBInitializer2.h"
#include "ServerRequest_Interceptor2.h"
#include "tao/ORB_Constants.h"
#include "tao/Exception.h"


Server_ORBInitializer2::Server_ORBInitializer2 (void)
  : sri_ (0)
{
}

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
  TAO249_ServerRequest_Interceptor2 *sri = 0;
  ACE_NEW_THROW_EX (sri,
                    TAO249_ServerRequest_Interceptor2,
                    CORBA::NO_MEMORY ());

  PortableInterceptor::ServerRequestInterceptor_var
    server_interceptor = sri;
  this->sri_ = sri;

  info->add_server_request_interceptor (server_interceptor.in ());
}

void
Server_ORBInitializer2::set_server_iogr (CORBA::Object_ptr obj)
{
  if (this->sri_)
    this->sri_->server_iogr_ = CORBA::Object::_duplicate (obj);
}
