// $Id$

#include "orbsvcs/FaultTolerance/FT_ClientORBInitializer.h"
#include "orbsvcs/FaultTolerance/FT_ClientPolicyFactory.h"
#include "orbsvcs/FaultTolerance/FT_ClientRequest_Interceptor.h"
#include "orbsvcs/FT_CORBA_ORBC.h"
#include "tao/Exception.h"
#include "tao/ORB_Constants.h"

ACE_RCSID (FaultTolerance,
           FT_ClientORBInitializer,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_FT_ClientORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
TAO_FT_ClientORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->register_policy_factories (info
                                   ACE_ENV_ARG_PARAMETER);


  this->register_client_request_interceptors (info
                                              ACE_ENV_ARG_PARAMETER);



}

void
TAO_FT_ClientORBInitializer::register_policy_factories (
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
                    TAO_FT_ClientPolicyFactory,
                    CORBA::NO_MEMORY (
                                      CORBA::SystemException::_tao_minor_code (
                         TAO::VMCID,
                         ENOMEM),
                      CORBA::COMPLETED_NO));

  policy_factory = temp_factory;

  // Bind the same policy factory to all RTCORBA related policy
  // types since a single policy factory is used to create each of
  // the different types of RTCORBA policies.

  CORBA::PolicyType type = FT::REQUEST_DURATION_POLICY;
  info->register_policy_factory (type,
                                 policy_factory.in ()
                                 ACE_ENV_ARG_PARAMETER);

  type = FT::HEARTBEAT_POLICY;
  info->register_policy_factory (type,
                                 policy_factory.in ()
                                 ACE_ENV_ARG_PARAMETER);

  /*
  type = FT::HEARTBEAT_ENABLED_POLICY;
  info->register_policy_factory (type,
                                 policy_factory.in ()
                                 ACE_ENV_ARG_PARAMETER);
  */
}


void
TAO_FT_ClientORBInitializer::register_client_request_interceptors (
    PortableInterceptor::ORBInitInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableInterceptor::ClientRequestInterceptor_ptr cri =
    PortableInterceptor::ClientRequestInterceptor::_nil ();

  ACE_NEW_THROW_EX (cri,
                    TAO::FT_ClientRequest_Interceptor,
                    CORBA::NO_MEMORY ());

  PortableInterceptor::ClientRequestInterceptor_var
    client_interceptor = cri;

  info->add_client_request_interceptor (client_interceptor.in ()
                                        ACE_ENV_ARG_PARAMETER);
}

TAO_END_VERSIONED_NAMESPACE_DECL
