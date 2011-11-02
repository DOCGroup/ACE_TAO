// $Id$

#include "Client_ORBInitializer.h"
#include "tao/Exception.h"
#include "tao/ORB_Constants.h"
#include "Client_Request_Interceptor.h"

void
Client_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr)
{
}

void
Client_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info)
{

  // Create and register the test request interceptors.
  PortableInterceptor::ClientRequestInterceptor_ptr ci =
    PortableInterceptor::ClientRequestInterceptor::_nil ();
  ACE_NEW_THROW_EX (ci,
                    Client_Request_Interceptor ("CLIENT A"),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  PortableInterceptor::ClientRequestInterceptor_var ci_interceptor =
    ci;

  info->add_client_request_interceptor (ci_interceptor.in ());

  ACE_NEW_THROW_EX (ci,
                    Client_Request_Interceptor ("CLIENT B"),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  ci_interceptor = ci;

  info->add_client_request_interceptor (ci_interceptor.in ());

  ACE_NEW_THROW_EX (ci,
                    Client_Request_Interceptor ("CLIENT C"),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  ci_interceptor = ci;

  info->add_client_request_interceptor (ci_interceptor.in ());
}
