// $Id$

#include "Server_ORBInitializer.h"
#include "tao/Exception.h"
#include "tao/ORB_Constants.h"
#include "Server_Request_Interceptor.h"

void
Server_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr)
{
}

void
Server_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info)
{

  // Create and register the test request interceptors.
  PortableInterceptor::ServerRequestInterceptor_ptr si =
    PortableInterceptor::ServerRequestInterceptor::_nil ();
  ACE_NEW_THROW_EX (si,
                    Server_Request_Interceptor ("SERVER A"),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  PortableInterceptor::ServerRequestInterceptor_var si_interceptor =
    si;

  info->add_server_request_interceptor (si_interceptor.in ());

  ACE_NEW_THROW_EX (si,
                    Server_Request_Interceptor ("SERVER B"),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  si_interceptor = si;

  info->add_server_request_interceptor (si_interceptor.in ());

  ACE_NEW_THROW_EX (si,
                    Server_Request_Interceptor ("SERVER C"),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  si_interceptor = si;

  info->add_server_request_interceptor (si_interceptor.in ());
}
