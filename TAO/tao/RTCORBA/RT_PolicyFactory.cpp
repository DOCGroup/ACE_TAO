#include "tao/RTCORBA/RT_PolicyFactory.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

ACE_RCSID (RTCORBA,
           RT_PolicyFactory,
           "$Id$")

#include "tao/RTCORBA/RT_Policy_i.h"

#include "tao/PolicyC.h"
#include "tao/ORB_Constants.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::Policy_ptr
TAO_RT_PolicyFactory::create_policy (
    CORBA::PolicyType type,
    const CORBA::Any &value)
{
  if (type == RTCORBA::PRIORITY_MODEL_POLICY_TYPE)
    return TAO_PriorityModelPolicy::create (value);

  if (type == RTCORBA::THREADPOOL_POLICY_TYPE)
    return TAO_ThreadpoolPolicy::create (value);

  if (type == RTCORBA::SERVER_PROTOCOL_POLICY_TYPE)
    return TAO_ServerProtocolPolicy::create (value);

  if (type == RTCORBA::CLIENT_PROTOCOL_POLICY_TYPE)
    return TAO_ClientProtocolPolicy::create (value);

  if (type == RTCORBA::PRIVATE_CONNECTION_POLICY_TYPE)
    return TAO_PrivateConnectionPolicy::create (value);

  if (type == RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE)
    return TAO_PriorityBandedConnectionPolicy::create (value);

  throw ::CORBA::PolicyError (CORBA::BAD_POLICY_TYPE);
}

CORBA::Policy_ptr
TAO_RT_PolicyFactory::_create_policy (CORBA::PolicyType type)
{
  CORBA::Policy_ptr policy = CORBA::Policy_ptr ();

  if (type == RTCORBA::PRIORITY_MODEL_POLICY_TYPE)
    {
      ACE_NEW_THROW_EX (policy,
                        TAO_PriorityModelPolicy,
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));

      return policy;
    }

  if (type == RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE)
    {
      ACE_NEW_THROW_EX (policy,
                        TAO_PriorityBandedConnectionPolicy,
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));

      return policy;
    }

  if (type == RTCORBA::CLIENT_PROTOCOL_POLICY_TYPE)
    {
      ACE_NEW_THROW_EX (policy,
                        TAO_ClientProtocolPolicy,
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));

      return policy;
    }

  throw ::CORBA::PolicyError (CORBA::BAD_POLICY_TYPE);
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
