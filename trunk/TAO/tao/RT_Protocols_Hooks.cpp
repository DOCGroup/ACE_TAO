// $Id$

#include "RT_Protocols_Hooks.h"
#include "RT_Policy_i.h"

#include "Invocation.h"
#include "Stub.h"
#include "MProfile.h"
#include "Priority_Mapping_Manager.h"

#include "ace/Dynamic_Service.h"

ACE_RCSID (TAO,
           RT_Protocols_Hooks,
           "$Id$")

#if (TAO_HAS_RT_CORBA == 1)

TAO_RT_Protocols_Hooks::Client_Protocols_Hook TAO_RT_Protocols_Hooks::client_protocols_hook_ = 0;
TAO_RT_Protocols_Hooks::Server_Protocols_Hook TAO_RT_Protocols_Hooks::server_protocols_hook_ = 0;

/// constructor
TAO_RT_Protocols_Hooks::TAO_RT_Protocols_Hooks (void)
{
}

/// destructor
TAO_RT_Protocols_Hooks::~TAO_RT_Protocols_Hooks (void)
{
}

int
TAO_RT_Protocols_Hooks::call_client_protocols_hook (
                          TAO_ORB_Core *orb_core,
                          int &send_buffer_size,
                          int &recv_buffer_size,
                          int &no_delay,
                          const char *protocol_type)
{
  if (TAO_RT_Protocols_Hooks::client_protocols_hook_ == 0)
    return -1;

  (*TAO_RT_Protocols_Hooks::client_protocols_hook_) (orb_core,
                                                     send_buffer_size,
                                                     recv_buffer_size,
                                                     no_delay,
                                                     protocol_type);

  return 0;
}

void
TAO_RT_Protocols_Hooks::set_client_protocols_hook (Client_Protocols_Hook hook)
{
  TAO_RT_Protocols_Hooks::client_protocols_hook_ = hook;
  // Saving the hook pointer so that we can use it later when needed.

  return;
}

int
TAO_RT_Protocols_Hooks::call_server_protocols_hook (
                          TAO_ORB_Core *orb_core,
                          int &send_buffer_size,
                          int &recv_buffer_size,
                          int &no_delay,
                          const char *protocol_type)
{
  if (TAO_RT_Protocols_Hooks::server_protocols_hook_ == 0)
    return -1;

  int result_value =
    (*TAO_RT_Protocols_Hooks::server_protocols_hook_) (orb_core,
                                                       send_buffer_size,
                                                       recv_buffer_size,
                                                       no_delay,
                                                       protocol_type);

  if (result_value != 0)
    return -1;

  return 0;
}

void
TAO_RT_Protocols_Hooks::set_server_protocols_hook (Server_Protocols_Hook hook)
{
  TAO_RT_Protocols_Hooks::server_protocols_hook_ = hook;
  // Saving the hook pointer so that we can use it later when needed.

  return;
}

void
TAO_RT_Protocols_Hooks::call_policy_type_hook (CORBA::PolicyList *&policy_list,
                                               CORBA::ULong &policy_type)
{
  CORBA::ULong length = policy_list->length ();
  //  CORBA::ULong index = 0;

  for (unsigned int i = 0; i < length; ++i)
    {
      if (((*policy_list)[i]->policy_type () ==
           RTCORBA::PRIORITY_MODEL_POLICY_TYPE))
        policy_type = 0;

      else if (((*policy_list)[i]->policy_type () ==
                RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE))
        policy_type = 1;

      else if (((*policy_list)[i]->policy_type () ==
                RTCORBA::CLIENT_PROTOCOL_POLICY_TYPE))
        policy_type = 2;
    }
}

void
TAO_RT_Protocols_Hooks::validate_policy_type (CORBA::ULong type,
                                    CORBA::ULong &type_value,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  // Validity check.  Make sure requested policy type is appropriate
  // for this scope.
  if (type == RTCORBA::PRIORITY_MODEL_POLICY_TYPE)
    type_value = 1;

  if (type == RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE)
    type_value = 2;

  if (type == RTCORBA::CLIENT_PROTOCOL_POLICY_TYPE)
    type_value = 3;

  if ((type == RTCORBA::THREADPOOL_POLICY_TYPE)
      || (type == RTCORBA::SERVER_PROTOCOL_POLICY_TYPE))
    {
      type_value = 4;
      ACE_THROW (CORBA::INV_POLICY ());
    }
}

