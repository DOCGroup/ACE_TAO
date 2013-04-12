// $Id$

#include "tao/RTCORBA/RT_Policy_i.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/ORB_Core.h"
#include "tao/Policy_Manager.h"
#include "tao/debug.h"
#include "tao/CDR.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/SystemException.h"

#include "ace/OS_NS_string.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_PriorityModelPolicy::TAO_PriorityModelPolicy (
  RTCORBA::PriorityModel priority_model,
  RTCORBA::Priority server_priority)
  : ::CORBA::Object ()
  , ::CORBA::Policy ()
  , RTCORBA::PriorityModelPolicy ()
  , ::CORBA::LocalObject ()
  , priority_model_ (priority_model)
  , server_priority_ (server_priority)
{
}

TAO_PriorityModelPolicy::TAO_PriorityModelPolicy (const TAO_PriorityModelPolicy &rhs)
  : ::CORBA::Object ()
  , ::CORBA::Policy ()
  , RTCORBA::PriorityModelPolicy ()
  , ::CORBA::LocalObject ()
  , priority_model_ (rhs.priority_model_)
  , server_priority_ (rhs.server_priority_)
{
}

TAO_PriorityModelPolicy::TAO_PriorityModelPolicy (void)
  : priority_model_ (RTCORBA::SERVER_DECLARED),
    server_priority_ (0)
{
}

TAO_PriorityModelPolicy::~TAO_PriorityModelPolicy (void)
{
}

CORBA::Policy_ptr
TAO_PriorityModelPolicy::create (const CORBA::Any &)
{
  /*
   * @@ The following code should be changed once the OMG spec has
   * been fixed such that a RTCORBA::PriorityModelPolicy can be
   * created by using the ORB::create_policy interface.
   */
  throw ::CORBA::PolicyError (CORBA::BAD_POLICY_VALUE);
}

RTCORBA::PriorityModel
TAO_PriorityModelPolicy::get_priority_model (void)
{
  return this->priority_model_;
}

RTCORBA::PriorityModel
TAO_PriorityModelPolicy::priority_model (void)
{
  return this->priority_model_;
}

RTCORBA::Priority
TAO_PriorityModelPolicy::server_priority (void)
{
  return this->server_priority_;
}

CORBA::PolicyType
TAO_PriorityModelPolicy::policy_type (void)
{
  return RTCORBA::PRIORITY_MODEL_POLICY_TYPE;
}

CORBA::Policy_ptr
TAO_PriorityModelPolicy::copy (void)
{
  TAO_PriorityModelPolicy* tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_PriorityModelPolicy (*this),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}

void
TAO_PriorityModelPolicy::destroy (void)
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

TAO_Cached_Policy_Type
TAO_PriorityModelPolicy::_tao_cached_type (void) const
{
  return TAO_CACHED_POLICY_PRIORITY_MODEL;
}

TAO_Policy_Scope
TAO_PriorityModelPolicy::_tao_scope (void) const
{
  // Note that this policy is propogated to the client even though
  // it is not specified here.  The reason for this is that the
  // server priority field is set dynamically depending on the model
  // and the servant's priority.  Therefore, it can't be simply
  // copied to the list of client exposed policies.
  return static_cast<TAO_Policy_Scope> (TAO_POLICY_ORB_SCOPE |
                          TAO_POLICY_POA_SCOPE);
}

// ****************************************************************

TAO_ThreadpoolPolicy::TAO_ThreadpoolPolicy (RTCORBA::ThreadpoolId id)
  : ::CORBA::Object ()
  , ::CORBA::Policy ()
  , RTCORBA::ThreadpoolPolicy ()
  , ::CORBA::LocalObject ()
  , id_ (id)
{
}

TAO_ThreadpoolPolicy::TAO_ThreadpoolPolicy (const TAO_ThreadpoolPolicy &rhs)
  : ::CORBA::Object ()
  , ::CORBA::Policy ()
  , RTCORBA::ThreadpoolPolicy ()
  , ::CORBA::LocalObject ()
  , id_ (rhs.id_)
{
}

TAO_ThreadpoolPolicy::~TAO_ThreadpoolPolicy (void)
{
}

CORBA::Policy_ptr
TAO_ThreadpoolPolicy::create (const CORBA::Any &val)
{
  RTCORBA::ThreadpoolId value;
  if (!(val >>= value))
    throw ::CORBA::PolicyError (CORBA::BAD_POLICY_VALUE);

  TAO_ThreadpoolPolicy *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_ThreadpoolPolicy (value),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}

