// $Id$

#include "RT_Policy_i.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/ORB_Core.h"
#include "tao/debug.h"

#if ! defined (__ACE_INLINE__)
#include "RT_Policy_i.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID (TAO,
           RT_Policy_i,
           "$Id$")

// ****************************************************************

TAO_PriorityModelPolicy::TAO_PriorityModelPolicy (RTCORBA::PriorityModel priority_model,
                                                  RTCORBA::Priority server_priority)
  : ACE_NESTED_CLASS (CORBA, Object) ()
  , ACE_NESTED_CLASS (CORBA, Policy) ()
  , ACE_NESTED_CLASS (CORBA, LocalObject) ()
  , RTCORBA::PriorityModelPolicy ()
  , TAO_Local_RefCounted_Object ()
  , priority_model_ (priority_model)
  , server_priority_ (server_priority)
{
}

TAO_PriorityModelPolicy::TAO_PriorityModelPolicy (const TAO_PriorityModelPolicy &rhs)
  : ACE_NESTED_CLASS (CORBA, Object) ()
  , ACE_NESTED_CLASS (CORBA, Policy) ()
  , ACE_NESTED_CLASS (CORBA, LocalObject) ()
  , RTCORBA::PriorityModelPolicy ()
  , TAO_Local_RefCounted_Object ()
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
TAO_PriorityModelPolicy::create (const CORBA::Any &
                                 ACE_ENV_ARG_DECL)
{
  /*
   * @@ The following code should be changed once the OMG spec has
   * been fixed such that a RTCORBA::PriorityModelPolicy can be
   * created by using the ORB::create_policy interface.
   */
  ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                    CORBA::Policy::_nil ());
}

RTCORBA::PriorityModel
TAO_PriorityModelPolicy::get_priority_model (void)
{
  return this->priority_model_;
}

RTCORBA::PriorityModel
TAO_PriorityModelPolicy::priority_model (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->priority_model_;
}

RTCORBA::Priority
TAO_PriorityModelPolicy::server_priority (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->server_priority_;
}

CORBA::PolicyType
TAO_PriorityModelPolicy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return RTCORBA::PRIORITY_MODEL_POLICY_TYPE;
}

CORBA::Policy_ptr
TAO_PriorityModelPolicy::copy (ACE_ENV_SINGLE_ARG_DECL)
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

void
TAO_PriorityModelPolicy::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
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
  return ACE_static_cast (TAO_Policy_Scope,
                          TAO_POLICY_ORB_SCOPE |
                          TAO_POLICY_POA_SCOPE |
                          TAO_POLICY_CLIENT_EXPOSED);
}

// ****************************************************************

TAO_ThreadpoolPolicy::TAO_ThreadpoolPolicy (RTCORBA::ThreadpoolId id)
  : ACE_NESTED_CLASS (CORBA, Object) ()
  , ACE_NESTED_CLASS (CORBA, Policy) ()
  , ACE_NESTED_CLASS (CORBA, LocalObject) ()
  , RTCORBA::ThreadpoolPolicy ()
  , TAO_Local_RefCounted_Object ()
  , id_ (id)
{
}

TAO_ThreadpoolPolicy::TAO_ThreadpoolPolicy (const TAO_ThreadpoolPolicy &rhs)
  : ACE_NESTED_CLASS (CORBA, Object) ()
  , ACE_NESTED_CLASS (CORBA, Policy) ()
  , ACE_NESTED_CLASS (CORBA, LocalObject) ()
  , RTCORBA::ThreadpoolPolicy ()
  , TAO_Local_RefCounted_Object ()
  , id_ (rhs.id_)
{
}

TAO_ThreadpoolPolicy::~TAO_ThreadpoolPolicy (void)
{
}

CORBA::Policy_ptr
TAO_ThreadpoolPolicy::create (const CORBA::Any &val
                              ACE_ENV_ARG_DECL)
{
  RTCORBA::ThreadpoolId value;
  if ((val >>= value) == 0)
    ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                      CORBA::Policy::_nil ());

  TAO_ThreadpoolPolicy *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_ThreadpoolPolicy (value),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return tmp;
}

