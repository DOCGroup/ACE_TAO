#include "tao/RTCORBA/RT_Protocols_Properties_Protocols_Hooks.h"

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
#include "tao/Client_Network_Priority_Policy.h"

#include "ace/Dynamic_Service.h"
#include "ace/OS_NS_string.h"

ACE_RCSID (RTCORBA,
           RT_Protocols_Properties_Protocols_Hooks,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_RT_Protocols_Properties_Protocols_Hooks::
TAO_RT_Protocols_Properties_Protocols_Hooks (void)
  : orb_core_ (0)
  , current_ ()
{
}


TAO_RT_Protocols_Properties_Protocols_Hooks::
~TAO_RT_Protocols_Properties_Protocols_Hooks (void)
{
}

void
TAO_RT_Protocols_Properties_Protocols_Hooks::init_hooks (TAO_ORB_Core *orb_core
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
TAO_RT_Protocols_Properties_Protocols_Hooks::server_protocol_properties (
  IOP::ProfileId protocol_tag, CORBA::Policy_ptr policy ACE_ENV_ARG_DECL)
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
TAO_RT_Protocols_Properties_Protocols_Hooks::client_protocol_properties (
  IOP::ProfileId protocol_tag, CORBA::Policy_ptr policy ACE_ENV_ARG_DECL)
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
TAO_RT_Protocols_Properties_Protocols_Hooks::
server_protocol_properties_at_orb_level (IOP::ProfileId protocol_tag
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
TAO_RT_Protocols_Properties_Protocols_Hooks::
client_protocol_properties_at_orb_level (IOP::ProfileId protocol_tag
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
TAO_RT_Protocols_Properties_Protocols_Hooks::
client_protocol_properties_at_object_level (IOP::ProfileId protocol_tag,
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
TAO_RT_Protocols_Properties_Protocols_Hooks::extract_protocol_properties (
  TAO_IIOP_Protocol_Properties &to,
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
TAO_RT_Protocols_Properties_Protocols_Hooks::
server_protocol_properties_at_orb_level (TAO_IIOP_Protocol_Properties &to
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
TAO_RT_Protocols_Properties_Protocols_Hooks::
client_protocol_properties_at_orb_level (TAO_IIOP_Protocol_Properties &to
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
TAO_RT_Protocols_Properties_Protocols_Hooks::
extract_protocol_properties (TAO_UIOP_Protocol_Properties &to,
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
TAO_RT_Protocols_Properties_Protocols_Hooks::
server_protocol_properties_at_orb_level (TAO_UIOP_Protocol_Properties &to
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
TAO_RT_Protocols_Properties_Protocols_Hooks::
client_protocol_properties_at_orb_level (TAO_UIOP_Protocol_Properties &to
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
TAO_RT_Protocols_Properties_Protocols_Hooks::extract_protocol_properties (
  TAO_SHMIOP_Protocol_Properties &to,
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
TAO_RT_Protocols_Properties_Protocols_Hooks::
server_protocol_properties_at_orb_level (TAO_SHMIOP_Protocol_Properties &to
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
TAO_RT_Protocols_Properties_Protocols_Hooks::
client_protocol_properties_at_orb_level (TAO_SHMIOP_Protocol_Properties &to
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
TAO_RT_Protocols_Properties_Protocols_Hooks::extract_protocol_properties (
  TAO_DIOP_Protocol_Properties &to,
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
TAO_RT_Protocols_Properties_Protocols_Hooks::
server_protocol_properties_at_orb_level (TAO_DIOP_Protocol_Properties &to
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
TAO_RT_Protocols_Properties_Protocols_Hooks::
client_protocol_properties_at_orb_level (TAO_DIOP_Protocol_Properties &to
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
TAO_RT_Protocols_Properties_Protocols_Hooks::extract_protocol_properties (
  TAO_SCIOP_Protocol_Properties &to,
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
TAO_RT_Protocols_Properties_Protocols_Hooks::
server_protocol_properties_at_orb_level (TAO_SCIOP_Protocol_Properties &to
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
TAO_RT_Protocols_Properties_Protocols_Hooks::
client_protocol_properties_at_orb_level (TAO_SCIOP_Protocol_Properties &to
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

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DEFINE (TAO_RT_Protocols_Properties_Protocols_Hooks,
                       ACE_TEXT ("RT_Protocols_Properties_Protocols_Hooks"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (
                          TAO_RT_Protocols_Properties_Protocols_Hooks),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_RTCORBA, TAO_RT_Protocols_Properties_Protocols_Hooks)

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