RTCORBA::ThreadpoolId
TAO_ThreadpoolPolicy::threadpool (void)
{
  return this->id_;
}

CORBA::PolicyType
TAO_ThreadpoolPolicy::policy_type (void)
{
  return RTCORBA::THREADPOOL_POLICY_TYPE;
}

CORBA::Policy_ptr
TAO_ThreadpoolPolicy::copy (void)
{
  TAO_ThreadpoolPolicy* tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_ThreadpoolPolicy (*this),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}

void
TAO_ThreadpoolPolicy::destroy (void)
{
}

TAO_Cached_Policy_Type
TAO_ThreadpoolPolicy::_tao_cached_type (void) const
{
  return TAO_CACHED_POLICY_THREADPOOL;
}

TAO_Policy_Scope
TAO_ThreadpoolPolicy::_tao_scope (void) const
{
  return static_cast<TAO_Policy_Scope> (TAO_POLICY_ORB_SCOPE |
                          TAO_POLICY_POA_SCOPE);
}

// ****************************************************************

TAO_PrivateConnectionPolicy::TAO_PrivateConnectionPolicy (void)
  : ::CORBA::Object ()
  , ::CORBA::Policy ()
  , RTCORBA::PrivateConnectionPolicy ()
  , ::CORBA::LocalObject ()
{
}

TAO_PrivateConnectionPolicy::TAO_PrivateConnectionPolicy (const TAO_PrivateConnectionPolicy &)
  : ::CORBA::Object ()
  , ::CORBA::Policy ()
  , RTCORBA::PrivateConnectionPolicy ()
  , ::CORBA::LocalObject ()
{
}

TAO_PrivateConnectionPolicy::~TAO_PrivateConnectionPolicy (void)
{
}

CORBA::Policy_ptr
TAO_PrivateConnectionPolicy::create (const CORBA::Any &)
{
  TAO_PrivateConnectionPolicy *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_PrivateConnectionPolicy (),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}

CORBA::PolicyType
TAO_PrivateConnectionPolicy::policy_type (void)
{
  return RTCORBA::PRIVATE_CONNECTION_POLICY_TYPE;
}

CORBA::Policy_ptr
TAO_PrivateConnectionPolicy::copy (void)
{
  TAO_PrivateConnectionPolicy* tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_PrivateConnectionPolicy (*this),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}

void
TAO_PrivateConnectionPolicy::destroy (void)
{
}

TAO_Cached_Policy_Type
TAO_PrivateConnectionPolicy::_tao_cached_type (void) const
{
  return TAO_CACHED_POLICY_RT_PRIVATE_CONNECTION;
}

TAO_Policy_Scope
TAO_PrivateConnectionPolicy::_tao_scope (void) const
{
  return static_cast<TAO_Policy_Scope> (TAO_POLICY_OBJECT_SCOPE |
                          TAO_POLICY_THREAD_SCOPE |
                          TAO_POLICY_ORB_SCOPE);
}

// ****************************************************************

TAO_PriorityBandedConnectionPolicy::TAO_PriorityBandedConnectionPolicy (const RTCORBA::PriorityBands &bands)
  : ::CORBA::Object ()
  , ::CORBA::Policy ()
  , RTCORBA::PriorityBandedConnectionPolicy ()
  , ::CORBA::LocalObject ()
  , priority_bands_ (bands)
{
}

TAO_PriorityBandedConnectionPolicy::TAO_PriorityBandedConnectionPolicy (const TAO_PriorityBandedConnectionPolicy &rhs)
  : ::CORBA::Object ()
  , ::CORBA::Policy ()
  , RTCORBA::PriorityBandedConnectionPolicy ()
  , ::CORBA::LocalObject ()
  , priority_bands_ (rhs.priority_bands_)
{
}

TAO_PriorityBandedConnectionPolicy::TAO_PriorityBandedConnectionPolicy (void)
{
}

TAO_PriorityBandedConnectionPolicy::~TAO_PriorityBandedConnectionPolicy (void)
{
}

CORBA::Policy_ptr
TAO_PriorityBandedConnectionPolicy::create (const CORBA::Any &val)
{
  RTCORBA::PriorityBands *value = 0;
  if (!(val >>= value))
    throw ::CORBA::PolicyError (CORBA::BAD_POLICY_VALUE);

  TAO_PriorityBandedConnectionPolicy *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_PriorityBandedConnectionPolicy (*value),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}

