// $Id$

#include "tao/RT_Policy_i.h"

#include "tao/ORB_Core.h"

#if ! defined (__ACE_INLINE__)
#include "tao/RT_Policy_i.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(TAO, RT_Policy_i, "$Id$")

#if (TAO_HAS_RT_CORBA == 1)

TAO_PriorityModelPolicy::TAO_PriorityModelPolicy
(RTCORBA::PriorityModel priority_model,
 RTCORBA::Priority server_priority)
  : priority_model_ (priority_model),
    server_priority_ (server_priority)
{
}

TAO_PriorityModelPolicy::TAO_PriorityModelPolicy (const
                                                  TAO_PriorityModelPolicy &rhs)
  : RTCORBA::PriorityModelPolicy (),
    TAO_Local_RefCounted_Object (),
    priority_model_ (rhs.priority_model_),
    server_priority_ (rhs.server_priority_)
{
}

TAO_PriorityModelPolicy::~TAO_PriorityModelPolicy (void)
{
}

RTCORBA::PriorityModel
TAO_PriorityModelPolicy::priority_model (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->priority_model_;
}

RTCORBA::Priority
TAO_PriorityModelPolicy::server_priority (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->server_priority_;
}

CORBA::PolicyType
TAO_PriorityModelPolicy::policy_type (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return RTCORBA::PRIORITY_MODEL_POLICY_TYPE;
}