RTCORBA::ThreadpoolId
TAO_ThreadpoolPolicy::threadpool (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->id_;
}

CORBA::PolicyType
TAO_ThreadpoolPolicy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return RTCORBA::THREADPOOL_POLICY_TYPE;
}

CORBA::Policy_ptr
TAO_ThreadpoolPolicy::copy (ACE_ENV_SINGLE_ARG_DECL)
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

void
TAO_ThreadpoolPolicy::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
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
  return ACE_static_cast (TAO_Policy_Scope,
                          TAO_POLICY_ORB_SCOPE |
                          TAO_POLICY_POA_SCOPE);
}

// ****************************************************************

TAO_PrivateConnectionPolicy::TAO_PrivateConnectionPolicy (void)
  : ACE_NESTED_CLASS (CORBA, Object) ()
  , ACE_NESTED_CLASS (CORBA, Policy) ()
  , ACE_NESTED_CLASS (CORBA, LocalObject) ()
  , RTCORBA::PrivateConnectionPolicy ()
  , TAO_Local_RefCounted_Object ()
{
}

TAO_PrivateConnectionPolicy::TAO_PrivateConnectionPolicy (const TAO_PrivateConnectionPolicy &)
  : ACE_NESTED_CLASS (CORBA, Object) ()
  , ACE_NESTED_CLASS (CORBA, Policy) ()
  , ACE_NESTED_CLASS (CORBA, LocalObject) ()
  , RTCORBA::PrivateConnectionPolicy ()
  , TAO_Local_RefCounted_Object ()
{
}

TAO_PrivateConnectionPolicy::~TAO_PrivateConnectionPolicy (void)
{
}

CORBA::Policy_ptr
TAO_PrivateConnectionPolicy::create (const CORBA::Any &
                                     ACE_ENV_ARG_DECL)
{
  TAO_PrivateConnectionPolicy *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_PrivateConnectionPolicy (),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return tmp;
}

CORBA::PolicyType
TAO_PrivateConnectionPolicy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return RTCORBA::PRIVATE_CONNECTION_POLICY_TYPE;
}

CORBA::Policy_ptr
TAO_PrivateConnectionPolicy::copy (ACE_ENV_SINGLE_ARG_DECL)
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
TAO_PrivateConnectionPolicy::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
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
  return ACE_static_cast (TAO_Policy_Scope,
                          TAO_POLICY_OBJECT_SCOPE |
                          TAO_POLICY_THREAD_SCOPE |
                          TAO_POLICY_ORB_SCOPE);
}

// ****************************************************************

TAO_PriorityBandedConnectionPolicy::TAO_PriorityBandedConnectionPolicy (const RTCORBA::PriorityBands &bands)
  : ACE_NESTED_CLASS (CORBA, Object) ()
  , ACE_NESTED_CLASS (CORBA, Policy) ()
  , ACE_NESTED_CLASS (CORBA, LocalObject) ()
  , RTCORBA::PriorityBandedConnectionPolicy ()
  , TAO_Local_RefCounted_Object ()
  , priority_bands_ (bands)
{
}

TAO_PriorityBandedConnectionPolicy::TAO_PriorityBandedConnectionPolicy (const TAO_PriorityBandedConnectionPolicy &rhs)
  : ACE_NESTED_CLASS (CORBA, Object) ()
  , ACE_NESTED_CLASS (CORBA, Policy) ()
  , ACE_NESTED_CLASS (CORBA, LocalObject) ()
  , RTCORBA::PriorityBandedConnectionPolicy ()
  , TAO_Local_RefCounted_Object ()
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
TAO_PriorityBandedConnectionPolicy::create (const CORBA::Any &val
                                            ACE_ENV_ARG_DECL)
{
  RTCORBA::PriorityBands *value = 0;
  if ((val >>= value) == 0)
    ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                      CORBA::Policy::_nil ());

  TAO_PriorityBandedConnectionPolicy *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_PriorityBandedConnectionPolicy (*value),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return tmp;
}