RTCORBA::PriorityBands *
TAO_PriorityBandedConnectionPolicy::priority_bands (void)
{
  RTCORBA::PriorityBands *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    RTCORBA::PriorityBands (this->priority_bands_),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}

CORBA::PolicyType
TAO_PriorityBandedConnectionPolicy::policy_type (void)
{
  return RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE;
}

CORBA::Policy_ptr
TAO_PriorityBandedConnectionPolicy::copy (void)
{
  TAO_PriorityBandedConnectionPolicy *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_PriorityBandedConnectionPolicy (*this),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}

void
TAO_PriorityBandedConnectionPolicy::destroy (void)
{
}

CORBA::Boolean
TAO_PriorityBandedConnectionPolicy::_tao_encode (TAO_OutputCDR &out_cdr)
{
  return out_cdr << this->priority_bands_;
}

CORBA::Boolean
TAO_PriorityBandedConnectionPolicy::_tao_decode (TAO_InputCDR &in_cdr)
{
  return in_cdr >> this->priority_bands_;
}

TAO_Cached_Policy_Type
TAO_PriorityBandedConnectionPolicy::_tao_cached_type (void) const
{
  return TAO_CACHED_POLICY_RT_PRIORITY_BANDED_CONNECTION;
}

TAO_Policy_Scope
TAO_PriorityBandedConnectionPolicy::_tao_scope (void) const
{
  return static_cast<TAO_Policy_Scope> (TAO_POLICY_DEFAULT_SCOPE |
                          TAO_POLICY_CLIENT_EXPOSED);
}

RTCORBA::PriorityBands &
TAO_PriorityBandedConnectionPolicy::priority_bands_rep (void)
{
  return this->priority_bands_;
}

// ****************************************************************

TAO_ServerProtocolPolicy::TAO_ServerProtocolPolicy (const RTCORBA::ProtocolList &protocols)
  : ::CORBA::Object ()
  , ::CORBA::Policy ()
  , RTCORBA::ServerProtocolPolicy ()
  , ::CORBA::LocalObject ()
  , protocols_ (protocols)
{
}

TAO_ServerProtocolPolicy::TAO_ServerProtocolPolicy (const TAO_ServerProtocolPolicy &rhs)
  : ::CORBA::Object ()
  , ::CORBA::Policy ()
  , RTCORBA::ServerProtocolPolicy ()
  , ::CORBA::LocalObject ()
  , protocols_ (rhs.protocols_)
{
}

TAO_ServerProtocolPolicy::~TAO_ServerProtocolPolicy (void)
{
}

CORBA::Policy_ptr
TAO_ServerProtocolPolicy::create (const CORBA::Any &val)
{
  RTCORBA::ProtocolList *value = 0;
  if (!(val >>= value))
    throw ::CORBA::PolicyError (CORBA::BAD_POLICY_VALUE);

  TAO_ServerProtocolPolicy *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_ServerProtocolPolicy (*value),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}

RTCORBA::ProtocolList *
TAO_ServerProtocolPolicy::protocols (void)
{
  RTCORBA::ProtocolList *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    RTCORBA::ProtocolList (this->protocols_),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}

CORBA::PolicyType
TAO_ServerProtocolPolicy::policy_type (void)
{
  return RTCORBA::SERVER_PROTOCOL_POLICY_TYPE;
}

CORBA::Policy_ptr
TAO_ServerProtocolPolicy::copy (void)
{
  TAO_ServerProtocolPolicy* tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_ServerProtocolPolicy (*this),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}

void
TAO_ServerProtocolPolicy::destroy (void)
{
}

TAO_Cached_Policy_Type
TAO_ServerProtocolPolicy::_tao_cached_type (void) const
{
  return TAO_CACHED_POLICY_RT_SERVER_PROTOCOL;
}

TAO_Policy_Scope
TAO_ServerProtocolPolicy::_tao_scope (void) const
{
  return static_cast<TAO_Policy_Scope> (TAO_POLICY_ORB_SCOPE |
                          TAO_POLICY_POA_SCOPE);
}

RTCORBA::ProtocolList &
TAO_ServerProtocolPolicy::protocols_rep (void)
{
  return protocols_;
}

// ****************************************************************

