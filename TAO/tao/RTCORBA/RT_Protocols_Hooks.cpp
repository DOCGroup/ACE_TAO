// $Id$

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
#include "tao/TSS_Resources.h"
#include "tao/Policy_Set.h"
#include "tao/debug.h"
#include "tao/CDR.h"
#include "tao/SystemException.h"

#include "ace/Dynamic_Service.h"
#include "ace/OS_NS_string.h"

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
TAO_RT_Protocols_Hooks::init_hooks (TAO_ORB_Core *orb_core)
{
  this->orb_core_ = orb_core;

  // Save a reference to the priority mapping manager.
  CORBA::Object_var obj =
    orb_core->object_ref_table ().resolve_initial_reference (
      TAO_OBJID_PRIORITYMAPPINGMANAGER);

  this->mapping_manager_ =
    TAO_Priority_Mapping_Manager::_narrow (obj.in ());

  // Save a reference to the priority mapping manager.
  CORBA::Object_var object =
    orb_core->object_ref_table ().resolve_initial_reference (
      TAO_OBJID_NETWORKPRIORITYMAPPINGMANAGER);

  this->network_mapping_manager_ =
    TAO_Network_Priority_Mapping_Manager::_narrow (object.in ());

  object =
    this->orb_core_->object_ref_table ().resolve_initial_reference (
      TAO_OBJID_RTCURRENT);

  this->current_ = RTCORBA::Current::_narrow (object.in ());

}

RTCORBA::ProtocolProperties_ptr
TAO_RT_Protocols_Hooks::server_protocol_properties (IOP::ProfileId protocol_tag,
                                                    CORBA::Policy_ptr policy)
{
  if (CORBA::is_nil (policy))
    return RTCORBA::ProtocolProperties::_nil ();

  RTCORBA::ServerProtocolPolicy_var server_protocol_policy =
    RTCORBA::ServerProtocolPolicy::_narrow (policy);

  if (CORBA::is_nil (server_protocol_policy.in ()))
    return RTCORBA::ProtocolProperties::_nil ();

  TAO_ServerProtocolPolicy *server_protocols =
    dynamic_cast<TAO_ServerProtocolPolicy *> (server_protocol_policy.in ());

  if (server_protocols == 0)
    return RTCORBA::ProtocolProperties::_nil ();

  // TAO_ServerProtocolPolicy
  RTCORBA::ProtocolList &protocols =
    server_protocols->protocols_rep ();

  for (CORBA::ULong j = 0; j < protocols.length (); ++j)
    {
      if (protocols[j].protocol_type == protocol_tag)
        {
          return RTCORBA::ProtocolProperties::_duplicate (
                  protocols[j].transport_protocol_properties.in ());
        }
    }

  return RTCORBA::ProtocolProperties::_nil ();
}

RTCORBA::ProtocolProperties_ptr
TAO_RT_Protocols_Hooks::client_protocol_properties (IOP::ProfileId protocol_tag,
                                                    CORBA::Policy_ptr policy)
{
  if (CORBA::is_nil (policy))
    return RTCORBA::ProtocolProperties::_nil ();

  RTCORBA::ClientProtocolPolicy_var client_protocol_policy =
    RTCORBA::ClientProtocolPolicy::_narrow (policy);

  if (CORBA::is_nil (client_protocol_policy.in ()))
    return RTCORBA::ProtocolProperties::_nil ();

  TAO_ClientProtocolPolicy *client_protocols =
    dynamic_cast<TAO_ClientProtocolPolicy *> (client_protocol_policy.in ());

  if (client_protocols == 0)
    return RTCORBA::ProtocolProperties::_nil ();

  // TAO_ClientProtocolPolicy
  RTCORBA::ProtocolList &protocols =
    client_protocols->protocols_rep ();

  for (CORBA::ULong j = 0; j < protocols.length (); ++j)
    {
      if (protocols[j].protocol_type == protocol_tag)
        {
          return RTCORBA::ProtocolProperties::_duplicate (
                  protocols[j].transport_protocol_properties.in ());
        }
    }

  return RTCORBA::ProtocolProperties::_nil ();
}

