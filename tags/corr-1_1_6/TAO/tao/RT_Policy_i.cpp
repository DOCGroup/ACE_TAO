// $Id$

#include "tao/RT_Policy_i.h"


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

  CORBA::Boolean b = (out_cdr << priority_model_);
  if (b  && (out_cdr << server_priority_))
    return 1;

  return 0;
}

CORBA::Boolean
TAO_PriorityModelPolicy::_tao_decode (TAO_InputCDR &in_cdr)
{
  CORBA::Boolean b = (in_cdr >> priority_model_);
  if (b  && (in_cdr >> server_priority_))
    return 1;

  priority_model_ = RTCORBA::SERVER_DECLARED;
  server_priority_ = 0;

  return 0;
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
  ACE_THROW_SPEC (())
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
  ACE_THROW_SPEC (())
{
  return this->send_buffer_size_;
}

void
TAO_TCP_Properties::send_buffer_size (CORBA::Long send_buffer_size,
                                      CORBA::Environment &)
  ACE_THROW_SPEC (())
{
  this->send_buffer_size_ = send_buffer_size;
}

CORBA::Long
TAO_TCP_Properties::recv_buffer_size (CORBA::Environment &)
  ACE_THROW_SPEC (())
{
  return this->recv_buffer_size_;
}

void
TAO_TCP_Properties::recv_buffer_size (CORBA::Long recv_buffer_size,
                                      CORBA::Environment &)
  ACE_THROW_SPEC (())
{
  this->recv_buffer_size_ = recv_buffer_size;
}

CORBA::Boolean
TAO_TCP_Properties::keep_alive (CORBA::Environment &)
  ACE_THROW_SPEC (())
{
  return this->keep_alive_;
}

void
TAO_TCP_Properties::keep_alive (CORBA::Boolean keep_alive,
                                CORBA::Environment &)
  ACE_THROW_SPEC (())
{
  this->keep_alive_ = keep_alive;
}

CORBA::Boolean
TAO_TCP_Properties::dont_route (CORBA::Environment &)
  ACE_THROW_SPEC (())
{
  return this->dont_route_;
}

void
TAO_TCP_Properties::dont_route (CORBA::Boolean dont_route,
                                CORBA::Environment &)
  ACE_THROW_SPEC (())
{
  this->dont_route_ = dont_route;
}

CORBA::Boolean TAO_TCP_Properties::no_delay (CORBA::Environment &)
  ACE_THROW_SPEC (())
{
  return this->no_delay_;
}

void
TAO_TCP_Properties::no_delay (CORBA::Boolean no_delay,
                              CORBA::Environment &)
  ACE_THROW_SPEC (())
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
        (out_cdr << protocols_[i].protocol_type)
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
  RTCORBA::ProtocolProperties *protocol_properties;

  for (CORBA::ULong i = 0; (i < length) && is_read_ok; i++)
    {
      IOP::ProfileId id;
      is_read_ok = in_cdr >> id;

      protocol_properties =
        TAO_Protocol_Properties_Factory::create_orb_protocol_property (id);

      this->protocols_[i].transport_protocol_properties =
        TAO_Protocol_Properties_Factory::create_transport_protocol_property (id);

      if (is_read_ok && protocol_properties != 0 )
        {
          this->protocols_[i].orb_protocol_properties = protocol_properties;

          if (is_read_ok && (this->protocols_[i].orb_protocol_properties.ptr () == 0))
            is_read_ok =
              this->protocols_[i].orb_protocol_properties->_tao_decode (in_cdr);

          if (is_read_ok
              && (this->protocols_[i].transport_protocol_properties.ptr () == 0))
            is_read_ok =
              this->protocols_[i].transport_protocol_properties->_tao_decode (in_cdr);
        }
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

// ****************************************************************

RTCORBA::ProtocolProperties*
TAO_Protocol_Properties_Factory::create_transport_protocol_property (IOP::ProfileId id)
{
  RTCORBA::ProtocolProperties* property = 0;

  if (id == IOP::TAG_INTERNET_IOP)
    ACE_NEW_RETURN (property, TAO_TCP_Properties, 0);

  return property;
}

RTCORBA::ProtocolProperties*
TAO_Protocol_Properties_Factory::create_orb_protocol_property (IOP::ProfileId id)
{
  RTCORBA::ProtocolProperties* property = 0;

  if (id == IOP::TAG_INTERNET_IOP)
    ACE_NEW_RETURN (property, TAO_GIOP_Properties, 0);

  return property;
}

#endif /* TAO_HAS_RT_CORBA == 1 */
