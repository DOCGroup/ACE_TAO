// -*- C++ -*-
//
// $Id$

#include "tao/RT_ORBInitializer.h"

ACE_RCSID (tao, RT_ORBInitializer, "$Id$")

#if TAO_HAS_RT_CORBA == 1

#include "tao/RT_PolicyFactory.h"
#include "tao/RTCORBAC.h"
#include "tao/Exception.h"

void
TAO_RT_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
TAO_RT_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->register_policy_factories (info
                                   TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_RT_ORBInitializer::register_policy_factories (
  PortableInterceptor::ORBInitInfo_ptr info
  TAO_ENV_ARG_DECL)
{
  /// Register the RTCORBA policy factories.

  PortableInterceptor::PolicyFactory_ptr temp_factory =
    PortableInterceptor::PolicyFactory::_nil ();
  PortableInterceptor::PolicyFactory_var policy_factory;

  TAO_ENV_ARG_DEFN;

  /// This policy factory is used for all RTCORBA related policies.
  ACE_NEW_THROW_EX (temp_factory,
                    TAO_RT_PolicyFactory,
                    CORBA::NO_MEMORY (
					CORBA::SystemException::_tao_minor_code (
                         TAO_DEFAULT_MINOR_CODE,
                         ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  policy_factory = temp_factory;

  /// Bind the same policy factory to all RTCORBA related policy
  /// types since a single policy factory is used to create each of
  /// the different types of RTCORBA policies.

  CORBA::PolicyType type = RTCORBA::PRIORITY_MODEL_POLICY_TYPE;
  info->register_policy_factory (type,
                                 policy_factory.in ()
                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  type = RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE;
  info->register_policy_factory (type,
                                 policy_factory.in ()
                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  type = RTCORBA::CLIENT_PROTOCOL_POLICY_TYPE;
  info->register_policy_factory (type,
                                 policy_factory.in ()
                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  /// Transfer ownership of the policy factory to the registry.
  (void) policy_factory._retn ();

}

#endif  /* TAO_HAS_RT_CORBA == 1 */