RTCORBA::PriorityBands *
TAO_PriorityBandedConnectionPolicy::priority_bands (ACE_ENV_SINGLE_ARG_DECL)
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
TAO_PriorityBandedConnectionPolicy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE;
}

CORBA::Policy_ptr
TAO_PriorityBandedConnectionPolicy::copy (ACE_ENV_SINGLE_ARG_DECL)
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

void
TAO_PriorityBandedConnectionPolicy::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
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

TAO_Cached_Policy_Type
TAO_PriorityBandedConnectionPolicy::_tao_cached_type (void) const
{
  return TAO_CACHED_POLICY_RT_PRIORITY_BANDED_CONNECTION;
}

TAO_Policy_Scope
TAO_PriorityBandedConnectionPolicy::_tao_scope (void) const
{
  return ACE_static_cast (TAO_Policy_Scope,
                          TAO_POLICY_DEFAULT_SCOPE |
                          TAO_POLICY_CLIENT_EXPOSED);
}

RTCORBA::PriorityBands &
TAO_PriorityBandedConnectionPolicy::priority_bands_rep (void)
{
  return priority_bands_;
}

// ****************************************************************

TAO_ServerProtocolPolicy::TAO_ServerProtocolPolicy (const RTCORBA::ProtocolList &protocols)
  : ACE_NESTED_CLASS (CORBA, Object) ()
  , ACE_NESTED_CLASS (CORBA, Policy) ()
  , ACE_NESTED_CLASS (CORBA, LocalObject) ()
  , RTCORBA::ServerProtocolPolicy ()
  , TAO_Local_RefCounted_Object ()
  , protocols_ (protocols)
{
}

TAO_ServerProtocolPolicy::TAO_ServerProtocolPolicy (const TAO_ServerProtocolPolicy &rhs)
  : ACE_NESTED_CLASS (CORBA, Object) ()
  , ACE_NESTED_CLASS (CORBA, Policy) ()
  , ACE_NESTED_CLASS (CORBA, LocalObject) ()
  , RTCORBA::ServerProtocolPolicy ()
  , TAO_Local_RefCounted_Object ()
  , protocols_ (rhs.protocols_)
{
}

TAO_ServerProtocolPolicy::~TAO_ServerProtocolPolicy (void)
{
}

CORBA::Policy_ptr
TAO_ServerProtocolPolicy::create (const CORBA::Any &val
                                  ACE_ENV_ARG_DECL)
{
  RTCORBA::ProtocolList *value = 0;
  if ((val >>= value) == 0)
    ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                      CORBA::Policy::_nil ());

  TAO_ServerProtocolPolicy *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_ServerProtocolPolicy (*value),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return tmp;
}

RTCORBA::ProtocolList *
TAO_ServerProtocolPolicy::protocols (ACE_ENV_SINGLE_ARG_DECL)
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
TAO_ServerProtocolPolicy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return RTCORBA::SERVER_PROTOCOL_POLICY_TYPE;
}

CORBA::Policy_ptr
TAO_ServerProtocolPolicy::copy (ACE_ENV_SINGLE_ARG_DECL)
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
TAO_ServerProtocolPolicy::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

