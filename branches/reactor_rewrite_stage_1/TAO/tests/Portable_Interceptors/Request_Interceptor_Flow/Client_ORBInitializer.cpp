// -*- C++ -*-
//
// $Id$

#include "Client_ORBInitializer.h"

ACE_RCSID (Request_Interceptor_Flow,
           Client_ORBInitializer,
           "$Id$")

#include "tao/Exception.h"

#include "Client_Request_Interceptor.h"

void
Client_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Client_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  // Create and register the test request interceptors.
  PortableInterceptor::ClientRequestInterceptor_ptr ci =
    PortableInterceptor::ClientRequestInterceptor::_nil ();
  ACE_NEW_THROW_EX (ci,
                    Client_Request_Interceptor ("CLIENT A"),
                    CORBA::NO_MEMORY (
                      CORBA_SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  PortableInterceptor::ClientRequestInterceptor_var ci_interceptor =
    ci;

  info->add_client_request_interceptor (ci_interceptor.in ()
                                        TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_NEW_THROW_EX (ci,
                    Client_Request_Interceptor ("CLIENT B"),
                    CORBA::NO_MEMORY (
                      CORBA_SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  ci_interceptor = ci;

  info->add_client_request_interceptor (ci_interceptor.in ()
                                        TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_NEW_THROW_EX (ci,
                    Client_Request_Interceptor ("CLIENT C"),
                    CORBA::NO_MEMORY (
                      CORBA_SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  ci_interceptor = ci;

  info->add_client_request_interceptor (ci_interceptor.in ()
                                        TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}