RTCORBA::ProtocolProperties_ptr
TAO_RT_Protocols_Hooks::server_protocol_properties_at_orb_level (
  IOP::ProfileId protocol_tag)
{
  CORBA::Policy_var policy =
    this->orb_core_->get_cached_policy (TAO_CACHED_POLICY_RT_SERVER_PROTOCOL);

  return this->server_protocol_properties (protocol_tag, policy.in ());
}

RTCORBA::ProtocolProperties_ptr
TAO_RT_Protocols_Hooks::client_protocol_properties_at_orb_level (
  IOP::ProfileId protocol_tag)
{
  CORBA::Policy_var policy =
    this->orb_core_->get_cached_policy (TAO_CACHED_POLICY_RT_CLIENT_PROTOCOL);

  return this->client_protocol_properties (protocol_tag, policy.in ());
}

RTCORBA::ProtocolProperties_ptr
TAO_RT_Protocols_Hooks::client_protocol_properties_at_object_level (
  IOP::ProfileId protocol_tag,
  TAO_Stub *stub)
{
  CORBA::Policy_var policy =
    stub->get_cached_policy (TAO_CACHED_POLICY_RT_CLIENT_PROTOCOL);

  return this->client_protocol_properties (protocol_tag, policy.in ());
}

void
TAO_RT_Protocols_Hooks::extract_protocol_properties (TAO_IIOP_Protocol_Properties &to,
                                                     RTCORBA::ProtocolProperties_ptr from)
{
  RTCORBA::TCPProtocolProperties_var protocol_properties =
    RTCORBA::TCPProtocolProperties::_narrow (from);

  to.send_buffer_size_ = protocol_properties->send_buffer_size ();
  to.recv_buffer_size_ = protocol_properties->recv_buffer_size ();
  to.keep_alive_ = protocol_properties->keep_alive ();
  to.dont_route_ = protocol_properties->dont_route ();
  to.no_delay_ = protocol_properties->no_delay ();
  to.enable_network_priority_ = protocol_properties->enable_network_priority ();
}

void
TAO_RT_Protocols_Hooks::server_protocol_properties_at_orb_level (TAO_IIOP_Protocol_Properties &to)
{
  RTCORBA::ProtocolProperties_var from =
    this->server_protocol_properties_at_orb_level (IOP::TAG_INTERNET_IOP);

  if (!CORBA::is_nil (from.in ()))
    this->extract_protocol_properties (to, from.in ());
}

void
TAO_RT_Protocols_Hooks::client_protocol_properties_at_orb_level (TAO_IIOP_Protocol_Properties &to)
{
  RTCORBA::ProtocolProperties_var from =
    this->client_protocol_properties_at_orb_level (IOP::TAG_INTERNET_IOP);

  if (!CORBA::is_nil (from.in ()))
    this->extract_protocol_properties (to, from.in ());
}

void
TAO_RT_Protocols_Hooks::extract_protocol_properties (
  TAO_UIOP_Protocol_Properties &to,
  RTCORBA::ProtocolProperties_ptr from)
{
  RTCORBA::UnixDomainProtocolProperties_var protocol_properties =
    RTCORBA::UnixDomainProtocolProperties::_narrow (from);

  to.send_buffer_size_ = protocol_properties->send_buffer_size ();
  to.recv_buffer_size_ = protocol_properties->recv_buffer_size ();
}

void
TAO_RT_Protocols_Hooks::server_protocol_properties_at_orb_level (
  TAO_UIOP_Protocol_Properties &to)
{
  RTCORBA::ProtocolProperties_var from =
    this->server_protocol_properties_at_orb_level (TAO_TAG_UIOP_PROFILE);

  if (!CORBA::is_nil (from.in ()))
    this->extract_protocol_properties (to, from.in ());
}

