// -*- C++ -*-
//
// $Id$

#include "FT_ServerORBInitializer.h"
#include "FT_ServerPolicyFactory.h"
#include "FT_ServerRequest_Interceptor.h"
#include "orbsvcs/FT_CORBA_ORBC.h"
#include "tao/Exception.h"


ACE_RCSID (FaultTolerance,
           FT_ServerORBInitializer,
           "$Id$")
void
TAO_FT_ServerORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

}

void
TAO_FT_ServerORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->register_policy_factories (info
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->register_server_request_interceptors (info
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

}

void
TAO_FT_ServerORBInitializer::register_policy_factories (
  PortableInterceptor::ORBInitInfo_ptr info
  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Register the FTCORBA policy factories.

  PortableInterceptor::PolicyFactory_ptr temp_factory =
    PortableInterceptor::PolicyFactory::_nil ();
  PortableInterceptor::PolicyFactory_var policy_factory;

  // This policy factory is used for all FTCORBA related policies.

  ACE_NEW_THROW_EX (temp_factory,
                    TAO_FT_ServerPolicyFactory,
                    CORBA::NO_MEMORY (
                                      CORBA::SystemException::_tao_minor_code (
                         TAO_DEFAULT_MINOR_CODE,
                         ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  policy_factory = temp_factory;

  // Bind the same policy factory to all FTCORBA related policy
  // types since a single policy factory is used to create each of
  // the different types of FTCORBA policies.


  CORBA::PolicyType type = FT::HEARTBEAT_ENABLED_POLICY;
  info->register_policy_factory (type,
                                 policy_factory.in ()
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  // Transfer ownership of the policy factory to the registry.
  (void) policy_factory._retn ();
}


void
TAO_FT_ServerORBInitializer::register_server_request_interceptors (
    PortableInterceptor::ORBInitInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableInterceptor::ServerRequestInterceptor_ptr sri =
    PortableInterceptor::ServerRequestInterceptor::_nil ();

  ACE_NEW_THROW_EX (sri,
                    TAO::FT_ServerRequest_Interceptor,
                    CORBA::NO_MEMORY ());

  PortableInterceptor::ServerRequestInterceptor_var
    server_interceptor = sri;

  info->add_server_request_interceptor (server_interceptor.in ()
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}


