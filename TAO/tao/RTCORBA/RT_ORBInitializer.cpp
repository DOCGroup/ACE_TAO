// $Id$

#include "tao/RTCORBA/RT_ORBInitializer.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#define TAO_RTCORBA_SAFE_INCLUDE
#include "tao/RTCORBA/RTCORBAC.h"
#undef TAO_RTCORBA_SAFE_INCLUDE

#include "tao/RTCORBA/RT_Policy_i.h"
#include "tao/RTCORBA/RT_PolicyFactory.h"
#include "tao/RTCORBA/RT_Protocols_Hooks.h"
#include "tao/RTCORBA/Priority_Mapping_Manager.h"
#include "tao/RTCORBA/Network_Priority_Mapping_Manager.h"
#include "tao/RTCORBA/RT_ORB_Loader.h"
#include "tao/RTCORBA/RT_Stub_Factory.h"
#include "tao/RTCORBA/RT_Endpoint_Selector_Factory.h"
#include "tao/RTCORBA/Continuous_Priority_Mapping.h"
#include "tao/RTCORBA/Linear_Priority_Mapping.h"
#include "tao/RTCORBA/Direct_Priority_Mapping.h"
#include "tao/RTCORBA/Linear_Network_Priority_Mapping.h"
#include "tao/RTCORBA/RT_ORB.h"
#include "tao/RTCORBA/RT_Current.h"
#include "tao/RTCORBA/RT_Thread_Lane_Resources_Manager.h"
#include "tao/RTCORBA/RT_Service_Context_Handler.h"

#include "tao/Exception.h"
#include "tao/ORB_Core.h"
#include "tao/PI/ORBInitInfo.h"
#include "tao/debug.h"

#include "ace/Service_Repository.h"
#include "ace/Svc_Conf.h"
#include "ace/Sched_Params.h"

static const char rt_poa_factory_name[] = "TAO_RT_Object_Adapter_Factory";
static const ACE_TCHAR rt_poa_factory_directive[] =
  ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE(
    "TAO_RT_Object_Adapter_Factory",
    "TAO_RTPortableServer",
    TAO_VERSION,
    "_make_TAO_RT_Object_Adapter_Factory",
    "");

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_RT_ORBInitializer::TAO_RT_ORBInitializer (int priority_mapping_type,
                                              int network_priority_mapping_type,
                                              int ace_sched_policy,
                                              long sched_policy,
                                              long scope_policy,
                                              TAO_RT_ORBInitializer::TAO_RTCORBA_DT_LifeSpan lifespan,
                                              ACE_Time_Value const &dynamic_thread_time)
  : priority_mapping_type_ (priority_mapping_type),
    network_priority_mapping_type_ (network_priority_mapping_type),
    ace_sched_policy_ (ace_sched_policy),
    sched_policy_ (sched_policy),
    scope_policy_ (scope_policy),
    lifespan_ (lifespan),
    dynamic_thread_time_ (dynamic_thread_time)
{
}