void
TAO_RT_Protocols_Hooks::client_protocol_properties_at_orb_level (
  TAO_UIOP_Protocol_Properties &to)
{
  RTCORBA::ProtocolProperties_var from =
    this->client_protocol_properties_at_orb_level (TAO_TAG_UIOP_PROFILE);

  if (!CORBA::is_nil (from.in ()))
    this->extract_protocol_properties (to, from.in ());
}

void
TAO_RT_Protocols_Hooks::extract_protocol_properties (
  TAO_SHMIOP_Protocol_Properties &to,
  RTCORBA::ProtocolProperties_ptr from)
{
  RTCORBA::SharedMemoryProtocolProperties_var protocol_properties =
    RTCORBA::SharedMemoryProtocolProperties::_narrow (from);

  to.preallocate_buffer_size_ = protocol_properties->preallocate_buffer_size ();
  to.mmap_filename_ = protocol_properties->mmap_filename ();
  to.mmap_lockname_ = protocol_properties->mmap_lockname ();
}

void
TAO_RT_Protocols_Hooks::server_protocol_properties_at_orb_level (
  TAO_SHMIOP_Protocol_Properties &to)
{
  RTCORBA::ProtocolProperties_var from =
    this->server_protocol_properties_at_orb_level (TAO_TAG_SHMEM_PROFILE);

  if (!CORBA::is_nil (from.in ()))
    this->extract_protocol_properties (to, from.in ());
}

void
TAO_RT_Protocols_Hooks::client_protocol_properties_at_orb_level (
  TAO_SHMIOP_Protocol_Properties &to)
{
  RTCORBA::ProtocolProperties_var from =
    this->client_protocol_properties_at_orb_level (TAO_TAG_SHMEM_PROFILE);

  if (!CORBA::is_nil (from.in ()))
    this->extract_protocol_properties (to, from.in ());
}

void
TAO_RT_Protocols_Hooks::extract_protocol_properties (
  TAO_DIOP_Protocol_Properties &to,
  RTCORBA::ProtocolProperties_ptr from)
{
  RTCORBA::UserDatagramProtocolProperties_var protocol_properties =
    RTCORBA::UserDatagramProtocolProperties::_narrow (from);

  to.enable_network_priority_ = protocol_properties->enable_network_priority ();
  to.send_buffer_size_ = protocol_properties->send_buffer_size ();
  to.recv_buffer_size_ = protocol_properties->recv_buffer_size ();
}

void
TAO_RT_Protocols_Hooks::server_protocol_properties_at_orb_level (
  TAO_DIOP_Protocol_Properties &to)
{
  RTCORBA::ProtocolProperties_var from =
    this->server_protocol_properties_at_orb_level (TAO_TAG_DIOP_PROFILE);

  if (!CORBA::is_nil (from.in ()))
    this->extract_protocol_properties (to, from.in ());
}

void
TAO_RT_Protocols_Hooks::client_protocol_properties_at_orb_level (
  TAO_DIOP_Protocol_Properties &to)
{
  RTCORBA::ProtocolProperties_var from =
    this->client_protocol_properties_at_orb_level (TAO_TAG_DIOP_PROFILE);

  if (!CORBA::is_nil (from.in ()))
    this->extract_protocol_properties (to, from.in ());
}

void
TAO_RT_Protocols_Hooks::extract_protocol_properties (
  TAO_SCIOP_Protocol_Properties &to,
  RTCORBA::ProtocolProperties_ptr from)
{
  RTCORBA::StreamControlProtocolProperties_var protocol_properties =
    RTCORBA::StreamControlProtocolProperties::_narrow (from);

  to.send_buffer_size_ = protocol_properties->send_buffer_size ();
  to.recv_buffer_size_ = protocol_properties->recv_buffer_size ();
  to.keep_alive_ = protocol_properties->keep_alive ();
  to.dont_route_ = protocol_properties->dont_route ();
  to.no_delay_ = protocol_properties->no_delay ();
  to.enable_network_priority_ = protocol_properties->enable_network_priority ();
}

