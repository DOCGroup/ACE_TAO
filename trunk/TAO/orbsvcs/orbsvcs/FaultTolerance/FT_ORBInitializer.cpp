// -*- C++ -*-
//
// $Id$

#include "FT_ORBInitializer.h"
#include "FT_PolicyFactory.h"
#include "orbsvcs/FT_CORBAC.h"
#include "tao/Exception.h"


ACE_RCSID (FaultTolerance, FT_ORBInitializer, "$Id$")
void
TAO_FT_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

}

void
TAO_FT_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->register_policy_factories (info
                                   TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_FT_ORBInitializer::register_policy_factories (
  PortableInterceptor::ORBInitInfo_ptr info
  TAO_ENV_ARG_DECL)
{
  // Register the FTCORBA policy factories.

  PortableInterceptor::PolicyFactory_ptr temp_factory =
    PortableInterceptor::PolicyFactory::_nil ();
  PortableInterceptor::PolicyFactory_var policy_factory;

  // This policy factory is used for all FTCORBA related policies.

  ACE_NEW_THROW_EX (temp_factory,
                    TAO_FT_PolicyFactory,
                    CORBA::NO_MEMORY (
                                      CORBA::SystemException::_tao_minor_code (
                         TAO_DEFAULT_MINOR_CODE,
                         ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  policy_factory = temp_factory;

  // Bind the same policy factory to all RTCORBA related policy
  // types since a single policy factory is used to create each of
  // the different types of RTCORBA policies.

  CORBA::PolicyType type = FT::REQUEST_DURATION_POLICY;
  info->register_policy_factory (type,
                                 policy_factory.in ()
                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  type = FT::HEARTBEAT_POLICY;
  info->register_policy_factory (type,
                                 policy_factory.in ()
                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  type = FT::HEARTBEAT_ENABLED_POLICY;
  info->register_policy_factory (type,
                                 policy_factory.in ()
                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Transfer ownership of the policy factory to the registry.
  (void) policy_factory._retn ();
}