void
TAO_RT_ORBInitializer::pre_init (PortableInterceptor::ORBInitInfo_ptr info)
{
  //
  // Register all of the RT related services.
  //

  // Narrow to a TAO_ORBInitInfo object to get access to the
  // orb_core() TAO extension.
  TAO_ORBInitInfo_var tao_info = TAO_ORBInitInfo::_narrow (info);

  if (CORBA::is_nil (tao_info.in ()))
    {
      if (TAO_debug_level > 0)
        TAOLIB_ERROR ((LM_ERROR,
                    "(%P|%t) TAO_RT_ORBInitializer::pre_init:\n"
                    "(%P|%t)    Unable to narrow "
                    "\"PortableInterceptor::ORBInitInfo_ptr\" to\n"
                    "(%P|%t)   \"TAO_ORBInitInfo *.\"\n"));

      throw ::CORBA::INTERNAL ();
    }

  // Bind the service context handler for RTCORBA
  TAO_RT_Service_Context_Handler* h = 0;
  ACE_NEW (h,
           TAO_RT_Service_Context_Handler());
  tao_info->orb_core ()->service_context_registry ().bind (IOP::RTCorbaPriority, h);

  // Set the name of the Protocol_Hooks to be RT_Protocols_Hooks.
  tao_info->orb_core ()->orb_params ()->protocols_hooks_name ("RT_Protocols_Hooks");
  ACE_Service_Config::process_directive (ace_svc_desc_TAO_RT_Protocols_Hooks);

  // Set the name of the stub factory to be RT_Stub_Factory.
  tao_info->orb_core ()->orb_params ()->stub_factory_name ("RT_Stub_Factory");
  ACE_Service_Config::process_directive (ace_svc_desc_TAO_RT_Stub_Factory);

  // Set the name of the endpoint selector factory to be RT_Endpoint_Selector_Factory.
  tao_info->orb_core ()->orb_params ()->endpoint_selector_factory_name ("RT_Endpoint_Selector_Factory");
  ACE_Service_Config::process_directive (ace_svc_desc_RT_Endpoint_Selector_Factory);

  // Set the name of the thread lane resources manager to be RT_Thread_Lane_Resources_Manager.
  tao_info->orb_core ()->orb_params ()->thread_lane_resources_manager_factory_name ("RT_Thread_Lane_Resources_Manager_Factory");
  ACE_Service_Config::process_directive (ace_svc_desc_TAO_RT_Thread_Lane_Resources_Manager_Factory);

  // If the application resolves the root POA, make sure we load the RT POA.
  tao_info->orb_core ()->orb_params ()->poa_factory_name (rt_poa_factory_name);
  tao_info->orb_core ()->orb_params ()->poa_factory_directive (rt_poa_factory_directive);

  // Create the initial priority mapping instance.
  TAO_Priority_Mapping *pm = 0;
  switch (this->priority_mapping_type_)
    {
    case TAO_PRIORITY_MAPPING_CONTINUOUS:
      ACE_NEW (pm,
               TAO_Continuous_Priority_Mapping (this->ace_sched_policy_));
      break;
    case TAO_PRIORITY_MAPPING_LINEAR:
      ACE_NEW (pm,
               TAO_Linear_Priority_Mapping (this->ace_sched_policy_));
      break;
    default:
    case TAO_PRIORITY_MAPPING_DIRECT:
      ACE_NEW (pm,
               TAO_Direct_Priority_Mapping (this->ace_sched_policy_));
      break;
    }

  // Set the Priority_Mapping_Manager
  TAO_Priority_Mapping_Manager *manager = 0;

  ACE_NEW_THROW_EX (manager,
                    TAO_Priority_Mapping_Manager (pm),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  TAO_Priority_Mapping_Manager_var safe_manager = manager;

  info->register_initial_reference ("PriorityMappingManager", manager);

  // Create the initial priority mapping instance.
  TAO_Network_Priority_Mapping *npm = 0;
  switch (this->network_priority_mapping_type_)
    {
    default:
    case TAO_NETWORK_PRIORITY_MAPPING_LINEAR:
      ACE_NEW (npm,
               TAO_Linear_Network_Priority_Mapping (this->ace_sched_policy_));
      break;
    }

  // Set the Priority_Mapping_Manager
  TAO_Network_Priority_Mapping_Manager * network_manager = 0;

  // @@ There is a potential memory leak here.  If the new() below
  //    throws an exception, the memory for the network priority
  //    mapping allocated above will be leaked.  Use an auto_ptr<> or
  //    the appropriate memory management tool.

  ACE_NEW_THROW_EX (network_manager,
                    TAO_Network_Priority_Mapping_Manager (npm),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  TAO_Network_Priority_Mapping_Manager_var safe_network_manager =
    network_manager;

  info->register_initial_reference ("NetworkPriorityMappingManager",
                                    network_manager);

  // Create the RT_ORB.
  CORBA::Object_ptr rt_orb = CORBA::Object::_nil ();
  ACE_NEW_THROW_EX (rt_orb,
                    TAO_RT_ORB (tao_info->orb_core (),
                    lifespan_,
                    dynamic_thread_time_),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  CORBA::Object_var safe_rt_orb = rt_orb;

  info->register_initial_reference (TAO_OBJID_RTORB, rt_orb);

  // Create the RT_Current.
  CORBA::Object_ptr current = CORBA::Object::_nil ();
  ACE_NEW_THROW_EX (current,
                    TAO_RT_Current (tao_info->orb_core ()),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  CORBA::Object_var safe_rt_current = current;

  info->register_initial_reference (TAO_OBJID_RTCURRENT, current);

  tao_info->orb_core ()->orb_params ()->scope_policy (this->scope_policy_);
  tao_info->orb_core ()->orb_params ()->sched_policy (this->sched_policy_);
  tao_info->orb_core ()->orb_params ()->ace_sched_policy (this->ace_sched_policy_);
}

void
TAO_RT_ORBInitializer::post_init (PortableInterceptor::ORBInitInfo_ptr info)
{
  this->register_policy_factories (info);
}

void
TAO_RT_ORBInitializer::register_policy_factories (
  PortableInterceptor::ORBInitInfo_ptr info)
{
  // The RTCorba policy factory is stateless and reentrant, so share a
  // single instance between all ORBs.
  if (CORBA::is_nil (this->policy_factory_.in ()))
    {
      PortableInterceptor::PolicyFactory_ptr policy_factory;
      ACE_NEW_THROW_EX (policy_factory,
                        TAO_RT_PolicyFactory,
                          CORBA::NO_MEMORY (
                            CORBA::SystemException::_tao_minor_code (
                              TAO::VMCID,
                              ENOMEM),
                            CORBA::COMPLETED_NO));

      this->policy_factory_ = policy_factory;
    }

  // Bind the same policy factory to all RTCORBA related policy
  // types since a single policy factory is used to create each of
  // the different types of RTCORBA policies.
  static CORBA::PolicyType const type[] = {
    RTCORBA::PRIORITY_MODEL_POLICY_TYPE,
    RTCORBA::THREADPOOL_POLICY_TYPE,
    RTCORBA::SERVER_PROTOCOL_POLICY_TYPE,
    RTCORBA::CLIENT_PROTOCOL_POLICY_TYPE,
    RTCORBA::PRIVATE_CONNECTION_POLICY_TYPE,
    RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE
  };

  const CORBA::PolicyType *end =
    type + sizeof (type) / sizeof (type[0]);

  for (CORBA::PolicyType const * i = type;
       i != end;
       ++i)
    {
      try
        {
          info->register_policy_factory (*i, this->policy_factory_.in ());
        }
      catch (const ::CORBA::BAD_INV_ORDER& ex)
        {
          if (ex.minor () == (CORBA::OMGVMCID | 16))
            {
              // The factory is already there, it happens because the
              // magic initializer in PortableServer.cpp registers
              // with the ORB multiple times.  This is an indication
              // that we should do no more work in this
              // ORBInitializer.
              return;
            }
          throw;
        }
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
