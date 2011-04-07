// $Id$

#include "tao/RTCORBA/RT_PolicyFactory.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/RTCORBA/RT_Policy_i.h"
#include "tao/PolicyC.h"
#include "tao/ORB_Constants.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::Policy_ptr
TAO_RT_PolicyFactory::create_policy (
    CORBA::PolicyType type,
    const CORBA::Any &value)
{
  switch (type)
  {
    case RTCORBA::PRIORITY_MODEL_POLICY_TYPE :
      return TAO_PriorityModelPolicy::create (value);
    case RTCORBA::THREADPOOL_POLICY_TYPE :
      return TAO_ThreadpoolPolicy::create (value);
    case RTCORBA::SERVER_PROTOCOL_POLICY_TYPE :
      return TAO_ServerProtocolPolicy::create (value);
    case RTCORBA::CLIENT_PROTOCOL_POLICY_TYPE :
      return TAO_ClientProtocolPolicy::create (value);
    case RTCORBA::PRIVATE_CONNECTION_POLICY_TYPE :
      return TAO_PrivateConnectionPolicy::create (value);
    case RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE :
      return TAO_PriorityBandedConnectionPolicy::create (value);
  }

  throw ::CORBA::PolicyError (CORBA::BAD_POLICY_TYPE);
}

CORBA::Policy_ptr
TAO_RT_PolicyFactory::_create_policy (CORBA::PolicyType type)
{
  CORBA::Policy_ptr policy = CORBA::Policy_ptr ();

  switch (type)
  {
    case RTCORBA::PRIORITY_MODEL_POLICY_TYPE :
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
    break;
    case RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE :
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
    break;
    case RTCORBA::CLIENT_PROTOCOL_POLICY_TYPE :
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
    break;
  }

  throw ::CORBA::PolicyError (CORBA::BAD_POLICY_TYPE);
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