void
TAO_RT_Protocols_Hooks::add_rt_service_context_hook (
                           TAO_GIOP_Invocation *invocation,
                           CORBA::Policy *model_policy,
                           CORBA::Short &client_priority,
                           CORBA::Environment &ACE_TRY_ENV)
{

  RTCORBA::PriorityModelPolicy_var model_policy_ptr =
    RTCORBA::PriorityModelPolicy::_narrow (model_policy,
                                           ACE_TRY_ENV);
  ACE_CHECK;

  TAO_PriorityModelPolicy *priority_model =
    ACE_static_cast (TAO_PriorityModelPolicy *,
                     model_policy_ptr.in ());

  if (priority_model->get_priority_model () == RTCORBA::CLIENT_PROPAGATED)
    {
      // Encapsulate the priority of the current thread into
      // a service context.
      TAO_OutputCDR cdr;
      if ((cdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER)
           == 0)
          || (cdr << client_priority)
          == 0)
        ACE_THROW (CORBA::MARSHAL ());

      TAO_Service_Context &service_context = invocation->request_service_context ();
      service_context.set_context (IOP::RTCorbaPriority, cdr);
    }
}

void
TAO_RT_Protocols_Hooks::get_selector_hook (
                           CORBA::Policy *model_policy,
                           CORBA::Boolean &is_client_propagated,
                           CORBA::Short &server_priority)
{

  RTCORBA::PriorityModelPolicy_var model_policy_ptr =
    RTCORBA::PriorityModelPolicy::_narrow (model_policy);

  TAO_PriorityModelPolicy *priority_model_policy =
    ACE_static_cast (TAO_PriorityModelPolicy *,
                     model_policy_ptr.in ());

  if (priority_model_policy->get_priority_model ()
      == RTCORBA::CLIENT_PROPAGATED)
    is_client_propagated = 1;

  if (!is_client_propagated)
    server_priority = priority_model_policy->server_priority ();

  return;
}

void
TAO_RT_Protocols_Hooks::get_selector_bands_policy_hook (
                           CORBA::Policy *bands_policy,
                           CORBA::Short &min_priority,
                           CORBA::Short &max_priority,
                           CORBA::Short &p,
                           int &in_range)
{
  RTCORBA::PriorityBandedConnectionPolicy_var bands_policy_ptr =
    RTCORBA::PriorityBandedConnectionPolicy::_narrow (bands_policy);

  TAO_PriorityBandedConnectionPolicy *priority_bands_policy =
    ACE_static_cast (TAO_PriorityBandedConnectionPolicy *,
                     bands_policy_ptr.in ());

  // Find the band with the range covering our target priority.
  RTCORBA::PriorityBands &bands =
    priority_bands_policy->priority_bands_rep ();

  for (CORBA::ULong i = 0; i < bands.length (); ++i)
    if (bands[i].low <= p && bands[i].high >= p)
      {
        min_priority = bands[i].low;
        max_priority = bands[i].high;

        in_range = 1;
        break;
      }
  return;
}