void
TAO_RT_Protocols_Hooks::server_protocol_properties_at_orb_level (
  TAO_SCIOP_Protocol_Properties &to)
{
  RTCORBA::ProtocolProperties_var from =
    this->server_protocol_properties_at_orb_level (TAO_TAG_SCIOP_PROFILE);

  if (!CORBA::is_nil (from.in ()))
    this->extract_protocol_properties (to, from.in ());
}

void
TAO_RT_Protocols_Hooks::client_protocol_properties_at_orb_level (
  TAO_SCIOP_Protocol_Properties &to)
{
  RTCORBA::ProtocolProperties_var from =
    this->client_protocol_properties_at_orb_level (TAO_TAG_SCIOP_PROFILE);

  if (!CORBA::is_nil (from.in ()))
    this->extract_protocol_properties (to, from.in ());
}

CORBA::Boolean
TAO_RT_Protocols_Hooks::set_network_priority (
  IOP::ProfileId protocol_tag,
  RTCORBA::ProtocolProperties_ptr protocol_properties)
{
  if (CORBA::is_nil (protocol_properties))
      return false;

  if (protocol_tag == IOP::TAG_INTERNET_IOP)
    {
      RTCORBA::TCPProtocolProperties_var tcp_protocol_properties =
        RTCORBA::TCPProtocolProperties::_narrow (protocol_properties);

      return tcp_protocol_properties->enable_network_priority ();
    }

  if (protocol_tag == TAO_TAG_DIOP_PROFILE)
    {
      RTCORBA::UserDatagramProtocolProperties_var udp_protocol_properties =
        RTCORBA::UserDatagramProtocolProperties::_narrow (protocol_properties);

      return udp_protocol_properties->enable_network_priority ();
    }

  if (protocol_tag == TAO_TAG_SCIOP_PROFILE)
    {
      RTCORBA::StreamControlProtocolProperties_var sctp_protocol_properties =
        RTCORBA::StreamControlProtocolProperties::_narrow (protocol_properties);

      return sctp_protocol_properties->enable_network_priority ();
    }

  return false;
}

CORBA::Boolean
TAO_RT_Protocols_Hooks::set_client_network_priority (IOP::ProfileId protocol_tag,
                                                     TAO_Stub *stub)
{
  if (protocol_tag != IOP::TAG_INTERNET_IOP &&
      protocol_tag != TAO_TAG_DIOP_PROFILE &&
      protocol_tag != TAO_TAG_SCIOP_PROFILE)
    return false;

  RTCORBA::ProtocolProperties_var protocol_properties =
    this->client_protocol_properties_at_object_level (protocol_tag, stub);

  return this->set_network_priority (protocol_tag, protocol_properties.in ());
}

CORBA::Boolean
TAO_RT_Protocols_Hooks::set_server_network_priority (IOP::ProfileId protocol_tag,
                                                     CORBA::Policy *policy)
{
  if (protocol_tag != IOP::TAG_INTERNET_IOP &&
      protocol_tag != TAO_TAG_DIOP_PROFILE &&
      protocol_tag != TAO_TAG_SCIOP_PROFILE)
    return false;

  RTCORBA::ProtocolProperties_var protocol_properties =
    this->server_protocol_properties (protocol_tag, policy);

  return this->set_network_priority (protocol_tag, protocol_properties.in ());
}

CORBA::Long
TAO_RT_Protocols_Hooks::get_dscp_codepoint (void)
{
  if (TAO_debug_level)
    TAOLIB_DEBUG ((LM_DEBUG,
                "TAO_RT_Protocols_Hooks::get_dscp_codepoint\n"));

  CORBA::Long codepoint = 0;

  try
    {
      // Make several invocation, changing the priority of this thread
      // for each.

      RTCORBA::NetworkPriorityMapping *pm =
        this->network_mapping_manager_->mapping ();

      CORBA::Short const priority = this->current_->the_priority ();

      if (pm->to_network (priority, codepoint) == 0)
        {
          if (TAO_debug_level > 0)
            {
              TAOLIB_ERROR ((LM_ERROR,
                          "Cannot convert corba priority %d "
                          "to network priority\n",
                          priority));
            }

          return -1;
        }
    }
  catch (const ::CORBA::Exception& ex)
    {
      if (TAO_debug_level > 0)
        {
          ex._tao_print_exception (
            "TAO_RT_Protocols_Hooks::get_dscp_codepoint");
        }

      return -1;
    }

  return codepoint;
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
    static_cast<TAO_PriorityModelPolicy *> (model_policy_ptr.in ());

  if (priority_model_policy->get_priority_model () == RTCORBA::CLIENT_PROPAGATED)
    {
      is_client_propagated = true;
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
    bool &in_range
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

          in_range = true;
          break;
        }
    }
}

