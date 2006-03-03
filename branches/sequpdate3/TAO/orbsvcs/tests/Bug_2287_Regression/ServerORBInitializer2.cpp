// -*- C++ -*-
//
// $Id$

#include "ServerORBInitializer2.h"
#include "ServerRequest_Interceptor2.h"
#include "tao/ORB_Constants.h"
#include "tao/Exception.h"


void
Server_ORBInitializer2::pre_init (
    PortableInterceptor::ORBInitInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

}

void
Server_ORBInitializer2::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->register_server_request_interceptors (info
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
Server_ORBInitializer2::register_server_request_interceptors (
    PortableInterceptor::ORBInitInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableInterceptor::ServerRequestInterceptor_ptr sri =
    PortableInterceptor::ServerRequestInterceptor::_nil ();

  ACE_NEW_THROW_EX (sri,
                    TAO249_ServerRequest_Interceptor2,
                    CORBA::NO_MEMORY ());

  PortableInterceptor::ServerRequestInterceptor_var
    server_interceptor = sri;

  info->add_server_request_interceptor (server_interceptor.in ()
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}