int
TAO_ServerProtocolPolicy::hook (TAO_ORB_Core *orb_core,
                                int &send_buffer_size,
                                int &recv_buffer_size,
                                int &no_delay,
                                int &enable_network_priority,
                                const char *protocol_type)
{
  RTCORBA::ProtocolProperties_var properties =
    RTCORBA::ProtocolProperties::_nil ();

  // ServerProtocolProperties policy controls protocols configuration.
  // Look for protocol properties in the effective ServerProtocolPolicy.
  CORBA::Policy_var policy =
    orb_core->get_cached_policy (TAO_CACHED_POLICY_RT_SERVER_PROTOCOL);

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      RTCORBA::ServerProtocolPolicy_var server_protocols_policy;
      TAO_ServerProtocolPolicy *server_protocols = 0;

      if (!CORBA::is_nil (policy.in ()))
        {
          server_protocols_policy =
            RTCORBA::ServerProtocolPolicy::_narrow (policy.in ()
                                                    ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          server_protocols =
            ACE_dynamic_cast (TAO_ServerProtocolPolicy *,
                              server_protocols_policy.in ());

          if (server_protocols != 0)
            {
              RTCORBA::ProtocolList & protocols =
                server_protocols->protocols_rep ();

              // Find protocol properties.
              for (CORBA::ULong j = 0; j < protocols.length (); ++j)
                {
                  if (protocols[j].protocol_type == IOP::TAG_INTERNET_IOP)
                    {
                      properties =
                        RTCORBA::ProtocolProperties::_narrow (
                          protocols[j].transport_protocol_properties.in ()
                          ACE_ENV_ARG_PARAMETER);
                      ACE_TRY_CHECK;

                      if (ACE_OS::strcmp (protocol_type,
                                          "iiop") == 0)
                        break;
                    }
                  else if (protocols[j].protocol_type == TAO_TAG_UIOP_PROFILE)
                    {
                      properties =
                        RTCORBA::ProtocolProperties::_narrow (
                          protocols[j].transport_protocol_properties.in ()
                          ACE_ENV_ARG_PARAMETER);
                      ACE_TRY_CHECK;

                        if (ACE_OS::strcmp (protocol_type, "uiop") == 0)
                          break;
                    }
                }
            }
        }

      if (CORBA::is_nil (properties.in ()))
        {
          // TCP/UIOP/SHMIOP Properties were not specified in the
          // effective policy.
          // We must use ORB defaults.
          policy = orb_core->get_default_policies ()->get_cached_policy (TAO_CACHED_POLICY_RT_SERVER_PROTOCOL);

          if (!CORBA::is_nil (policy.in ()))
            {
              server_protocols_policy =
                RTCORBA::ServerProtocolPolicy::_narrow (policy.in ()
                                                        ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              server_protocols =
                ACE_dynamic_cast (TAO_ServerProtocolPolicy *,
                                  server_protocols_policy.in ());

              if (!CORBA::is_nil (server_protocols))
                {
                  // Find protocol properties for IIOP.
                  RTCORBA::ProtocolList & protocols =
                    server_protocols->protocols_rep ();

                  for (CORBA::ULong j = 0; j < protocols.length (); ++j)
                    {
                      if (protocols[j].protocol_type == IOP::TAG_INTERNET_IOP)
                        {
                          properties =
                            RTCORBA::ProtocolProperties::_narrow (
                              protocols[j].transport_protocol_properties.in ()
                              ACE_ENV_ARG_PARAMETER);
                          ACE_TRY_CHECK;

                          if (ACE_OS::strcmp (protocol_type, "iiop") == 0)
                            break;
                        }
                      else if (protocols[j].protocol_type ==
                                 TAO_TAG_UIOP_PROFILE)
                        {
                          properties =
                            RTCORBA::ProtocolProperties::_narrow (
                              protocols[j].transport_protocol_properties.in ()
                              ACE_ENV_ARG_PARAMETER);
                          ACE_TRY_CHECK;

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
            RTCORBA::TCPProtocolProperties::_narrow (properties.in ()
                                                     ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          send_buffer_size =
            tcp_properties->send_buffer_size (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
          recv_buffer_size =
            tcp_properties->recv_buffer_size (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
          no_delay = tcp_properties->no_delay (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
          enable_network_priority =
            tcp_properties->enable_network_priority (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      if (ACE_OS::strcmp (protocol_type, "uiop") == 0)
        {
          RTCORBA::UnixDomainProtocolProperties_var uiop_properties =
            RTCORBA::UnixDomainProtocolProperties::_narrow (properties.in ()
                                                            ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (!CORBA::is_nil (uiop_properties.in ()))
            {
              // Extract and locally store properties of interest.
              send_buffer_size =
                uiop_properties->send_buffer_size (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;
              recv_buffer_size =
                uiop_properties->recv_buffer_size (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
        }
    }
  ACE_CATCHANY
    {
      if (TAO_debug_level > 4)
        ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                             "TAO_ServerProtocolPolicy::hook");

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

TAO_Cached_Policy_Type
TAO_ServerProtocolPolicy::_tao_cached_type (void) const
{
  return TAO_CACHED_POLICY_RT_SERVER_PROTOCOL;
}

TAO_Policy_Scope
TAO_ServerProtocolPolicy::_tao_scope (void) const
{
  return ACE_static_cast (TAO_Policy_Scope,
                          TAO_POLICY_ORB_SCOPE |
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
  : ACE_NESTED_CLASS (CORBA, Object) ()
  , ACE_NESTED_CLASS (CORBA, Policy) ()
  , ACE_NESTED_CLASS (CORBA, LocalObject) ()
  , RTCORBA::ClientProtocolPolicy ()
  , TAO_Local_RefCounted_Object ()
  , protocols_ (protocols)
{
}

TAO_ClientProtocolPolicy::TAO_ClientProtocolPolicy (const TAO_ClientProtocolPolicy &rhs)
  : ACE_NESTED_CLASS (CORBA, Object) ()
  , ACE_NESTED_CLASS (CORBA, Policy) ()
  , ACE_NESTED_CLASS (CORBA, LocalObject) ()
  , RTCORBA::ClientProtocolPolicy ()
  , TAO_Local_RefCounted_Object ()
  , protocols_ (rhs.protocols_)
{
}

TAO_ClientProtocolPolicy::~TAO_ClientProtocolPolicy ()
{
}

CORBA::Policy_ptr
TAO_ClientProtocolPolicy::create (const CORBA::Any &val
                                  ACE_ENV_ARG_DECL)
{
  RTCORBA::ProtocolList *value = 0;
  if ((val >>= value) == 0)
    ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                      CORBA::Policy::_nil ());

  TAO_ClientProtocolPolicy *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_ClientProtocolPolicy (*value),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return tmp;
}

RTCORBA::ProtocolList *
TAO_ClientProtocolPolicy::protocols (ACE_ENV_SINGLE_ARG_DECL)
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
TAO_ClientProtocolPolicy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return RTCORBA::CLIENT_PROTOCOL_POLICY_TYPE;
}

CORBA::Policy_ptr
TAO_ClientProtocolPolicy::copy (ACE_ENV_SINGLE_ARG_DECL)
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
TAO_ClientProtocolPolicy::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

int
TAO_ClientProtocolPolicy::hook (TAO_ORB_Core *orb_core,
                                int &send_buffer_size,
                                int &recv_buffer_size,
                                int &no_delay,
                                int &enable_network_priority,
                                const char *protocol_type)
{
  RTCORBA::ProtocolProperties_var properties =
    RTCORBA::ProtocolProperties::_nil ();

  // Check ORB-level override for properties.
  CORBA::Policy_var policy =
    orb_core->policy_manager ()->get_cached_policy (TAO_CACHED_POLICY_RT_CLIENT_PROTOCOL);

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      RTCORBA::ClientProtocolPolicy_var client_protocols_policy;
      TAO_ClientProtocolPolicy *client_protocols = 0;

      if (!CORBA::is_nil (policy.in ()))
        {
          client_protocols_policy =
            RTCORBA::ClientProtocolPolicy::_narrow (policy.in ()
                                                    ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          client_protocols =
            ACE_dynamic_cast (TAO_ClientProtocolPolicy *,
                              client_protocols_policy.in ());

          if (client_protocols != 0)
            {
              //TAO_ClientProtocolPolicy
              RTCORBA::ProtocolList & protocols =
                client_protocols->protocols_rep ();

              for (CORBA::ULong j = 0; j < protocols.length (); ++j)
                {
                  if (protocols[j].protocol_type == IOP::TAG_INTERNET_IOP)
                    {
                      properties =
                        RTCORBA::ProtocolProperties::_narrow (
                          protocols[j].transport_protocol_properties.in ()
                          ACE_ENV_ARG_PARAMETER);
                      ACE_TRY_CHECK;

                      if (ACE_OS::strcmp (protocol_type, "iiop") == 0)
                        break;
                    }
                  else if (protocols[j].protocol_type == TAO_TAG_UIOP_PROFILE)
                    {
                      properties =
                        RTCORBA::ProtocolProperties::_narrow (
                          protocols[j].transport_protocol_properties.in ()
                          ACE_ENV_ARG_PARAMETER);
                      ACE_TRY_CHECK;

                      if (ACE_OS::strcmp (protocol_type, "uiop") == 0)
                        break;
                    }
                }
            }
        }

      if (CORBA::is_nil (properties.in ()))
        {
          // No tcp/uiop properties in ORB-level override.  Use ORB
          // defaults.  ORB defaults should never be null - they were
          // initialized by the ORB_Core.
          policy =
            orb_core->get_default_policies ()->get_cached_policy (TAO_CACHED_POLICY_RT_CLIENT_PROTOCOL);

          if (!CORBA::is_nil (policy.in ()))
            {
              client_protocols_policy =
                RTCORBA::ClientProtocolPolicy::_narrow (policy.in ()
                                                        ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              client_protocols =
                ACE_dynamic_cast (TAO_ClientProtocolPolicy *,
                                  client_protocols_policy.in ());

              if (client_protocols != 0)
                {
                  RTCORBA::ProtocolList & protocols =
                    client_protocols->protocols_rep ();

                  for (CORBA::ULong j = 0; j < protocols.length (); ++j)
                    {
                      if (protocols[j].protocol_type == IOP::TAG_INTERNET_IOP)
                        {
                          properties =
                            RTCORBA::ProtocolProperties::_narrow (
                              protocols[j].transport_protocol_properties.in ()
                              ACE_ENV_ARG_PARAMETER);
                          ACE_TRY_CHECK;

                          if (ACE_OS::strcmp (protocol_type, "iiop") == 0)
                            break;
                        }
                      else if (protocols[j].protocol_type ==
                                 TAO_TAG_UIOP_PROFILE)
                        {
                          properties =
                            RTCORBA::ProtocolProperties::_narrow (
                              protocols[j].transport_protocol_properties.in ()
                              ACE_ENV_ARG_PARAMETER);
                          ACE_TRY_CHECK;

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
            RTCORBA::TCPProtocolProperties::_narrow (properties.in ()
                                                     ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          send_buffer_size =
            tcp_properties->send_buffer_size (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
          recv_buffer_size =
            tcp_properties->recv_buffer_size (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
          no_delay = tcp_properties->no_delay (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
          enable_network_priority =
            tcp_properties->enable_network_priority (ACE_ENV_SINGLE_ARG_PARAMETER);
        }

      if (ACE_OS::strcmp (protocol_type, "uiop") == 0)
        {
          RTCORBA::UnixDomainProtocolProperties_var uiop_properties =
            RTCORBA::UnixDomainProtocolProperties::_narrow (properties.in ()
                                                            ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (!CORBA::is_nil (uiop_properties.in ()))
            {
              // Extract and locally store properties of interest.
              send_buffer_size =
                uiop_properties->send_buffer_size (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;
              recv_buffer_size =
                uiop_properties->recv_buffer_size ();
              ACE_TRY_CHECK;
            }
        }
    }
  ACE_CATCHANY
    {
      if (TAO_debug_level > 4)
        ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                             "TAO_ClientProtocolPolicy::hook");

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

TAO_Cached_Policy_Type
TAO_ClientProtocolPolicy::_tao_cached_type (void) const
{
  return TAO_CACHED_POLICY_RT_CLIENT_PROTOCOL;
}

TAO_Policy_Scope
TAO_ClientProtocolPolicy::_tao_scope (void) const
{
  return ACE_static_cast (TAO_Policy_Scope,
                          TAO_POLICY_DEFAULT_SCOPE |
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

RTCORBA::ProtocolList &
TAO_ClientProtocolPolicy::protocols_rep (void)
{
  return protocols_;
}

// ****************************************************************

TAO_TCP_Properties::TAO_TCP_Properties (CORBA::Long send_buffer_size,
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

TAO_TCP_Properties::~TAO_TCP_Properties (void)
{
}

CORBA::Long
TAO_TCP_Properties::send_buffer_size (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->send_buffer_size_;
}

void
TAO_TCP_Properties::send_buffer_size (CORBA::Long send_buffer_size
                                      ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->send_buffer_size_ = send_buffer_size;
}

CORBA::Long
TAO_TCP_Properties::recv_buffer_size (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->recv_buffer_size_;
}

void
TAO_TCP_Properties::recv_buffer_size (CORBA::Long recv_buffer_size
                                      ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->recv_buffer_size_ = recv_buffer_size;
}

CORBA::Boolean
TAO_TCP_Properties::keep_alive (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->keep_alive_;
}

void
TAO_TCP_Properties::keep_alive (CORBA::Boolean keep_alive
                                ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->keep_alive_ = keep_alive;
}

CORBA::Boolean
TAO_TCP_Properties::dont_route (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->dont_route_;
}

void
TAO_TCP_Properties::dont_route (CORBA::Boolean dont_route
                                ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->dont_route_ = dont_route;
}

CORBA::Boolean TAO_TCP_Properties::no_delay (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->no_delay_;
}

void
TAO_TCP_Properties::no_delay (CORBA::Boolean no_delay
                              ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->no_delay_ = no_delay;
}

CORBA::Boolean
TAO_TCP_Properties::enable_network_priority (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->enable_network_priority_;
}

void
TAO_TCP_Properties::enable_network_priority (CORBA::Boolean enable
                                             ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->enable_network_priority_ = enable;
}

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
TAO_Unix_Domain_Properties::send_buffer_size (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->send_buffer_size_;
}

void
TAO_Unix_Domain_Properties::send_buffer_size (CORBA::Long send_buffer_size
                                              ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->send_buffer_size_ = send_buffer_size;
}

CORBA::Long
TAO_Unix_Domain_Properties::recv_buffer_size (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->recv_buffer_size_;
}

void
TAO_Unix_Domain_Properties::recv_buffer_size (CORBA::Long recv_buffer_size
                                              ACE_ENV_ARG_DECL_NOT_USED)
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
TAO_SMEM_Properties::preallocate_buffer_size (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->preallocate_buffer_size_;
}

void
TAO_SMEM_Properties::preallocate_buffer_size (CORBA::Long preallocate_buffer_size
                                              ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->preallocate_buffer_size_ = preallocate_buffer_size;
}

char *
TAO_SMEM_Properties::mmap_filename (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->mmap_filename_.rep ();
}

void
TAO_SMEM_Properties::mmap_filename (const char * mmap_filename
                                    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->mmap_filename_.set (mmap_filename);
}

char *
TAO_SMEM_Properties::mmap_lockname (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->mmap_lockname_.rep ();
}

void
TAO_SMEM_Properties::mmap_lockname (const char * mmap_lockname
                                    ACE_ENV_ARG_DECL_NOT_USED)
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

TAO_GIOP_Properties::TAO_GIOP_Properties (void)
{
}

TAO_GIOP_Properties::~TAO_GIOP_Properties (void)
{
}

CORBA::Boolean
TAO_GIOP_Properties::_tao_encode (TAO_OutputCDR &)
{
  return 1;
}

CORBA::Boolean
TAO_GIOP_Properties::_tao_decode (TAO_InputCDR &)
{
  return 1;
}

// ****************************************************************

RTCORBA::ProtocolProperties*
TAO_Protocol_Properties_Factory::create_transport_protocol_property (
  IOP::ProfileId id)
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

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