CORBA::Policy *
TAO_RT_Protocols_Hooks::effective_priority_banded_connection_hook (
  CORBA::Policy *override,
  CORBA::Policy *exposed,
  CORBA::Environment &ACE_TRY_ENV)
{
  RTCORBA::PriorityBandedConnectionPolicy_var override_policy_ptr =
    RTCORBA::PriorityBandedConnectionPolicy::_narrow (override,
                                                      ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  TAO_PriorityBandedConnectionPolicy *override_policy =
    ACE_static_cast (TAO_PriorityBandedConnectionPolicy *,
                     override_policy_ptr.in ());

  RTCORBA::PriorityBandedConnectionPolicy_var exposed_policy_ptr =
    RTCORBA::PriorityBandedConnectionPolicy::_narrow (exposed,
                                                      ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  TAO_PriorityBandedConnectionPolicy *exposed_policy =
    ACE_static_cast (TAO_PriorityBandedConnectionPolicy *,
                      exposed_policy_ptr.in ());

  // Both override and exposed have been set.
  // See if either of them has empty priority bands.
  CORBA::Object_var auto_release_exp = exposed_policy;
  CORBA::Object_var auto_release_ov = override_policy;

  if (exposed_policy->priority_bands_rep ().length () == 0)
    {
      auto_release_ov._retn ();
      return override;
    }

  if (override_policy->priority_bands_rep ().length () == 0)
    {
      auto_release_exp._retn ();
      return exposed;
    }

  // Both override and exposed have been set and neither has empty
  // priority bands.  This is illegal (ptc/99-05-03, sec. 4.12.1).
  ACE_THROW_RETURN (CORBA::INV_POLICY (),
                    0);
}

CORBA::Policy *
TAO_RT_Protocols_Hooks::effective_client_protocol_hook (
                           CORBA::Policy_ptr override,
                           CORBA::Policy_ptr exposed,
                           CORBA::Environment &ACE_TRY_ENV)
{
  RTCORBA::ClientProtocolPolicy_var override_policy_var =
    RTCORBA::ClientProtocolPolicy::_narrow (override,
                                            ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  TAO_ClientProtocolPolicy *override_policy =
    ACE_static_cast (TAO_ClientProtocolPolicy *,
                     override_policy_var.in ());

  RTCORBA::ClientProtocolPolicy_var exposed_policy_var =
    RTCORBA::ClientProtocolPolicy::_narrow (exposed,
                                            ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  TAO_ClientProtocolPolicy *exposed_policy =
    ACE_static_cast (TAO_ClientProtocolPolicy *,
                     exposed_policy_var.in ());

  // Both override and exposed have been set.
  // See if either of them has empty priority bands.
  RTCORBA::ProtocolList &protocols_rep_var =
    exposed_policy->protocols_rep ();

  if (protocols_rep_var.length () == 0)
    {
      return override;
    }

  if (override_policy->protocols_rep ().length () == 0)
    {
      return exposed;
    }

  // Both override and exposed have been set and neither has empty
  // protocols.  This is illegal (ptc/99-05-03, sec. 4.15.4).
  ACE_THROW_RETURN (CORBA::INV_POLICY (),
                    0);
}

int
TAO_RT_Protocols_Hooks::get_thread_priority (TAO_ORB_Core *orb_core,
                                             CORBA::Short &priority,
                                             CORBA::Environment &ACE_TRY_ENV)
{
  ACE_hthread_t current;
  ACE_Thread::self (current);

  int native_priority;
  if (ACE_Thread::getprio (current, native_priority) == -1)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - ")
                  ACE_TEXT ("RT_Protocols_Hooks::get_thread_priority: ")
                  ACE_TEXT (" ACE_Thread::get_prio\n")));

      return -1;
    }

  CORBA::Object_var obj = orb_core->priority_mapping_manager ();

  TAO_Priority_Mapping_Manager_var mapping_manager =
    TAO_Priority_Mapping_Manager::_narrow (obj.in (),
                                           ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  TAO_Priority_Mapping *priority_mapping =
    mapping_manager.in ()->mapping ();

  if (priority_mapping->to_CORBA (native_priority, priority) == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - ")
                  ACE_TEXT ("RT_Protocols_Hooks::get_thread_priority: ")
                  ACE_TEXT ("Priority_Mapping::to_CORBA\n")));
      return -1;
    }

  return 0;
}

