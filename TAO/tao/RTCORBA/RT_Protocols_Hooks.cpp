// $Id$

#include "RT_Protocols_Hooks.h"
#include "RT_Policy_i.h"

#include "tao/Invocation.h"
#include "tao/Stub.h"
#include "tao/MProfile.h"
#include "tao/Acceptor_Registry.h"
#include "tao/Thread_Lane_Resources.h"
#include "Priority_Mapping_Manager.h"
#include "RT_Stub.h"

#include "ace/Dynamic_Service.h"

ACE_RCSID (TAO,
           RT_Protocols_Hooks,
           "$Id$")

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

void
TAO_RT_Protocols_Hooks::init_hooks (TAO_ORB_Core *orb_core
                                    TAO_ENV_ARG_DECL)
{
  this->orb_core_ = orb_core;

  // Save a reference to the priority mapping manager.
  CORBA::Object_var obj =
    orb_core->object_ref_table ().resolve_initial_references (TAO_OBJID_PRIORITYMAPPINGMANAGER
                                                              TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->mapping_manager_ =
    TAO_Priority_Mapping_Manager::_narrow (obj.in ()
                                           TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

int
TAO_RT_Protocols_Hooks::call_client_protocols_hook (int &send_buffer_size,
                                                    int &recv_buffer_size,
                                                    int &no_delay,
                                                    const char *protocol_type)
{
  if (TAO_RT_Protocols_Hooks::client_protocols_hook_ == 0)
    return -1;

  (*TAO_RT_Protocols_Hooks::client_protocols_hook_) (this->orb_core_,
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
}

int
TAO_RT_Protocols_Hooks::call_server_protocols_hook (int &send_buffer_size,
                                                    int &recv_buffer_size,
                                                    int &no_delay,
                                                    const char *protocol_type)
{
  if (TAO_RT_Protocols_Hooks::server_protocols_hook_ == 0)
    return -1;

  int result_value =
    (*TAO_RT_Protocols_Hooks::server_protocols_hook_) (this->orb_core_,
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
}

void
TAO_RT_Protocols_Hooks::rt_service_context (TAO_Stub *stub,
                                            TAO_Service_Context &service_context,
                                            CORBA::Boolean restart
                                            TAO_ENV_ARG_DECL)
{
  // If the restart flag is true, then this call for a
  // reinvocation. We need not prepare the Service Context List once
  // again. We can use the already existing one.
  if (!restart)
    {
      TAO_RT_Stub *rt_stub = ACE_dynamic_cast (TAO_RT_Stub *,
                                               stub);
      CORBA::Policy *priority_model_policy =
        rt_stub->exposed_priority_model (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
      if (priority_model_policy)
        {

          CORBA::Policy *priority_model_policy =
            rt_stub->exposed_priority_model (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;

          CORBA::Short client_priority;
          int status = this->get_thread_CORBA_priority (client_priority
                                                        TAO_ENV_ARG_PARAMETER);
          if (status == -1)
            ACE_THROW (CORBA::DATA_CONVERSION (1, CORBA::COMPLETED_NO));


          this->add_rt_service_context_hook (service_context,
                                             priority_model_policy,
                                             client_priority
                                             TAO_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
      else
        {
          // The Object does not contain PriorityModel policy in its IOR.
          // We must be talking to a non-RT ORB.  Do nothing.
        }
    }
}

void
TAO_RT_Protocols_Hooks::add_rt_service_context_hook (TAO_Service_Context &service_context,
                                                     CORBA::Policy *model_policy,
                                                     CORBA::Short &client_priority
                                                     TAO_ENV_ARG_DECL)
{

  RTCORBA::PriorityModelPolicy_var model_policy_ptr =
    RTCORBA::PriorityModelPolicy::_narrow (model_policy
                                           TAO_ENV_ARG_PARAMETER);
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
}

int
TAO_RT_Protocols_Hooks::get_thread_CORBA_priority (CORBA::Short &priority
                                                   TAO_ENV_ARG_DECL)
{
  CORBA::Short native_priority = 0;
  int result =
    this->get_thread_CORBA_and_native_priority (priority,
                                                native_priority
                                                TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (result == -1)
    return result;

  return 0;
}

int
TAO_RT_Protocols_Hooks::get_thread_native_priority (CORBA::Short &native_priority
                                                    TAO_ENV_ARG_DECL_NOT_USED)
{
  ACE_hthread_t current;
  ACE_Thread::self (current);

  int priority;
  if (ACE_Thread::getprio (current, priority) == -1)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - ")
                  ACE_TEXT ("RT_Protocols_Hooks::get_thread_priority: ")
                  ACE_TEXT (" ACE_Thread::get_prio\n")));

      return -1;
    }

  native_priority =
    CORBA::Short (priority);
  return 0;
}

int
TAO_RT_Protocols_Hooks::get_thread_CORBA_and_native_priority (CORBA::Short &priority,
                                                              CORBA::Short &native_priority
                                                              TAO_ENV_ARG_DECL)
{
  int result =
    this->get_thread_native_priority (native_priority
                                      TAO_ENV_ARG_PARAMETER);
  if (result == -1)
    return result;

  TAO_Priority_Mapping *priority_mapping =
    this->mapping_manager_.in ()->mapping ();

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
TAO_RT_Protocols_Hooks::set_thread_CORBA_priority (CORBA::Short priority
                                                   TAO_ENV_ARG_DECL)
{
  TAO_Priority_Mapping *priority_mapping =
    this->mapping_manager_.in ()->mapping ();

  CORBA::Short native_priority;
  if (priority_mapping->to_native (priority, native_priority) == 0)
    return -1;

  return this->set_thread_native_priority (native_priority
                                           TAO_ENV_ARG_PARAMETER);
}

int
TAO_RT_Protocols_Hooks::set_thread_native_priority (CORBA::Short native_priority
                                                    TAO_ENV_ARG_DECL_NOT_USED)
{
  ACE_hthread_t current;
  ACE_Thread::self (current);

  if (ACE_Thread::setprio (current, native_priority) == -1)
    return -1;

  return 0;
}

int
TAO_RT_Protocols_Hooks::set_default_policies (TAO_ENV_SINGLE_ARG_DECL)
{
  // Set RTCORBA policy defaults.
  // Set RTCORBA::ServerProtocolPolicy and
  // RTCORBA::ClientProtocolPolicy defaults to include all protocols
  // that were loaded into this ORB.
  // First, create a protocol list.

  TAO_ProtocolFactorySet *pfs = this->orb_core_->protocol_factories ();

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

  this->orb_core_->get_default_policies ()->set_policy (server_protocol_policy
                                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

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

  this->orb_core_->get_default_policies ()->set_policy (client_protocol_policy
                                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
TAO_RT_Protocols_Hooks::set_default_server_protocol_policy (TAO_Acceptor_Registry &acceptor_registry
                                                            TAO_ENV_ARG_DECL)
{
  RTCORBA::ProtocolList protocols;

  TAO_AcceptorSetIterator end =
    acceptor_registry.end ();

  for (TAO_AcceptorSetIterator acceptor =
         acceptor_registry.begin ();
       acceptor != end;
       ++acceptor)
    {
      if (*acceptor == 0)
        continue;

      CORBA::ULong current_length =
        protocols.length ();

      protocols.length (current_length + 1);

      protocols[current_length].protocol_type =
        (*acceptor)->tag ();

      protocols[current_length].orb_protocol_properties =
        RTCORBA::ProtocolProperties::_nil ();

      // @@ Later, we will likely migrate to using RTCORBA protocol
      // policies for configuration of protocols in nonRT use cases.
      // Then, the code below will change to each protocol factory
      // being responsible for creation of its own default protocol
      // properties.
      protocols[current_length].transport_protocol_properties =
        TAO_Protocol_Properties_Factory::create_transport_protocol_property ((*acceptor)->tag ());
    }

  // Set ServerProtocolPolicy.
  TAO_ServerProtocolPolicy *server_protocol_policy = 0;
  ACE_NEW_RETURN (server_protocol_policy,
                  TAO_ServerProtocolPolicy (protocols),
                  -1);

  RTCORBA::ServerProtocolPolicy_var safe_server_protocol_policy =
    server_protocol_policy;

  this->orb_core_->get_default_policies ()->set_policy (server_protocol_policy
                                                        TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}

ACE_STATIC_SVC_DEFINE (TAO_RT_Protocols_Hooks,
                       ACE_TEXT ("RT_Protocols_Hooks"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_RT_Protocols_Hooks),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_RTCORBA, TAO_RT_Protocols_Hooks)

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<TAO_RT_Protocols_Hooks>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<TAO_RT_Protocols_Hooks>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
