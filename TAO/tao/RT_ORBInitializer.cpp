// -*- C++ -*-
//
// $Id$

#include "tao/RT_ORBInitializer.h"

ACE_RCSID (TAO,
           RT_ORBInitializer,
           "$Id$")

#if TAO_HAS_RT_CORBA == 1


#include "tao/RTCORBAC.h"
#include "tao/RT_Policy_i.h"
#include "tao/RT_Protocols_Hooks.h"
#include "tao/Priority_Mapping_Manager.h"
#include "tao/Exception.h"
#include "tao/ORB_Core.h"

#include "ace/Service_Repository.h"
#include "ace/Svc_Conf.h"

void
TAO_RT_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr info
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_ENV_ARG_DEFN;

  // Just a convenient way to say the same thing.
  const ACE_Static_Svc_Descriptor &desc =
    ace_svc_desc_TAO_RT_Protocols_Hooks;

  ACE_Service_Object_Exterminator gobbler;
  void *sym = (*desc.alloc_)(&gobbler);

  ACE_Service_Type_Impl *service_type_impl =
    ace_create_service_type (desc.name_,
                             desc.type_,
                             sym,
                             desc.flags_,
                             gobbler);

  // @@ Raise exception
  ACE_Service_Type *service_type;
  ACE_NEW (service_type,
           ACE_Service_Type (desc.name_,
                             service_type_impl,
                             0,
                             desc.active_));
  // @@ Error checking
  (void) ACE_Service_Repository::instance ()->insert (service_type);

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

  info->register_initial_reference ("PriorityMappingManager",
                                    manager,
                                    ACE_TRY_ENV);
  ACE_CHECK;

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
  TAO_ENV_ARG_DEFN;

  this->register_policy_factories (info,
                                   ACE_TRY_ENV);
  ACE_CHECK;
}

void
TAO_RT_ORBInitializer::register_policy_factories (
  PortableInterceptor::ORBInitInfo_ptr info,
  CORBA::Environment &ACE_TRY_ENV)
{
  // Register the RTCORBA policy factories.

  // The RTCORBA policy factory is stateless and reentrant, so share a
  // single instance between all ORBs.
  PortableInterceptor::PolicyFactory_ptr policy_factory =
    &(this->policy_factory_);

  // Bind the same policy factory to all RTCORBA related policy
  // types since a single policy factory is used to create each of
  // the different types of RTCORBA policies.

  CORBA::PolicyType type = RTCORBA::PRIORITY_MODEL_POLICY_TYPE;
  info->register_policy_factory (type,
                                 policy_factory,
                                 ACE_TRY_ENV);
  ACE_CHECK;

  type = RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE;
  info->register_policy_factory (type,
                                 policy_factory,
                                 ACE_TRY_ENV);
  ACE_CHECK;

  type = RTCORBA::CLIENT_PROTOCOL_POLICY_TYPE;
  info->register_policy_factory (type,
                                 policy_factory,
                                 ACE_TRY_ENV);
  ACE_CHECK;
}

#endif  /* TAO_HAS_RT_CORBA == 1 */