int
TAO_RT_Protocols_Hooks::set_thread_priority (TAO_ORB_Core *orb_core,
                                             CORBA::Short priority,
                                             CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Object_var obj = orb_core->priority_mapping_manager ();

  TAO_Priority_Mapping_Manager_var mapping_manager =
    TAO_Priority_Mapping_Manager::_narrow (obj.in (),
                                           ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  TAO_Priority_Mapping *priority_mapping =
    mapping_manager.in ()->mapping ();

  CORBA::Short native_priority;
  if (priority_mapping->to_native (priority, native_priority) == 0)
    return -1;

  ACE_hthread_t current;
  ACE_Thread::self (current);

  if (ACE_Thread::setprio (current, native_priority) == -1)
    return -1;

  return 0;
}

void
TAO_RT_Protocols_Hooks::set_priority_mapping (TAO_ORB_Core *orb_core,
                                              TAO_Resource_Factory *trf,
                                              CORBA::Environment &ACE_TRY_ENV)
{
  // Obtain a reference to the Priority Mapping Manager.
  CORBA::Object_var obj = orb_core->priority_mapping_manager ();

  TAO_Priority_Mapping_Manager_var priority_mapping_manager =
    TAO_Priority_Mapping_Manager::_narrow (obj.in (),
                                           ACE_TRY_ENV);
  ACE_CHECK;

  if (!CORBA::is_nil (priority_mapping_manager.in ()))
    priority_mapping_manager->mapping (trf->get_priority_mapping ());
  else
    ACE_THROW (CORBA::INV_OBJREF ());
}

int
TAO_RT_Protocols_Hooks::set_default_policies (TAO_ORB_Core *orb_core)
{
#if (TAO_HAS_RT_CORBA == 1)
  // Set RTCORBA policy defaults.
  // Set RTCORBA::ServerProtocolPolicy and
  // RTCORBA::ClientProtocolPolicy defaults to include all protocols
  // that were loaded into this ORB.
  // First, create a protocol list.

  TAO_ProtocolFactorySet *pfs = orb_core->protocol_factories ();

  RTCORBA::ProtocolList protocols;
  protocols.length (pfs->size ());

  int i = 0;
  for (TAO_ProtocolFactorySetItor factory = pfs->begin ();
       factory != pfs->end ();
       ++factory, ++i)
    {
      CORBA::ULong protocol_type = (*factory)->factory ()->tag ();
      protocols[i].protocol_type = protocol_type;
      protocols[i].orb_protocol_properties =
        RTCORBA::ProtocolProperties::_nil ();
      // @@ Later, we will likely migrate to using RTCORBA protocol
      // policies for configuration of protocols in nonRT use cases.
      // Then, the code below will change to each protocol factory
      // being responsible for creation of its own default protocol
      // properties.
      protocols[i].transport_protocol_properties =
        TAO_Protocol_Properties_Factory::create_transport_protocol_property
        (protocol_type);
    }

  // Set ServerProtocolPolicy.
  TAO_ServerProtocolPolicy *server_protocol_policy = 0;
  ACE_NEW_RETURN (server_protocol_policy,
                    TAO_ServerProtocolPolicy (protocols),
                  -1);

  RTCORBA::ServerProtocolPolicy_var safe_server_protocol_policy =
    server_protocol_policy;

  orb_core->get_default_policies ()->server_protocol (server_protocol_policy);

  // Set ClientProtocolPolicy.
  // NOTE: ClientProtocolPolicy default is used ONLY for protocol
  // configuration (not protocol preference) IF there is no ORB-level
  // override.  It is not used when computing effective policy value
  // for preferencing protocols.
  TAO_ClientProtocolPolicy *client_protocol_policy = 0;
  ACE_NEW_RETURN (client_protocol_policy,
                  TAO_ClientProtocolPolicy (protocols),
                  -1);

  RTCORBA::ClientProtocolPolicy_var safe_client_protocol_policy =
    client_protocol_policy;

  orb_core->get_default_policies ()->client_protocol (client_protocol_policy);

#endif /* TAO_HAS_RT_CORBA == 1 */

  return 0;
}

ACE_STATIC_SVC_DEFINE (TAO_RT_Protocols_Hooks,
                       ACE_TEXT ("RT_Protocols_Hooks"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_RT_Protocols_Hooks),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO, TAO_RT_Protocols_Hooks)

#endif /* TAO_HAS_RT_CORBA == 1 */

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<TAO_RT_Protocols_Hooks>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<TAO_RT_Protocols_Hooks>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
