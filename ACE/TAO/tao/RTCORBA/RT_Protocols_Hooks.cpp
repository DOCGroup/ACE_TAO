#include "tao/RTCORBA/RT_Protocols_Hooks.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/RTCORBA/RT_Policy_i.h"
#include "tao/RTCORBA/Priority_Mapping_Manager.h"
#include "tao/RTCORBA/RT_Stub.h"

#include "tao/Stub.h"
#include "tao/ORB_Core.h"
#include "tao/MProfile.h"
#include "tao/Service_Context.h"
#include "tao/Acceptor_Registry.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Protocol_Factory.h"
#include "tao/Transport_Acceptor.h"
#include "tao/Transport_Connector.h"
#include "tao/Policy_Set.h"
#include "tao/debug.h"
#include "tao/CDR.h"

#include "ace/Dynamic_Service.h"
#include "ace/OS_NS_string.h"

ACE_RCSID (RTCORBA,
           RT_Protocols_Hooks,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_RT_Protocols_Hooks::TAO_RT_Protocols_Hooks (void)
  : orb_core_ (0)
  , current_ ()
{
}


TAO_RT_Protocols_Hooks::~TAO_RT_Protocols_Hooks (void)
{
}

void
TAO_RT_Protocols_Hooks::init_hooks (TAO_ORB_Core *orb_core
                                    ACE_ENV_ARG_DECL)
{
  this->orb_core_ = orb_core;

  // Save a reference to the priority mapping manager.
  CORBA::Object_var obj =
    orb_core->object_ref_table ().resolve_initial_reference (
      TAO_OBJID_PRIORITYMAPPINGMANAGER);

  this->mapping_manager_ =
    TAO_Priority_Mapping_Manager::_narrow (obj.in ()
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Save a reference to the priority mapping manager.
  CORBA::Object_var object =
    orb_core->object_ref_table ().resolve_initial_reference (
      TAO_OBJID_NETWORKPRIORITYMAPPINGMANAGER);

  this->network_mapping_manager_ =
    TAO_Network_Priority_Mapping_Manager::_narrow (object.in ()
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  object =
    this->orb_core_->object_ref_table ().resolve_initial_reference (
      TAO_OBJID_RTCURRENT);

  this->current_ =
    RTCORBA::Current::_narrow (object.in ()
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

}

RTCORBA::ProtocolProperties_ptr
TAO_RT_Protocols_Hooks::server_protocol_properties (IOP::ProfileId protocol_tag,
                                                    CORBA::Policy_ptr policy
                                                    ACE_ENV_ARG_DECL)
{
  if (CORBA::is_nil (policy))
    return 0;

  RTCORBA::ServerProtocolPolicy_var server_protocol_policy =
    RTCORBA::ServerProtocolPolicy::_narrow (policy
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (CORBA::is_nil (server_protocol_policy.in ()))
    return 0;

  TAO_ServerProtocolPolicy *server_protocols =
    dynamic_cast<TAO_ServerProtocolPolicy *> (server_protocol_policy.in ());

  if (server_protocols == 0)
    return 0;

  // TAO_ServerProtocolPolicy
  RTCORBA::ProtocolList &protocols =
    server_protocols->protocols_rep ();

  for (CORBA::ULong j = 0; j < protocols.length (); ++j)
    {
      if (protocols[j].protocol_type == protocol_tag)
        {
          return RTCORBA::ProtocolProperties::_duplicate (protocols[j].transport_protocol_properties.in ());
        }
    }

  return 0;
}

RTCORBA::ProtocolProperties_ptr
TAO_RT_Protocols_Hooks::client_protocol_properties (IOP::ProfileId protocol_tag,
                                                    CORBA::Policy_ptr policy
                                                    ACE_ENV_ARG_DECL)
{
  if (CORBA::is_nil (policy))
    return 0;

  RTCORBA::ClientProtocolPolicy_var client_protocol_policy =
    RTCORBA::ClientProtocolPolicy::_narrow (policy
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (CORBA::is_nil (client_protocol_policy.in ()))
    return 0;

  TAO_ClientProtocolPolicy *client_protocols =
    dynamic_cast<TAO_ClientProtocolPolicy *> (client_protocol_policy.in ());

  if (client_protocols == 0)
    return 0;

  // TAO_ClientProtocolPolicy
  RTCORBA::ProtocolList &protocols =
    client_protocols->protocols_rep ();

  for (CORBA::ULong j = 0; j < protocols.length (); ++j)
    {
      if (protocols[j].protocol_type == protocol_tag)
        {
          return RTCORBA::ProtocolProperties::_duplicate (protocols[j].transport_protocol_properties.in ());
        }
    }

  return 0;
}

RTCORBA::ProtocolProperties_ptr
TAO_RT_Protocols_Hooks::server_protocol_properties_at_orb_level (IOP::ProfileId protocol_tag
                                                                 ACE_ENV_ARG_DECL)
{
  CORBA::Policy_var policy =
    this->orb_core_->get_cached_policy (TAO_CACHED_POLICY_RT_SERVER_PROTOCOL
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->server_protocol_properties (protocol_tag,
                                           policy.in ()
                                           ACE_ENV_ARG_PARAMETER);
}

RTCORBA::ProtocolProperties_ptr
TAO_RT_Protocols_Hooks::client_protocol_properties_at_orb_level (IOP::ProfileId protocol_tag
                                                                 ACE_ENV_ARG_DECL)
{
  CORBA::Policy_var policy =
    this->orb_core_->get_cached_policy (TAO_CACHED_POLICY_RT_CLIENT_PROTOCOL
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->client_protocol_properties (protocol_tag,
                                           policy.in ()
                                           ACE_ENV_ARG_PARAMETER);
}

RTCORBA::ProtocolProperties_ptr
TAO_RT_Protocols_Hooks::client_protocol_properties_at_object_level (IOP::ProfileId protocol_tag,
                                                                    TAO_Stub *stub
                                                                    ACE_ENV_ARG_DECL)
{
  CORBA::Policy_var policy =
    stub->get_cached_policy (TAO_CACHED_POLICY_RT_CLIENT_PROTOCOL
                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->client_protocol_properties (protocol_tag,
                                           policy.in ()
                                           ACE_ENV_ARG_PARAMETER);
}

void
TAO_RT_Protocols_Hooks::extract_protocol_properties (TAO_IIOP_Protocol_Properties &to,
                                                     RTCORBA::ProtocolProperties_ptr from
                                                     ACE_ENV_ARG_DECL)
{
  RTCORBA::TCPProtocolProperties_var protocol_properties =
    RTCORBA::TCPProtocolProperties::_narrow (from
                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  to.send_buffer_size_ = protocol_properties->send_buffer_size ();
  to.recv_buffer_size_ = protocol_properties->recv_buffer_size ();
  to.keep_alive_ = protocol_properties->keep_alive ();
  to.dont_route_ = protocol_properties->dont_route ();
  to.no_delay_ = protocol_properties->no_delay ();
  to.enable_network_priority_ = protocol_properties->enable_network_priority ();
}

void
TAO_RT_Protocols_Hooks::server_protocol_properties_at_orb_level (TAO_IIOP_Protocol_Properties &to
                                                                 ACE_ENV_ARG_DECL)
{
  RTCORBA::ProtocolProperties_var from =
    this->server_protocol_properties_at_orb_level (IOP::TAG_INTERNET_IOP
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (!CORBA::is_nil (from.in ()))
    this->extract_protocol_properties (to,
                                       from.in ()
                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_RT_Protocols_Hooks::client_protocol_properties_at_orb_level (TAO_IIOP_Protocol_Properties &to
                                                                 ACE_ENV_ARG_DECL)
{
  RTCORBA::ProtocolProperties_var from =
    this->client_protocol_properties_at_orb_level (IOP::TAG_INTERNET_IOP
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (!CORBA::is_nil (from.in ()))
    this->extract_protocol_properties (to,
                                       from.in ()
                                       ACE_ENV_ARG_PARAMETER);
}

void
TAO_RT_Protocols_Hooks::extract_protocol_properties (TAO_UIOP_Protocol_Properties &to,
                                                     RTCORBA::ProtocolProperties_ptr from
                                                     ACE_ENV_ARG_DECL)
{
  RTCORBA::UnixDomainProtocolProperties_var protocol_properties =
    RTCORBA::UnixDomainProtocolProperties::_narrow (from
                                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  to.send_buffer_size_ = protocol_properties->send_buffer_size ();
  to.recv_buffer_size_ = protocol_properties->recv_buffer_size ();
}

void
TAO_RT_Protocols_Hooks::server_protocol_properties_at_orb_level (TAO_UIOP_Protocol_Properties &to
                                                                 ACE_ENV_ARG_DECL)
{
  RTCORBA::ProtocolProperties_var from =
    this->server_protocol_properties_at_orb_level (TAO_TAG_UIOP_PROFILE
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (!CORBA::is_nil (from.in ()))
    this->extract_protocol_properties (to,
                                       from.in ()
                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_RT_Protocols_Hooks::client_protocol_properties_at_orb_level (TAO_UIOP_Protocol_Properties &to
                                                                 ACE_ENV_ARG_DECL)
{
  RTCORBA::ProtocolProperties_var from =
    this->client_protocol_properties_at_orb_level (TAO_TAG_UIOP_PROFILE
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (!CORBA::is_nil (from.in ()))
    this->extract_protocol_properties (to,
                                       from.in ()
                                       ACE_ENV_ARG_PARAMETER);
}

void
TAO_RT_Protocols_Hooks::extract_protocol_properties (TAO_SHMIOP_Protocol_Properties &to,
                                                     RTCORBA::ProtocolProperties_ptr from
                                                     ACE_ENV_ARG_DECL)
{
  RTCORBA::SharedMemoryProtocolProperties_var protocol_properties =
    RTCORBA::SharedMemoryProtocolProperties::_narrow (from
                                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  to.preallocate_buffer_size_ = protocol_properties->preallocate_buffer_size ();
  to.mmap_filename_ = protocol_properties->mmap_filename ();
  to.mmap_lockname_ = protocol_properties->mmap_lockname ();
}

void
TAO_RT_Protocols_Hooks::server_protocol_properties_at_orb_level (TAO_SHMIOP_Protocol_Properties &to
                                                                 ACE_ENV_ARG_DECL)
{
  RTCORBA::ProtocolProperties_var from =
    this->server_protocol_properties_at_orb_level (TAO_TAG_SHMEM_PROFILE
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (!CORBA::is_nil (from.in ()))
    this->extract_protocol_properties (to,
                                       from.in ()
                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_RT_Protocols_Hooks::client_protocol_properties_at_orb_level (TAO_SHMIOP_Protocol_Properties &to
                                                                 ACE_ENV_ARG_DECL)
{
  RTCORBA::ProtocolProperties_var from =
    this->client_protocol_properties_at_orb_level (TAO_TAG_SHMEM_PROFILE
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (!CORBA::is_nil (from.in ()))
    this->extract_protocol_properties (to,
                                       from.in ()
                                       ACE_ENV_ARG_PARAMETER);
}

void
TAO_RT_Protocols_Hooks::extract_protocol_properties (TAO_DIOP_Protocol_Properties &to,
                                                     RTCORBA::ProtocolProperties_ptr from
                                                     ACE_ENV_ARG_DECL)
{
  RTCORBA::UserDatagramProtocolProperties_var protocol_properties =
    RTCORBA::UserDatagramProtocolProperties::_narrow (from
                                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  to.enable_network_priority_ = protocol_properties->enable_network_priority ();
}

void
TAO_RT_Protocols_Hooks::server_protocol_properties_at_orb_level (TAO_DIOP_Protocol_Properties &to
                                                                 ACE_ENV_ARG_DECL)
{
  RTCORBA::ProtocolProperties_var from =
    this->server_protocol_properties_at_orb_level (TAO_TAG_DIOP_PROFILE
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (!CORBA::is_nil (from.in ()))
    this->extract_protocol_properties (to,
                                       from.in ()
                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_RT_Protocols_Hooks::client_protocol_properties_at_orb_level (TAO_DIOP_Protocol_Properties &to
                                                                 ACE_ENV_ARG_DECL)
{
  RTCORBA::ProtocolProperties_var from =
    this->client_protocol_properties_at_orb_level (TAO_TAG_DIOP_PROFILE
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (!CORBA::is_nil (from.in ()))
    this->extract_protocol_properties (to,
                                       from.in ()
                                       ACE_ENV_ARG_PARAMETER);
}

void
TAO_RT_Protocols_Hooks::extract_protocol_properties (TAO_SCIOP_Protocol_Properties &to,
                                                     RTCORBA::ProtocolProperties_ptr from
                                                     ACE_ENV_ARG_DECL)
{
  RTCORBA::StreamControlProtocolProperties_var protocol_properties =
    RTCORBA::StreamControlProtocolProperties::_narrow (from
                                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  to.send_buffer_size_ = protocol_properties->send_buffer_size ();
  to.recv_buffer_size_ = protocol_properties->recv_buffer_size ();
  to.keep_alive_ = protocol_properties->keep_alive ();
  to.dont_route_ = protocol_properties->dont_route ();
  to.no_delay_ = protocol_properties->no_delay ();
  to.enable_network_priority_ = protocol_properties->enable_network_priority ();
}

void
TAO_RT_Protocols_Hooks::server_protocol_properties_at_orb_level (TAO_SCIOP_Protocol_Properties &to
                                                                 ACE_ENV_ARG_DECL)
{
  RTCORBA::ProtocolProperties_var from =
    this->server_protocol_properties_at_orb_level (TAO_TAG_SCIOP_PROFILE
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (!CORBA::is_nil (from.in ()))
    this->extract_protocol_properties (to,
                                       from.in ()
                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_RT_Protocols_Hooks::client_protocol_properties_at_orb_level (TAO_SCIOP_Protocol_Properties &to
                                                                 ACE_ENV_ARG_DECL)
{
  RTCORBA::ProtocolProperties_var from =
    this->client_protocol_properties_at_orb_level (TAO_TAG_SCIOP_PROFILE
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (!CORBA::is_nil (from.in ()))
    this->extract_protocol_properties (to,
                                       from.in ()
                                       ACE_ENV_ARG_PARAMETER);
}

CORBA::Boolean
TAO_RT_Protocols_Hooks::set_network_priority (IOP::ProfileId protocol_tag,
                                              RTCORBA::ProtocolProperties_ptr protocol_properties
                                              ACE_ENV_ARG_DECL)
{
  if (CORBA::is_nil (protocol_properties))
      return false;

  if (protocol_tag == IOP::TAG_INTERNET_IOP)
    {
      RTCORBA::TCPProtocolProperties_var tcp_protocol_properties =
        RTCORBA::TCPProtocolProperties::_narrow (protocol_properties
                                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (false);

      return tcp_protocol_properties->enable_network_priority ();
    }

  if (protocol_tag == TAO_TAG_DIOP_PROFILE)
    {
      RTCORBA::UserDatagramProtocolProperties_var udp_protocol_properties =
        RTCORBA::UserDatagramProtocolProperties::_narrow (protocol_properties
                                                          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (false);

      return udp_protocol_properties->enable_network_priority ();
    }

  if (protocol_tag == TAO_TAG_SCIOP_PROFILE)
    {
      RTCORBA::StreamControlProtocolProperties_var sctp_protocol_properties =
        RTCORBA::StreamControlProtocolProperties::_narrow (protocol_properties
                                                           ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (false);

      return sctp_protocol_properties->enable_network_priority ();
    }

  return false;
}

CORBA::Boolean
TAO_RT_Protocols_Hooks::set_client_network_priority (IOP::ProfileId protocol_tag,
                                                     TAO_Stub *stub
                                                     ACE_ENV_ARG_DECL)
{
  if (protocol_tag != IOP::TAG_INTERNET_IOP &&
      protocol_tag != TAO_TAG_DIOP_PROFILE &&
      protocol_tag != TAO_TAG_SCIOP_PROFILE)
    return false;

  RTCORBA::ProtocolProperties_var protocol_properties =
    this->client_protocol_properties_at_object_level (protocol_tag,
                                                      stub
                                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (false);

  return this->set_network_priority (protocol_tag,
                                     protocol_properties.in ()
                                     ACE_ENV_ARG_PARAMETER);
}

CORBA::Boolean
TAO_RT_Protocols_Hooks::set_server_network_priority (IOP::ProfileId protocol_tag,
                                                     CORBA::Policy *policy
                                                     ACE_ENV_ARG_DECL)
{
  if (protocol_tag != IOP::TAG_INTERNET_IOP &&
      protocol_tag != TAO_TAG_DIOP_PROFILE &&
      protocol_tag != TAO_TAG_SCIOP_PROFILE)
    return false;

  RTCORBA::ProtocolProperties_var protocol_properties =
    this->server_protocol_properties (protocol_tag,
                                      policy
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (false);

  return this->set_network_priority (protocol_tag,
                                     protocol_properties.in ()
                                     ACE_ENV_ARG_PARAMETER);
}

CORBA::Long
TAO_RT_Protocols_Hooks::get_dscp_codepoint (void)
{
  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG,
                "TAO_RT_Protocols_Hooks::get_dscp_codepoint\n"));

  CORBA::Long codepoint = 0;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Make several invocation, changing the priority of this thread
      // for each.

      RTCORBA::NetworkPriorityMapping *pm =
        this->network_mapping_manager_->mapping ();

      const CORBA::Short priority =
        this->current_->the_priority (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (pm->to_network (priority, codepoint) == 0)
        {
          if (TAO_debug_level > 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "Cannot convert corba priority %d "
                          "to network priority\n",
                          priority));
            }

          return -1;
        }
    }
  ACE_CATCHANY
    {
      if (TAO_debug_level > 0)
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               "TAO_RT_Protocols_Hooks::get_dscp_codepoint");
        }

      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return codepoint;
}

void
TAO_RT_Protocols_Hooks::rt_service_context (
    TAO_Stub *stub,
    TAO_Service_Context &service_context,
    CORBA::Boolean restart
    ACE_ENV_ARG_DECL
  )
{
  // If the restart flag is true, then this call for a
  // reinvocation. We need not prepare the Service Context List once
  // again. We can use the already existing one.
  if (!restart)
    {
      TAO_RT_Stub *rt_stub =
        dynamic_cast<TAO_RT_Stub *> (stub);

      CORBA::Policy_var priority_model_policy =
        rt_stub->get_cached_policy (TAO_CACHED_POLICY_PRIORITY_MODEL
                                    ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (!CORBA::is_nil (priority_model_policy.in ()))
        {
          CORBA::Short client_priority;

          int status =
            this->get_thread_CORBA_priority (client_priority
                                             ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          if (status == -1)
            ACE_THROW (CORBA::DATA_CONVERSION (1, CORBA::COMPLETED_NO));

          this->add_rt_service_context_hook (service_context,
                                             priority_model_policy.in (),
                                             client_priority
                                             ACE_ENV_ARG_PARAMETER);
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
TAO_RT_Protocols_Hooks::add_rt_service_context_hook (
    TAO_Service_Context &service_context,
    CORBA::Policy *model_policy,
    CORBA::Short &client_priority
    ACE_ENV_ARG_DECL
  )
{

  RTCORBA::PriorityModelPolicy_var model_policy_ptr =
    RTCORBA::PriorityModelPolicy::_narrow (model_policy
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  TAO_PriorityModelPolicy *priority_model =
    static_cast<TAO_PriorityModelPolicy *> (model_policy_ptr.in ());

  if (priority_model->get_priority_model () == RTCORBA::CLIENT_PROPAGATED)
    {
      // Encapsulate the priority of the current thread into
      // a service context.
      TAO_OutputCDR cdr;
      if ((cdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER) == 0)
          || (cdr << client_priority) == 0)
        {
          ACE_THROW (CORBA::MARSHAL ());
        }

      service_context.set_context (IOP::RTCorbaPriority, cdr);
    }
}

void
TAO_RT_Protocols_Hooks::get_selector_hook (
    CORBA::Policy *model_policy,
    CORBA::Boolean &is_client_propagated,
    CORBA::Short &server_priority
  )
{

  RTCORBA::PriorityModelPolicy_var model_policy_ptr =
    RTCORBA::PriorityModelPolicy::_narrow (model_policy);

  TAO_PriorityModelPolicy *priority_model_policy =
    static_cast<TAO_PriorityModelPolicy *> (model_policy_ptr.in ());

  if (priority_model_policy->get_priority_model ()
        == RTCORBA::CLIENT_PROPAGATED)
    {
      is_client_propagated = 1;
    }

  if (!is_client_propagated)
    {
      server_priority = priority_model_policy->server_priority ();
    }

  return;
}

void
TAO_RT_Protocols_Hooks::get_selector_bands_policy_hook (
    CORBA::Policy *bands_policy,
    CORBA::Short priority,
    CORBA::Short &min_priority,
    CORBA::Short &max_priority,
    int &in_range
  )
{
  RTCORBA::PriorityBandedConnectionPolicy_var bands_policy_ptr =
    RTCORBA::PriorityBandedConnectionPolicy::_narrow (bands_policy);

  TAO_PriorityBandedConnectionPolicy *priority_bands_policy =
    static_cast<TAO_PriorityBandedConnectionPolicy *> (bands_policy_ptr.in ());

  // Find the band with the range covering our target priority.
  RTCORBA::PriorityBands &bands =
    priority_bands_policy->priority_bands_rep ();

  for (CORBA::ULong i = 0; i < bands.length (); ++i)
    {
      if (bands[i].low <= priority && bands[i].high >= priority)
        {
          min_priority = bands[i].low;
          max_priority = bands[i].high;

          in_range = 1;
          break;
        }
    }
}

int
TAO_RT_Protocols_Hooks::get_thread_CORBA_priority (CORBA::Short &priority
                                                   ACE_ENV_ARG_DECL)
{
  CORBA::Short native_priority = 0;
  int const result =
    this->get_thread_CORBA_and_native_priority (priority,
                                                native_priority
                                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (result == -1)
    {
      return result;
    }

  return 0;
}

int
TAO_RT_Protocols_Hooks::get_thread_native_priority (
    CORBA::Short &native_priority
    ACE_ENV_ARG_DECL_NOT_USED
  )
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

  native_priority = CORBA::Short (priority);
  return 0;
}

int
TAO_RT_Protocols_Hooks::get_thread_CORBA_and_native_priority (
    CORBA::Short &priority,
    CORBA::Short &native_priority
    ACE_ENV_ARG_DECL
  )
{
  int result =
    this->get_thread_native_priority (native_priority
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (result == -1)
    {
      return result;
    }

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
                                                   ACE_ENV_ARG_DECL)
{
  TAO_Priority_Mapping *priority_mapping =
    this->mapping_manager_.in ()->mapping ();

  CORBA::Short native_priority;

  if (priority_mapping->to_native (priority, native_priority) == 0)
    {
      return -1;
    }

  return this->set_thread_native_priority (native_priority
                                           ACE_ENV_ARG_PARAMETER);
}

int
TAO_RT_Protocols_Hooks::set_thread_native_priority (
    CORBA::Short native_priority
    ACE_ENV_ARG_DECL_NOT_USED
   )
{
  ACE_hthread_t current;
  ACE_Thread::self (current);

  if (ACE_Thread::setprio (current, native_priority) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N,%l) Error setting thread ")
                         ACE_TEXT ("priority to %d, errno %d %m\n"),
                         native_priority,
                         errno ),
                       -1);
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DEFINE (TAO_RT_Protocols_Hooks,
                       ACE_TEXT ("RT_Protocols_Hooks"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_RT_Protocols_Hooks),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_RTCORBA, TAO_RT_Protocols_Hooks)

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