TAO_ClientProtocolPolicy::TAO_ClientProtocolPolicy (void)
{
}


TAO_ClientProtocolPolicy::TAO_ClientProtocolPolicy (const RTCORBA::ProtocolList &protocols)
  : ::CORBA::Object ()
  , ::CORBA::Policy ()
  , RTCORBA::ClientProtocolPolicy ()
  , ::CORBA::LocalObject ()
  , protocols_ (protocols)
{
}

TAO_ClientProtocolPolicy::TAO_ClientProtocolPolicy (const TAO_ClientProtocolPolicy &rhs)
  : ::CORBA::Object ()
  , ::CORBA::Policy ()
  , RTCORBA::ClientProtocolPolicy ()
  , ::CORBA::LocalObject ()
  , protocols_ (rhs.protocols_)
{
}

TAO_ClientProtocolPolicy::~TAO_ClientProtocolPolicy ()
{
}

CORBA::Policy_ptr
TAO_ClientProtocolPolicy::create (const CORBA::Any &val)
{
  RTCORBA::ProtocolList *value = 0;
  if (!(val >>= value))
    throw ::CORBA::PolicyError (CORBA::BAD_POLICY_VALUE);

  TAO_ClientProtocolPolicy *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_ClientProtocolPolicy (*value),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}

RTCORBA::ProtocolList *
TAO_ClientProtocolPolicy::protocols (void)
{
  RTCORBA::ProtocolList *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    RTCORBA::ProtocolList (this->protocols_),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}

CORBA::PolicyType
TAO_ClientProtocolPolicy::policy_type (void)
{
  return RTCORBA::CLIENT_PROTOCOL_POLICY_TYPE;
}

CORBA::Policy_ptr
TAO_ClientProtocolPolicy::copy (void)
{
  TAO_ClientProtocolPolicy* tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_ClientProtocolPolicy (*this),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}

void
TAO_ClientProtocolPolicy::destroy (void)
{
}

TAO_Cached_Policy_Type
TAO_ClientProtocolPolicy::_tao_cached_type (void) const
{
  return TAO_CACHED_POLICY_RT_CLIENT_PROTOCOL;
}