int
TAO_RT_Protocols_Hooks::get_thread_CORBA_priority (CORBA::Short &priority)
{
  priority = TAO_TSS_Resources::instance ()->rtcorba_current_priority_;

  return (priority == -1 ? -1 : 0);
}

int
TAO_RT_Protocols_Hooks::get_thread_native_priority (
    CORBA::Short &native_priority)
{
  ACE_hthread_t current;
  ACE_Thread::self (current);

  int priority;

  if (ACE_Thread::getprio (current, priority) == -1)
    {
      TAOLIB_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - ")
                  ACE_TEXT ("RT_Protocols_Hooks::get_thread_native_priority: ")
                  ACE_TEXT (" ACE_Thread::get_prio failed\n")));

      return -1;
    }

  native_priority = CORBA::Short (priority);
  return 0;
}

int
TAO_RT_Protocols_Hooks::get_thread_CORBA_and_native_priority (
    CORBA::Short &priority,
    CORBA::Short &native_priority)
{
  if (this->get_thread_native_priority (native_priority) == -1)
    {
      return -1;
    }

  if (this->get_thread_CORBA_priority (priority) == -1)
    {
      // This thread had an unset CORBA priority - as this method
      // should only be used to get and restore the state of play before and
      // after invocations that might be OK for all I know. Or care frankly.
      priority = -1;
    }

  return 0;
}

int
TAO_RT_Protocols_Hooks::get_thread_implicit_CORBA_priority (CORBA::Short &priority)
{
  TAO_Priority_Mapping *pm =
    this->mapping_manager_.in ()->mapping ();

  CORBA::Short native_priority = 0;
  if (this->get_thread_native_priority (native_priority) == 0 &&
      pm->to_CORBA (native_priority, priority))
    {
      return 0;
    }

  return -1;
}

int
TAO_RT_Protocols_Hooks::restore_thread_CORBA_and_native_priority (
    CORBA::Short priority,
    CORBA::Short native_priority
  )
{
  // Only used for restoration of values that were previously set /
  // checked when so no need to re-apply the mapping.
  int result = this->set_thread_native_priority (native_priority);

  if (result == 0)
    TAO_TSS_Resources::instance ()->rtcorba_current_priority_ = priority;

  return result;
}

int
TAO_RT_Protocols_Hooks::set_thread_CORBA_priority (CORBA::Short priority)
{
  TAO_Priority_Mapping *priority_mapping =
    this->mapping_manager_.in ()->mapping ();

  CORBA::Short native_priority;

  if (priority_mapping->to_native (priority, native_priority) == 0)
    {
      return -1;
    }

  return this->restore_thread_CORBA_and_native_priority (priority,
                                                         native_priority);
}

int
TAO_RT_Protocols_Hooks::set_thread_native_priority (
  CORBA::Short native_priority)
{
  ACE_hthread_t current;
  ACE_Thread::self (current);

  if (ACE_Thread::setprio (current, native_priority) == -1)
    {
      TAOLIB_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N,%l) Error setting thread ")
                         ACE_TEXT ("priority to %d, errno %d %m\n"),
                         native_priority,
                         ACE_ERRNO_GET),
                       -1);
    }

  return 0;
}


ACE_STATIC_SVC_DEFINE (TAO_RT_Protocols_Hooks,
                       ACE_TEXT ("RT_Protocols_Hooks"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_RT_Protocols_Hooks),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_RTCORBA, TAO_RT_Protocols_Hooks)

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
