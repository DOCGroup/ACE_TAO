// -*- C++ -*-
//
// $Id$

#include "RT_ORBInitializer.h"

ACE_RCSID (TAO, RT_ORBInitializer, "$Id$")

#include "RTCORBA.h"
#include "RT_Policy_i.h"
#include "RT_Protocols_Hooks.h"
#include "Priority_Mapping_Manager.h"
#include "tao/Exception.h"
#include "tao/ORB_Core.h"
#include "RT_ORB_Loader.h"
#include "RT_Stub_Factory.h"
#include "RT_Endpoint_Selector_Factory.h"

#include "ace/Service_Repository.h"
#include "ace/Svc_Conf.h"

static const char *rt_poa_factory_name = "TAO_RT_POA";
static const char *rt_poa_factory_directive = "dynamic TAO_RT_POA Service_Object * TAO_RTPortableServer:_make_TAO_RT_Object_Adapter_Factory()";

TAO_RT_ORBInitializer::TAO_RT_ORBInitializer (TAO_Priority_Mapping *priority_mapping)
  : priority_mapping_ (priority_mapping)
{
}

void
TAO_RT_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr info
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_ENV_ARG_DEFN;

  // Register all of the RT related services.
  ACE_Service_Config::process_directive (ace_svc_desc_TAO_RT_Protocols_Hooks);
  ACE_Service_Config::process_directive (ace_svc_desc_TAO_RT_Stub_Factory);
  ACE_Service_Config::process_directive (ace_svc_desc_RT_Endpoint_Selector_Factory);

  // If the application resolves the root POA, make sure we load the RT POA.
  TAO_ORB_Core::set_poa_factory (rt_poa_factory_name,
                                 rt_poa_factory_directive);

//  @@ RTCORBA Subsetting: service gets automatically loaded now by using a static initializer.
//  ACE_Service_Config::process_directive (ace_svc_desc_TAO_RT_ORB_Loader);

  // Set the name of the Protocol_Hooks to be the RT_Protocols_Hooks.
  TAO_ORB_Core::set_protocols_hooks ("RT_Protocols_Hooks");

  // Set the name of the stub factory to be the RT_Stub_Factory.
  TAO_ORB_Core::set_stub_factory ("RT_Stub_Factory");

  // Set the name of the stub factory to be the RT_Stub_Factory.
  TAO_ORB_Core::set_endpoint_selector_factory ("RT_Endpoint_Selector_Factory");

  // Set the Priority_Mapping_Manager
  TAO_Priority_Mapping_Manager *manager = 0;

  ACE_NEW_THROW_EX (manager,
                    TAO_Priority_Mapping_Manager (this->priority_mapping_),
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