CORBA::Policy_ptr
TAO_PriorityModelPolicy::copy (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_PriorityModelPolicy* tmp;
  ACE_NEW_THROW_EX (tmp,
                    TAO_PriorityModelPolicy (*this),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return tmp;
}

void TAO_PriorityModelPolicy::destroy (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

TAO_PriorityModelPolicy::TAO_PriorityModelPolicy (void)
  : priority_model_ (RTCORBA::SERVER_DECLARED),
    server_priority_ (0)
{
}

CORBA::Boolean
TAO_PriorityModelPolicy::_tao_encode (TAO_OutputCDR &out_cdr)
{
  // Note: the fields are encoded according to
  // the order specified in the RTCORBA 1.0 spec (ptc/99-05-03)
  // section 4.7.3.

  return ((out_cdr << priority_model_) && (out_cdr << server_priority_));
}

CORBA::Boolean
TAO_PriorityModelPolicy::_tao_decode (TAO_InputCDR &in_cdr)
{
  return ((in_cdr >> priority_model_) && (in_cdr >> server_priority_));
}

// ****************************************************************

TAO_ThreadpoolPolicy::TAO_ThreadpoolPolicy (RTCORBA::ThreadpoolId id)
  : id_ (id)
{
}

TAO_ThreadpoolPolicy::TAO_ThreadpoolPolicy (const TAO_ThreadpoolPolicy
                                            &rhs)
  : RTCORBA::ThreadpoolPolicy (),
    TAO_Local_RefCounted_Object (),
    id_ (rhs.id_)
{
}

TAO_ThreadpoolPolicy::~TAO_ThreadpoolPolicy (void)
{
}

RTCORBA::ThreadpoolId
TAO_ThreadpoolPolicy::threadpool (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->id_;
}

CORBA::PolicyType
TAO_ThreadpoolPolicy::policy_type (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return RTCORBA::THREADPOOL_POLICY_TYPE;
}

CORBA::Policy_ptr
TAO_ThreadpoolPolicy::copy (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_ThreadpoolPolicy* tmp;
  ACE_NEW_THROW_EX (tmp,
                    TAO_ThreadpoolPolicy (*this),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return tmp;
}

void TAO_ThreadpoolPolicy::destroy (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

// ****************************************************************

TAO_PrivateConnectionPolicy::TAO_PrivateConnectionPolicy (void)
{
}

TAO_PrivateConnectionPolicy::TAO_PrivateConnectionPolicy (const
                                                           TAO_PrivateConnectionPolicy &)
  : RTCORBA::PrivateConnectionPolicy (),
    TAO_Local_RefCounted_Object ()
{
}

TAO_PrivateConnectionPolicy::~TAO_PrivateConnectionPolicy (void)
{
}

CORBA::PolicyType
TAO_PrivateConnectionPolicy::policy_type (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return RTCORBA::PRIVATE_CONNECTION_POLICY_TYPE;
}

CORBA::Policy_ptr
TAO_PrivateConnectionPolicy::copy (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_PrivateConnectionPolicy* tmp;
  ACE_NEW_THROW_EX (tmp,
                    TAO_PrivateConnectionPolicy (*this),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return tmp;
}

void
TAO_PrivateConnectionPolicy::destroy (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

// ****************************************************************

TAO_PriorityBandedConnectionPolicy::TAO_PriorityBandedConnectionPolicy
(const RTCORBA::PriorityBands &bands)
  : RTCORBA::PriorityBandedConnectionPolicy (),
    priority_bands_ (bands)
{
}

TAO_PriorityBandedConnectionPolicy::TAO_PriorityBandedConnectionPolicy
(const TAO_PriorityBandedConnectionPolicy &rhs)
  : RTCORBA::PriorityBandedConnectionPolicy (),
    TAO_Local_RefCounted_Object (),
    priority_bands_ (rhs.priority_bands_)
{
}

TAO_PriorityBandedConnectionPolicy::~TAO_PriorityBandedConnectionPolicy (void)
{
}

RTCORBA::PriorityBands *
TAO_PriorityBandedConnectionPolicy::priority_bands (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  RTCORBA::PriorityBands *tmp;
  ACE_NEW_THROW_EX (tmp,
                    RTCORBA::PriorityBands (this->priority_bands_),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (0);

  return tmp;
}

CORBA::PolicyType
TAO_PriorityBandedConnectionPolicy::policy_type (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE;
}

CORBA::Policy_ptr
TAO_PriorityBandedConnectionPolicy::copy (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_PriorityBandedConnectionPolicy *tmp;
  ACE_NEW_THROW_EX (tmp,
                    TAO_PriorityBandedConnectionPolicy (*this),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return tmp;
}

void TAO_PriorityBandedConnectionPolicy::destroy (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

///////////////////////////////////////////////////////
// Method for serialization support.

TAO_PriorityBandedConnectionPolicy::TAO_PriorityBandedConnectionPolicy
(void)
{
}

CORBA::Boolean
TAO_PriorityBandedConnectionPolicy::_tao_encode (TAO_OutputCDR &out_cdr)
{
  return out_cdr << priority_bands_;
}

CORBA::Boolean
TAO_PriorityBandedConnectionPolicy::_tao_decode (TAO_InputCDR &in_cdr)
{
  return in_cdr >> priority_bands_;
}

// ****************************************************************

TAO_TCP_Properties::TAO_TCP_Properties (CORBA::Long send_buffer_size,
                                        CORBA::Long recv_buffer_size,
                                        CORBA::Boolean keep_alive,
                                        CORBA::Boolean dont_route,
                                        CORBA::Boolean no_delay)
  : send_buffer_size_ (send_buffer_size),
    recv_buffer_size_ (recv_buffer_size),
    keep_alive_ (keep_alive),
    dont_route_ (dont_route),
    no_delay_ (no_delay)
{
}

TAO_TCP_Properties::~TAO_TCP_Properties (void)
{
}

CORBA::Long
TAO_TCP_Properties::send_buffer_size (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->send_buffer_size_;
}

void
TAO_TCP_Properties::send_buffer_size (CORBA::Long send_buffer_size,
                                      CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->send_buffer_size_ = send_buffer_size;
}

CORBA::Long
TAO_TCP_Properties::recv_buffer_size (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->recv_buffer_size_;
}

void
TAO_TCP_Properties::recv_buffer_size (CORBA::Long recv_buffer_size,
                                      CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->recv_buffer_size_ = recv_buffer_size;
}

CORBA::Boolean
TAO_TCP_Properties::keep_alive (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->keep_alive_;
}

void
TAO_TCP_Properties::keep_alive (CORBA::Boolean keep_alive,
                                CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->keep_alive_ = keep_alive;
}

CORBA::Boolean
TAO_TCP_Properties::dont_route (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->dont_route_;
}

void
TAO_TCP_Properties::dont_route (CORBA::Boolean dont_route,
                                CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->dont_route_ = dont_route;
}

CORBA::Boolean TAO_TCP_Properties::no_delay (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->no_delay_;
}

void
TAO_TCP_Properties::no_delay (CORBA::Boolean no_delay,
                              CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->no_delay_ = no_delay;
}

///////////////////////////////////////////////////////
// Method for serialization support.

CORBA::Boolean
TAO_TCP_Properties::_tao_encode (TAO_OutputCDR & out_cdr)
{
  return ((out_cdr << this->send_buffer_size_)
          &&
          (out_cdr << this->recv_buffer_size_)
          &&
          (out_cdr.write_boolean (this->keep_alive_))
          &&
          (out_cdr.write_boolean (this->dont_route_))
          &&
          (out_cdr.write_boolean (this->no_delay_)));
}

CORBA::Boolean
TAO_TCP_Properties::_tao_decode (TAO_InputCDR &in_cdr)
{
  return ((in_cdr >> this->send_buffer_size_)
          &&
          (in_cdr >> this->recv_buffer_size_)
          &&
          (in_cdr.read_boolean (this->keep_alive_))
          &&
          (in_cdr.read_boolean (this->dont_route_))
          &&
          (in_cdr.read_boolean (this->no_delay_)));
}

// ****************************************************************

TAO_Unix_Domain_Properties::TAO_Unix_Domain_Properties
(CORBA::Long send_buffer_size,
 CORBA::Long recv_buffer_size)
  : send_buffer_size_ (send_buffer_size),
    recv_buffer_size_ (recv_buffer_size)
{
}

TAO_Unix_Domain_Properties::~TAO_Unix_Domain_Properties (void)
{
}

CORBA::Long
TAO_Unix_Domain_Properties::send_buffer_size (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->send_buffer_size_;
}

void
TAO_Unix_Domain_Properties::send_buffer_size (CORBA::Long send_buffer_size,
                                              CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->send_buffer_size_ = send_buffer_size;
}

CORBA::Long
TAO_Unix_Domain_Properties::recv_buffer_size (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->recv_buffer_size_;
}

void
TAO_Unix_Domain_Properties::recv_buffer_size (CORBA::Long recv_buffer_size,
                                              CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->recv_buffer_size_ = recv_buffer_size;
}


CORBA::Boolean
TAO_Unix_Domain_Properties::_tao_encode (TAO_OutputCDR &out_cdr)
{
  return ((out_cdr << this->send_buffer_size_)
          && (out_cdr << this->recv_buffer_size_));
}

CORBA::Boolean
TAO_Unix_Domain_Properties::_tao_decode (TAO_InputCDR &in_cdr)
{
  return ((in_cdr >> this->send_buffer_size_)
          && (in_cdr >> this->recv_buffer_size_));
}

// ****************************************************************

TAO_SMEM_Properties::TAO_SMEM_Properties (void)
  : preallocate_buffer_size_ (0),
    mmap_filename_ (),
    mmap_lockname_ ()
{
}

TAO_SMEM_Properties::~TAO_SMEM_Properties (void)
{
}


CORBA::Long
TAO_SMEM_Properties::preallocate_buffer_size (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->preallocate_buffer_size_;
}

void
TAO_SMEM_Properties::preallocate_buffer_size (CORBA::Long preallocate_buffer_size,
                                              CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->preallocate_buffer_size_ = preallocate_buffer_size;
}

char *
TAO_SMEM_Properties::mmap_filename (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->mmap_filename_.rep ();
}

void
TAO_SMEM_Properties::mmap_filename (const char * mmap_filename,
                                    CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->mmap_filename_.set (mmap_filename);
}

char *
TAO_SMEM_Properties::mmap_lockname (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->mmap_lockname_.rep ();
}

void
TAO_SMEM_Properties::mmap_lockname (const char * mmap_lockname,
                                    CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->mmap_lockname_.set (mmap_lockname);
}

CORBA::Boolean
TAO_SMEM_Properties::_tao_encode (TAO_OutputCDR &out_cdr)
{
  return ((out_cdr << this->preallocate_buffer_size_)
          &&
          (out_cdr << this->mmap_filename_)
          &&
          (out_cdr << this->mmap_lockname_));
}

CORBA::Boolean
TAO_SMEM_Properties::_tao_decode (TAO_InputCDR &in_cdr)
{
  return ((in_cdr >> this->preallocate_buffer_size_)
          &&
          (in_cdr >> this->mmap_filename_)
          &&
          (in_cdr >> this->mmap_lockname_));
}

// ****************************************************************

TAO_ServerProtocolPolicy::TAO_ServerProtocolPolicy (const
                                                    RTCORBA::ProtocolList &protocols)
  : RTCORBA::ServerProtocolPolicy (),
    protocols_ (protocols)
{
}

TAO_ServerProtocolPolicy::TAO_ServerProtocolPolicy (const
                                                    TAO_ServerProtocolPolicy &rhs)
  : RTCORBA::ServerProtocolPolicy (),
    TAO_Local_RefCounted_Object (),
    protocols_ (rhs.protocols_)
{
}

TAO_ServerProtocolPolicy::~TAO_ServerProtocolPolicy (void)
{
}

RTCORBA::ProtocolList *
TAO_ServerProtocolPolicy::protocols (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  RTCORBA::ProtocolList *tmp;
  ACE_NEW_THROW_EX (tmp,
                    RTCORBA::ProtocolList (this->protocols_),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (0);

  return tmp;
}

CORBA::PolicyType
TAO_ServerProtocolPolicy::policy_type (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return RTCORBA::SERVER_PROTOCOL_POLICY_TYPE;
}

CORBA::Policy_ptr
TAO_ServerProtocolPolicy::copy (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_ServerProtocolPolicy* tmp;
  ACE_NEW_THROW_EX (tmp,
                    TAO_ServerProtocolPolicy (*this),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return tmp;
}

void
TAO_ServerProtocolPolicy::destroy (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

int
TAO_ServerProtocolPolicy::hook (TAO_ORB_Core *orb_core,
                                int &send_buffer_size,
                                int &recv_buffer_size,
                                int &no_delay,
                                const char *protocol_type)
{
  RTCORBA::ProtocolProperties_var properties =
    RTCORBA::ProtocolProperties::_nil ();

  // ServerProtocolProperties policy controls protocols configuration.
  // Look for protocol properties in the effective ServerProtocolPolicy.
  CORBA::Policy_var policy =
    orb_core->server_protocol ();

  ACE_DECLARE_NEW_CORBA_ENV;

  RTCORBA::ServerProtocolPolicy_var server_protocols_policy;
  TAO_ServerProtocolPolicy *server_protocols = 0;

  if (!CORBA::is_nil (policy.in ()))
  {
    server_protocols_policy =
      RTCORBA::ServerProtocolPolicy::_narrow (policy.in (),
                                              ACE_TRY_ENV);
    ACE_CHECK_RETURN (-1);

    server_protocols =
      ACE_static_cast (TAO_ServerProtocolPolicy *,
                       server_protocols_policy.in ());

    if (server_protocols != 0)
      {
        RTCORBA::ProtocolList & protocols = server_protocols->protocols_rep ();

        // Find protocol properties.
        for (CORBA::ULong j = 0; j < protocols.length (); ++j)
          {
            if (protocols[j].protocol_type == TAO_TAG_IIOP_PROFILE)
              {
                properties =
                  RTCORBA::ProtocolProperties::_narrow (
                     protocols[j].transport_protocol_properties.in (),
                     ACE_TRY_ENV);
                ACE_CHECK_RETURN (-1);

                if (ACE_OS::strcmp (protocol_type,
                                    "iiop") == 0)
                  break;
              }
            else if (protocols[j].protocol_type == TAO_TAG_UIOP_PROFILE)
              {
                properties =
                  RTCORBA::ProtocolProperties::_narrow (
                     protocols[j].transport_protocol_properties.in (),
                     ACE_TRY_ENV);
                ACE_CHECK_RETURN (-1);

                if (ACE_OS::strcmp (protocol_type, "uiop") == 0)
                  break;
              }
          }
      }
  }

  if (CORBA::is_nil (properties.in ()))
    {
      // TCP/UIOP/SHMIOP Properties were not specified in the effective policy.
      // We must use orb defaults.
      policy = orb_core->default_server_protocol ();

      if (!CORBA::is_nil (policy.in ()))
        {
          server_protocols_policy =
            RTCORBA::ServerProtocolPolicy::_narrow (policy.in (),
                                                    ACE_TRY_ENV);
          ACE_CHECK_RETURN (-1);

          server_protocols =
            ACE_static_cast (TAO_ServerProtocolPolicy *,
                             server_protocols_policy.in ());

          if (!CORBA::is_nil (server_protocols))
            {
              // Find protocol properties for IIOP.
              RTCORBA::ProtocolList & protocols =
                server_protocols->protocols_rep ();

              for (CORBA::ULong j = 0; j < protocols.length (); ++j)
                {
                  if (protocols[j].protocol_type == TAO_TAG_IIOP_PROFILE)
                    {
                      properties =
                        RTCORBA::ProtocolProperties::_narrow (
                           protocols[j].transport_protocol_properties.in (),
                           ACE_TRY_ENV);
                      ACE_CHECK_RETURN (-1);

                      if (ACE_OS::strcmp (protocol_type, "iiop") == 0)
                        break;
                    }
                  else if (protocols[j].protocol_type == TAO_TAG_UIOP_PROFILE)
                    {
                      properties =
                        RTCORBA::ProtocolProperties::_narrow (
                           protocols[j].transport_protocol_properties.in (),
                           ACE_TRY_ENV);
                      ACE_CHECK_RETURN (-1);

                      if (ACE_OS::strcmp (protocol_type, "uiop") == 0)
                        break;
                    }
                }
            }
        }
    }

  if (ACE_OS::strcmp (protocol_type, "iiop") == 0)
    {
      RTCORBA::TCPProtocolProperties_var tcp_properties =
        RTCORBA::TCPProtocolProperties::_narrow (properties.in (),
                                                 ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);

      send_buffer_size = tcp_properties->send_buffer_size ();
      recv_buffer_size = tcp_properties->recv_buffer_size ();
      no_delay = tcp_properties->no_delay ();
    }

  if (ACE_OS::strcmp (protocol_type, "uiop") == 0)
    {
      RTCORBA::UnixDomainProtocolProperties_var uiop_properties =
        RTCORBA::UnixDomainProtocolProperties::_narrow (properties.in (),
                                                        ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);

      if (!CORBA::is_nil (uiop_properties.in ()))
        {
          // Extract and locally store properties of interest.
          send_buffer_size =
            uiop_properties->send_buffer_size ();
          recv_buffer_size =
            uiop_properties->recv_buffer_size ();
        }
    }

  return 0;
}

// ****************************************************************

TAO_ClientProtocolPolicy::TAO_ClientProtocolPolicy (const
                                                    RTCORBA::ProtocolList &protocols)
  : RTCORBA::ClientProtocolPolicy (),
    protocols_ (protocols)
{
}

TAO_ClientProtocolPolicy::TAO_ClientProtocolPolicy (const
                                                    TAO_ClientProtocolPolicy &rhs)
  : RTCORBA::ClientProtocolPolicy (),
    TAO_Local_RefCounted_Object (),
    protocols_ (rhs.protocols_)
{
}

TAO_ClientProtocolPolicy::~TAO_ClientProtocolPolicy ()
{
}

RTCORBA::ProtocolList *
TAO_ClientProtocolPolicy::protocols (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  RTCORBA::ProtocolList *tmp;
  ACE_NEW_THROW_EX (tmp,
                    RTCORBA::ProtocolList (this->protocols_),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (0);

  return tmp;
}

CORBA::PolicyType
TAO_ClientProtocolPolicy::policy_type (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return RTCORBA::CLIENT_PROTOCOL_POLICY_TYPE;
}

CORBA::Policy_ptr
TAO_ClientProtocolPolicy::copy (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_ClientProtocolPolicy* tmp;
  ACE_NEW_THROW_EX (tmp,
                    TAO_ClientProtocolPolicy (*this),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return tmp;
}

void
TAO_ClientProtocolPolicy::destroy (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

int
TAO_ClientProtocolPolicy::hook (TAO_ORB_Core *orb_core,
                                int &send_buffer_size,
                                int &recv_buffer_size,
                                int &no_delay,
                                const char *protocol_type)
{
  RTCORBA::ProtocolProperties_var properties =
    RTCORBA::ProtocolProperties::_nil ();

  // Check ORB-level override for properties.
  CORBA::Policy_var policy =
    orb_core->policy_manager ()->client_protocol ();

  ACE_DECLARE_NEW_CORBA_ENV;

  RTCORBA::ClientProtocolPolicy_var client_protocols_policy;
  TAO_ClientProtocolPolicy *client_protocols = 0;

  if (!CORBA::is_nil (policy.in ()))
  {
    client_protocols_policy =
      RTCORBA::ClientProtocolPolicy::_narrow (policy.in (),
                                              ACE_TRY_ENV);
    ACE_CHECK_RETURN (-1);

    client_protocols =
      ACE_static_cast (TAO_ClientProtocolPolicy *,
                       client_protocols_policy.in ());

    if (client_protocols != 0)
      {
        //TAO_ClientProtocolPolicy
        RTCORBA::ProtocolList & protocols =
          client_protocols->protocols_rep ();

        for (CORBA::ULong j = 0; j < protocols.length (); ++j)
        {
          if (protocols[j].protocol_type == TAO_TAG_IIOP_PROFILE)
            {
              properties =
                RTCORBA::ProtocolProperties::_narrow (
                  protocols[j].transport_protocol_properties.in (),
                  ACE_TRY_ENV);
              ACE_CHECK_RETURN (-1);

              if (ACE_OS::strcmp (protocol_type, "iiop") == 0)
                  break;
            }
          else if (protocols[j].protocol_type == TAO_TAG_UIOP_PROFILE)
            {
              properties =
                RTCORBA::ProtocolProperties::_narrow (
                  protocols[j].transport_protocol_properties.in (),
                  ACE_TRY_ENV);
              ACE_CHECK_RETURN (-1);

              if (ACE_OS::strcmp (protocol_type, "uiop") == 0)
                break;
            }
        }
      }
  }

  if (CORBA::is_nil (properties.in ()))
  {
    // No tcp/uiop properties in ORB-level override.  Use ORB defaults.
    // Orb defaults should never be null - they were initialized by
    // the ORB_Core.
    policy =
      orb_core->default_client_protocol ();

    if (!CORBA::is_nil (policy.in ()))
      {
        client_protocols_policy =
          RTCORBA::ClientProtocolPolicy::_narrow (policy.in (),
                                                  ACE_TRY_ENV);
        ACE_CHECK_RETURN (-1);

        client_protocols =
          ACE_dynamic_cast (TAO_ClientProtocolPolicy *,
                           client_protocols_policy.in ());

        if (client_protocols != 0)
          {
            RTCORBA::ProtocolList & protocols =
              client_protocols->protocols_rep ();

            for (CORBA::ULong j = 0; j < protocols.length (); ++j)
              {
                if (protocols[j].protocol_type == TAO_TAG_IIOP_PROFILE)
                  {
                    properties =
                      RTCORBA::ProtocolProperties::_narrow (
                         protocols[j].transport_protocol_properties.in (),
                         ACE_TRY_ENV);
                    ACE_CHECK_RETURN (-1);

                    if (ACE_OS::strcmp (protocol_type, "iiop") == 0)
                      break;
                  }
                else if (protocols[j].protocol_type == TAO_TAG_UIOP_PROFILE)
                  {
                    properties =
                      RTCORBA::ProtocolProperties::_narrow (
                         protocols[j].transport_protocol_properties.in (),
                         ACE_TRY_ENV);
                    ACE_CHECK_RETURN (-1);

                    if (ACE_OS::strcmp (protocol_type, "uiop") == 0)
                      break;
                  }
              }
          }
      }
  }

  if (ACE_OS::strcmp (protocol_type, "iiop") == 0)
    {
      RTCORBA::TCPProtocolProperties_var tcp_properties =
        RTCORBA::TCPProtocolProperties::_narrow (properties.in (),
                                                 ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);

      send_buffer_size = tcp_properties->send_buffer_size ();
      recv_buffer_size = tcp_properties->recv_buffer_size ();
      no_delay = tcp_properties->no_delay ();
    }

  if (ACE_OS::strcmp (protocol_type, "uiop") == 0)
    {
      RTCORBA::UnixDomainProtocolProperties_var uiop_properties =
        RTCORBA::UnixDomainProtocolProperties::_narrow (properties.in (),
                                                        ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);

      if (!CORBA::is_nil (uiop_properties.in ()))
        {
          // Extract and locally store properties of interest.
          send_buffer_size =
            uiop_properties->send_buffer_size ();
          recv_buffer_size =
            uiop_properties->recv_buffer_size ();
        }
    }
  return 0;
}

///////////////////////////////////////////////////////
// Method for serialization support.

TAO_ClientProtocolPolicy::TAO_ClientProtocolPolicy (void)
{
}


CORBA::Boolean
TAO_ClientProtocolPolicy::_tao_encode (TAO_OutputCDR &out_cdr)
{
  CORBA::Boolean is_write_ok = out_cdr << this->protocols_.length ();

  for (CORBA::ULong i = 0;
       (i < this->protocols_.length ()) && is_write_ok;
       i++)
    {
      is_write_ok =
        (out_cdr << this->protocols_[i].protocol_type)
        &&
        this->protocols_[i].orb_protocol_properties->_tao_encode (out_cdr)
        &&
        this->protocols_[i].transport_protocol_properties->_tao_encode (out_cdr);
    }

  return is_write_ok;
}

CORBA::Boolean
TAO_ClientProtocolPolicy::_tao_decode (TAO_InputCDR &in_cdr)
{
  CORBA::ULong length;
  CORBA::Boolean is_read_ok = in_cdr >> length;

  this->protocols_.length (length);

  for (CORBA::ULong i = 0; (i < length) && is_read_ok; i++)
    {
      is_read_ok = in_cdr >> this->protocols_[i].protocol_type;

      this->protocols_[i].orb_protocol_properties =
        TAO_Protocol_Properties_Factory::create_orb_protocol_property
        (this->protocols_[i].protocol_type);

      this->protocols_[i].transport_protocol_properties =
        TAO_Protocol_Properties_Factory::create_transport_protocol_property
        (this->protocols_[i].protocol_type);

      if (is_read_ok
          && (this->protocols_[i].orb_protocol_properties.ptr () != 0))
        is_read_ok =
          this->protocols_[i].orb_protocol_properties->_tao_decode (in_cdr);

      if (is_read_ok
          && (this->protocols_[i].transport_protocol_properties.ptr () != 0))
        is_read_ok =
          this->protocols_[i].transport_protocol_properties->_tao_decode (in_cdr);

    }

  return is_read_ok;
}
// ****************************************************************

TAO_GIOP_Properties::TAO_GIOP_Properties (void)
{
}

TAO_GIOP_Properties::~TAO_GIOP_Properties (void)
{
}

CORBA::Boolean
TAO_GIOP_Properties::_tao_encode (TAO_OutputCDR &out_cdr)
{
  ACE_UNUSED_ARG (out_cdr);
  return 1;
}

CORBA::Boolean
TAO_GIOP_Properties::_tao_decode (TAO_InputCDR &in_cdr)
{
  ACE_UNUSED_ARG (in_cdr);
  return 1;
}

// ****************************************************************

RTCORBA::ProtocolProperties*
TAO_Protocol_Properties_Factory::create_transport_protocol_property (IOP::ProfileId id)
{
  RTCORBA::ProtocolProperties* property = 0;

  if (id == IOP::TAG_INTERNET_IOP)

    ACE_NEW_RETURN (property,
                    TAO_TCP_Properties,
                    0);

  else if(id == TAO_TAG_SHMEM_PROFILE)
    ACE_NEW_RETURN (property,
                    TAO_SMEM_Properties,
                    0);

  else if (id == TAO_TAG_UIOP_PROFILE)
    ACE_NEW_RETURN (property,
                    TAO_Unix_Domain_Properties,
                    0);
  return property;
}

RTCORBA::ProtocolProperties*
TAO_Protocol_Properties_Factory::create_orb_protocol_property (IOP::ProfileId id)
{
  RTCORBA::ProtocolProperties* property = 0;

  if (id == IOP::TAG_INTERNET_IOP)
    ACE_NEW_RETURN (property,
                    TAO_GIOP_Properties,
                    0);

  // Right now the only supported ORB protocol is GIOP
  // so we couple this with every protocol property.
  // The else statement is not necessary, but it
  // is here just to make clear that as soon as
  // new ORB protocol are supported other case
  // should be considered.
  else
    ACE_NEW_RETURN (property,
                    TAO_GIOP_Properties,
                    0);
   return property;
}

#endif /* TAO_HAS_RT_CORBA == 1 */
