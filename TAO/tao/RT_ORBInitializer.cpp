// -*- C++ -*-
//
// $Id$

#include "tao/RT_ORBInitializer.h"

ACE_RCSID (tao, RT_ORBInitializer, "$Id$")

#if TAO_HAS_RT_CORBA == 1

#include "tao/RT_PolicyFactory.h"
#include "tao/RTCORBAC.h"
#include "tao/RT_Policy_i.h"
#include "tao/RT_Protocols_Hooks.h"
#include "tao/Priority_Mapping_Manager.h"
#include "tao/Exception.h"
#include "tao/ORB_Core.h"

void
TAO_RT_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_ENV_ARG_DEFN;

  // Sets the name of the Protocol_Hooks to be the RT_Protocols_Hooks.
  TAO_ORB_Core::set_protocols_hooks ("RT_Protocols_Hooks");

  // Set the Priority_Mapping_Manager
  TAO_Priority_Mapping_Manager *manager = 0;

  ACE_NEW_THROW_EX (manager,
                    TAO_Priority_Mapping_Manager,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  TAO_Priority_Mapping_Manager_var safe_manager = manager;

  TAO_ORB_Core::priority_mapping_manager (manager);

  // Sets the client_protocol policy.
  TAO_RT_Protocols_Hooks::set_client_protocols_hook
    (TAO_ClientProtocolPolicy::hook);

  // Sets the server_protocol policy.
  TAO_RT_Protocols_Hooks::set_server_protocols_hook
    (TAO_ServerProtocolPolicy::hook);
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
}

#endif  /* TAO_HAS_RT_CORBA == 1 */