TAO_Policy_Scope
TAO_ClientProtocolPolicy::_tao_scope (void) const
{
  return static_cast<TAO_Policy_Scope> (TAO_POLICY_DEFAULT_SCOPE |
                          TAO_POLICY_CLIENT_EXPOSED);
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
        (this->protocols_[i].protocol_type, in_cdr.orb_core ());

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

RTCORBA::ProtocolList &
TAO_ClientProtocolPolicy::protocols_rep (void)
{
  return protocols_;
}

// ****************************************************************

TAO_TCP_Protocol_Properties::TAO_TCP_Protocol_Properties (CORBA::Long send_buffer_size,
                                                          CORBA::Long recv_buffer_size,
                                                          CORBA::Boolean keep_alive,
                                                          CORBA::Boolean dont_route,
                                                          CORBA::Boolean no_delay,
                                                          CORBA::Boolean enable_network_priority)
  : send_buffer_size_ (send_buffer_size),
    recv_buffer_size_ (recv_buffer_size),
    keep_alive_ (keep_alive),
    dont_route_ (dont_route),
    no_delay_ (no_delay),
    enable_network_priority_ (enable_network_priority)
{
}

TAO_TCP_Protocol_Properties::~TAO_TCP_Protocol_Properties (void)
{
}

CORBA::Long
TAO_TCP_Protocol_Properties::send_buffer_size (void)
{
  return this->send_buffer_size_;
}

void
TAO_TCP_Protocol_Properties::send_buffer_size (CORBA::Long send_buffer_size)
{
  this->send_buffer_size_ = send_buffer_size;
}

CORBA::Long
TAO_TCP_Protocol_Properties::recv_buffer_size (void)
{
  return this->recv_buffer_size_;
}

void
TAO_TCP_Protocol_Properties::recv_buffer_size (CORBA::Long recv_buffer_size)
{
  this->recv_buffer_size_ = recv_buffer_size;
}

CORBA::Boolean
TAO_TCP_Protocol_Properties::keep_alive (void)
{
  return this->keep_alive_;
}

void
TAO_TCP_Protocol_Properties::keep_alive (CORBA::Boolean keep_alive)
{
  this->keep_alive_ = keep_alive;
}

CORBA::Boolean
TAO_TCP_Protocol_Properties::dont_route (void)
{
  return this->dont_route_;
}

void
TAO_TCP_Protocol_Properties::dont_route (CORBA::Boolean dont_route)
{
  this->dont_route_ = dont_route;
}

CORBA::Boolean TAO_TCP_Protocol_Properties::no_delay (void)
{
  return this->no_delay_;
}

void
TAO_TCP_Protocol_Properties::no_delay (CORBA::Boolean no_delay)
{
  this->no_delay_ = no_delay;
}

CORBA::Boolean
TAO_TCP_Protocol_Properties::enable_network_priority (void)
{
  return this->enable_network_priority_;
}

void
TAO_TCP_Protocol_Properties::enable_network_priority (CORBA::Boolean enable)
{
  this->enable_network_priority_ = enable;
}

CORBA::Boolean
TAO_TCP_Protocol_Properties::_tao_encode (TAO_OutputCDR & out_cdr)
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
TAO_TCP_Protocol_Properties::_tao_decode (TAO_InputCDR &in_cdr)
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

TAO_UnixDomain_Protocol_Properties::TAO_UnixDomain_Protocol_Properties
(CORBA::Long send_buffer_size,
 CORBA::Long recv_buffer_size)
  : send_buffer_size_ (send_buffer_size),
    recv_buffer_size_ (recv_buffer_size)
{
}

TAO_UnixDomain_Protocol_Properties::~TAO_UnixDomain_Protocol_Properties (void)
{
}

CORBA::Long
TAO_UnixDomain_Protocol_Properties::send_buffer_size (void)
{
  return this->send_buffer_size_;
}

void
TAO_UnixDomain_Protocol_Properties::send_buffer_size (CORBA::Long send_buffer_size)
{
  this->send_buffer_size_ = send_buffer_size;
}

CORBA::Long
TAO_UnixDomain_Protocol_Properties::recv_buffer_size (void)
{
  return this->recv_buffer_size_;
}

void
TAO_UnixDomain_Protocol_Properties::recv_buffer_size (CORBA::Long recv_buffer_size)
{
  this->recv_buffer_size_ = recv_buffer_size;
}

CORBA::Boolean
TAO_UnixDomain_Protocol_Properties::_tao_encode (TAO_OutputCDR &out_cdr)
{
  return ((out_cdr << this->send_buffer_size_)
          && (out_cdr << this->recv_buffer_size_));
}

CORBA::Boolean
TAO_UnixDomain_Protocol_Properties::_tao_decode (TAO_InputCDR &in_cdr)
{
  return ((in_cdr >> this->send_buffer_size_)
          && (in_cdr >> this->recv_buffer_size_));
}

// ****************************************************************

TAO_SharedMemory_Protocol_Properties::TAO_SharedMemory_Protocol_Properties (CORBA::Long send_buffer_size,
                                                                            CORBA::Long recv_buffer_size,
                                                                            CORBA::Boolean keep_alive,
                                                                            CORBA::Boolean dont_route,
                                                                            CORBA::Boolean no_delay,
                                                                            CORBA::Long preallocate_buffer_size,
                                                                            const char *mmap_filename,
                                                                            const char *mmap_lockname)
  : send_buffer_size_ (send_buffer_size),
    recv_buffer_size_ (recv_buffer_size),
    keep_alive_ (keep_alive),
    dont_route_ (dont_route),
    no_delay_ (no_delay),
    preallocate_buffer_size_ (preallocate_buffer_size),
    mmap_filename_ (mmap_filename),
    mmap_lockname_ (mmap_lockname)
{
}

TAO_SharedMemory_Protocol_Properties::~TAO_SharedMemory_Protocol_Properties (void)
{
}


CORBA::Long
TAO_SharedMemory_Protocol_Properties::send_buffer_size (void)
{
  return this->send_buffer_size_;
}

void
TAO_SharedMemory_Protocol_Properties::send_buffer_size (CORBA::Long send_buffer_size)
{
  this->send_buffer_size_ = send_buffer_size;
}

CORBA::Long
TAO_SharedMemory_Protocol_Properties::recv_buffer_size (void)
{
  return this->recv_buffer_size_;
}

void
TAO_SharedMemory_Protocol_Properties::recv_buffer_size (CORBA::Long recv_buffer_size)
{
  this->recv_buffer_size_ = recv_buffer_size;
}

CORBA::Boolean
TAO_SharedMemory_Protocol_Properties::keep_alive (void)
{
  return this->keep_alive_;
}

void
TAO_SharedMemory_Protocol_Properties::keep_alive (CORBA::Boolean keep_alive)
{
  this->keep_alive_ = keep_alive;
}

CORBA::Boolean
TAO_SharedMemory_Protocol_Properties::dont_route (void)
{
  return this->dont_route_;
}

void
TAO_SharedMemory_Protocol_Properties::dont_route (CORBA::Boolean dont_route)
{
  this->dont_route_ = dont_route;
}

CORBA::Boolean
TAO_SharedMemory_Protocol_Properties::no_delay (void)
{
  return this->no_delay_;
}

void
TAO_SharedMemory_Protocol_Properties::no_delay (CORBA::Boolean no_delay)
{
  this->no_delay_ = no_delay;
}

CORBA::Long
TAO_SharedMemory_Protocol_Properties::preallocate_buffer_size (void)
{
  return this->preallocate_buffer_size_;
}

void
TAO_SharedMemory_Protocol_Properties::preallocate_buffer_size (CORBA::Long preallocate_buffer_size)
{
  this->preallocate_buffer_size_ = preallocate_buffer_size;
}

char *
TAO_SharedMemory_Protocol_Properties::mmap_filename (void)
{
  return this->mmap_filename_.rep ();
}

void
TAO_SharedMemory_Protocol_Properties::mmap_filename (const char * mmap_filename)
{
  this->mmap_filename_.set (mmap_filename);
}

char *
TAO_SharedMemory_Protocol_Properties::mmap_lockname (void)
{
  return this->mmap_lockname_.rep ();
}

void
TAO_SharedMemory_Protocol_Properties::mmap_lockname (const char * mmap_lockname)
{
  this->mmap_lockname_.set (mmap_lockname);
}

CORBA::Boolean
TAO_SharedMemory_Protocol_Properties::_tao_encode (TAO_OutputCDR &out_cdr)
{
  return ((out_cdr << this->send_buffer_size_)
          &&
          (out_cdr << this->recv_buffer_size_)
          &&
          (out_cdr.write_boolean (this->keep_alive_))
          &&
          (out_cdr.write_boolean (this->dont_route_))
          &&
          (out_cdr.write_boolean (this->no_delay_))
          &&
          (out_cdr << this->preallocate_buffer_size_)
          &&
          (out_cdr << this->mmap_filename_)
          &&
          (out_cdr << this->mmap_lockname_));
}

CORBA::Boolean
TAO_SharedMemory_Protocol_Properties::_tao_decode (TAO_InputCDR &in_cdr)
{
  return ((in_cdr >> this->send_buffer_size_)
          &&
          (in_cdr >> this->recv_buffer_size_)
          &&
          (in_cdr.read_boolean (this->keep_alive_))
          &&
          (in_cdr.read_boolean (this->dont_route_))
          &&
          (in_cdr.read_boolean (this->no_delay_))
          &&
          (in_cdr >> this->preallocate_buffer_size_)
          &&
          (in_cdr >> this->mmap_filename_)
          &&
          (in_cdr >> this->mmap_lockname_));
}

// ****************************************************************

TAO_UserDatagram_Protocol_Properties::TAO_UserDatagram_Protocol_Properties (
  CORBA::Long send_buffer_size,
  CORBA::Long recv_buffer_size,
  CORBA::Boolean enable_network_priority)
  : send_buffer_size_ (send_buffer_size),
    recv_buffer_size_ (recv_buffer_size),
    enable_network_priority_ (enable_network_priority)
{
}

TAO_UserDatagram_Protocol_Properties::~TAO_UserDatagram_Protocol_Properties (void)
{
}

CORBA::Boolean
TAO_UserDatagram_Protocol_Properties::enable_network_priority (void)
{
  return this->enable_network_priority_;
}

void
TAO_UserDatagram_Protocol_Properties::enable_network_priority (CORBA::Boolean enable)
{
  this->enable_network_priority_ = enable;
}

CORBA::Long
TAO_UserDatagram_Protocol_Properties::send_buffer_size (void)
{
  return this->send_buffer_size_;
}

void
TAO_UserDatagram_Protocol_Properties::send_buffer_size (CORBA::Long send_buffer_size)
{
  this->send_buffer_size_ = send_buffer_size;
}

CORBA::Long
TAO_UserDatagram_Protocol_Properties::recv_buffer_size (void)
{
  return this->recv_buffer_size_;
}

void
TAO_UserDatagram_Protocol_Properties::recv_buffer_size (CORBA::Long recv_buffer_size)
{
  this->recv_buffer_size_ = recv_buffer_size;
}

CORBA::Boolean
TAO_UserDatagram_Protocol_Properties::_tao_encode (TAO_OutputCDR &)
{
  return true;
}

CORBA::Boolean
TAO_UserDatagram_Protocol_Properties::_tao_decode (TAO_InputCDR &)
{
  return true;
}

// ****************************************************************

TAO_StreamControl_Protocol_Properties::TAO_StreamControl_Protocol_Properties (CORBA::Long send_buffer_size,
                                                                              CORBA::Long recv_buffer_size,
                                                                              CORBA::Boolean keep_alive,
                                                                              CORBA::Boolean dont_route,
                                                                              CORBA::Boolean no_delay,
                                                                              CORBA::Boolean enable_network_priority)
  : send_buffer_size_ (send_buffer_size),
    recv_buffer_size_ (recv_buffer_size),
    keep_alive_ (keep_alive),
    dont_route_ (dont_route),
    no_delay_ (no_delay),
    enable_network_priority_ (enable_network_priority)
{
}

TAO_StreamControl_Protocol_Properties::~TAO_StreamControl_Protocol_Properties (void)
{
}

CORBA::Long
TAO_StreamControl_Protocol_Properties::send_buffer_size (void)
{
  return this->send_buffer_size_;
}

void
TAO_StreamControl_Protocol_Properties::send_buffer_size (CORBA::Long send_buffer_size)
{
  this->send_buffer_size_ = send_buffer_size;
}

CORBA::Long
TAO_StreamControl_Protocol_Properties::recv_buffer_size (void)
{
  return this->recv_buffer_size_;
}

void
TAO_StreamControl_Protocol_Properties::recv_buffer_size (CORBA::Long recv_buffer_size)
{
  this->recv_buffer_size_ = recv_buffer_size;
}

CORBA::Boolean
TAO_StreamControl_Protocol_Properties::keep_alive (void)
{
  return this->keep_alive_;
}

void
TAO_StreamControl_Protocol_Properties::keep_alive (CORBA::Boolean keep_alive)
{
  this->keep_alive_ = keep_alive;
}

CORBA::Boolean
TAO_StreamControl_Protocol_Properties::dont_route (void)
{
  return this->dont_route_;
}

void
TAO_StreamControl_Protocol_Properties::dont_route (CORBA::Boolean dont_route)
{
  this->dont_route_ = dont_route;
}

CORBA::Boolean TAO_StreamControl_Protocol_Properties::no_delay (void)
{
  return this->no_delay_;
}

void
TAO_StreamControl_Protocol_Properties::no_delay (CORBA::Boolean no_delay)
{
  this->no_delay_ = no_delay;
}

CORBA::Boolean
TAO_StreamControl_Protocol_Properties::enable_network_priority (void)
{
  return this->enable_network_priority_;
}

void
TAO_StreamControl_Protocol_Properties::enable_network_priority (CORBA::Boolean enable)
{
  this->enable_network_priority_ = enable;
}

CORBA::Boolean
TAO_StreamControl_Protocol_Properties::_tao_encode (TAO_OutputCDR & out_cdr)
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
TAO_StreamControl_Protocol_Properties::_tao_decode (TAO_InputCDR &in_cdr)
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

TAO_GIOP_Protocol_Properties::TAO_GIOP_Protocol_Properties (void)
{
}

TAO_GIOP_Protocol_Properties::~TAO_GIOP_Protocol_Properties (void)
{
}

CORBA::Boolean
TAO_GIOP_Protocol_Properties::_tao_encode (TAO_OutputCDR &)
{
  return true;
}

CORBA::Boolean
TAO_GIOP_Protocol_Properties::_tao_decode (TAO_InputCDR &)
{
  return true;
}

// ****************************************************************

RTCORBA::ProtocolProperties *
TAO_Protocol_Properties_Factory::create_transport_protocol_property (IOP::ProfileId id,
                                                                     TAO_ORB_Core *orb_core)
{
  RTCORBA::ProtocolProperties* property = 0;

  if (id == IOP::TAG_INTERNET_IOP)
    {
      int send_buffer_size = orb_core ? orb_core->orb_params ()->sock_sndbuf_size () : 0;
      int recv_buffer_size = orb_core ? orb_core->orb_params ()->sock_rcvbuf_size () : 0;
      int no_delay = orb_core ? orb_core->orb_params ()->nodelay () : 0;
      CORBA::Boolean keep_alive = orb_core ? orb_core->orb_params ()->sock_keepalive () : true;
      CORBA::Boolean dont_route = orb_core ? orb_core->orb_params ()->sock_dontroute () : false;
      CORBA::Boolean enable_network_priority = false;

      ACE_NEW_RETURN (property,
                      TAO_TCP_Protocol_Properties (send_buffer_size,
                                                   recv_buffer_size,
                                                   keep_alive,
                                                   dont_route,
                                                   no_delay,
                                                   enable_network_priority),
                      0);
    }

  else if (id == TAO_TAG_SHMEM_PROFILE)
    {
      int send_buffer_size = orb_core ? orb_core->orb_params ()->sock_sndbuf_size () : 0;
      int recv_buffer_size = orb_core ? orb_core->orb_params ()->sock_rcvbuf_size () : 0;
      int no_delay = orb_core ? orb_core->orb_params ()->nodelay () : 0;
      CORBA::Boolean keep_alive = true;
      CORBA::Boolean dont_route = false;
      CORBA::Long preallocate_buffer_size = false;
      const char *mmap_filename = "";
      const char *mmap_lockname = "";

      ACE_NEW_RETURN (property,
                      TAO_SharedMemory_Protocol_Properties (send_buffer_size,
                                                            recv_buffer_size,
                                                            keep_alive,
                                                            dont_route,
                                                            no_delay,
                                                            preallocate_buffer_size,
                                                            mmap_filename,
                                                            mmap_lockname),
                      0);
    }

  else if (id == TAO_TAG_UIOP_PROFILE)
    {
      int const send_buffer_size = orb_core ? orb_core->orb_params ()->sock_sndbuf_size () : 0;
      int const recv_buffer_size = orb_core ? orb_core->orb_params ()->sock_rcvbuf_size () : 0;

      ACE_NEW_RETURN (property,
                      TAO_UnixDomain_Protocol_Properties (send_buffer_size,
                                                          recv_buffer_size),
                      0);
    }

  else if (id == TAO_TAG_DIOP_PROFILE)
    {
      int const send_buffer_size = orb_core ? orb_core->orb_params ()->sock_sndbuf_size () : 0;
      int const recv_buffer_size = orb_core ? orb_core->orb_params ()->sock_rcvbuf_size () : 0;
      CORBA::Boolean enable_network_priority = false;

      ACE_NEW_RETURN (property,
                      TAO_UserDatagram_Protocol_Properties (send_buffer_size,
                                                            recv_buffer_size,
                                                            enable_network_priority),
                      0);
    }

  else if (id == TAO_TAG_SCIOP_PROFILE)
    {
      int send_buffer_size = orb_core ? orb_core->orb_params ()->sock_sndbuf_size () : 0;
      int recv_buffer_size = orb_core ? orb_core->orb_params ()->sock_rcvbuf_size () : 0;
      int no_delay = orb_core ? orb_core->orb_params ()->nodelay () : 0;
      CORBA::Boolean keep_alive = true;
      CORBA::Boolean dont_route = false;
      CORBA::Boolean enable_network_priority = false;

      ACE_NEW_RETURN (property,
                      TAO_StreamControl_Protocol_Properties (send_buffer_size,
                                                             recv_buffer_size,
                                                             keep_alive,
                                                             dont_route,
                                                             no_delay,
                                                             enable_network_priority),
                      0);
    }

  return property;
}

RTCORBA::ProtocolProperties*
TAO_Protocol_Properties_Factory::create_orb_protocol_property (IOP::ProfileId id)
{
  RTCORBA::ProtocolProperties* property = 0;

  if (id == IOP::TAG_INTERNET_IOP)
    ACE_NEW_RETURN (property,
                    TAO_GIOP_Protocol_Properties,
                    0);

  // Right now the only supported ORB protocol is GIOP
  // so we couple this with every protocol property.
  // The else statement is not necessary, but it
  // is here just to make clear that as soon as
  // new ORB protocol are supported other case
  // should be considered.
  else
    ACE_NEW_RETURN (property,
                    TAO_GIOP_Protocol_Properties,
                    0);
  return property;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
