#include "RT_PolicyFactory.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

ACE_RCSID (RTCORBA,
           RT_PolicyFactory,
           "$Id$")


#include "tao/PolicyC.h"
#include "RT_Policy_i.h"


CORBA::Policy_ptr
TAO_RT_PolicyFactory::create_policy (
    CORBA::PolicyType type,
    const CORBA::Any &value
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CORBA::PolicyError))
{
  if (type == RTCORBA::PRIORITY_MODEL_POLICY_TYPE)
    return TAO_PriorityModelPolicy::create (value
                                            ACE_ENV_ARG_PARAMETER);

  if (type == RTCORBA::THREADPOOL_POLICY_TYPE)
    return TAO_ThreadpoolPolicy::create (value
                                         ACE_ENV_ARG_PARAMETER);

  if (type == RTCORBA::SERVER_PROTOCOL_POLICY_TYPE)
    return TAO_ServerProtocolPolicy::create (value
                                             ACE_ENV_ARG_PARAMETER);

  if (type == RTCORBA::CLIENT_PROTOCOL_POLICY_TYPE)
    return TAO_ClientProtocolPolicy::create (value
                                             ACE_ENV_ARG_PARAMETER);

  if (type == RTCORBA::PRIVATE_CONNECTION_POLICY_TYPE)
    return TAO_PrivateConnectionPolicy::create (value
                                                ACE_ENV_ARG_PARAMETER);

  if (type == RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE)
    return TAO_PriorityBandedConnectionPolicy::create (value
                                                       ACE_ENV_ARG_PARAMETER);

  ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                    CORBA::Policy::_nil ());
}

CORBA::Policy_ptr
TAO_RT_PolicyFactory::_create_policy (
    CORBA::PolicyType type
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CORBA::PolicyError))
{
  CORBA::Policy_ptr policy = CORBA::Policy::_nil ();

  if (type == RTCORBA::PRIORITY_MODEL_POLICY_TYPE)
    {
      ACE_NEW_THROW_EX (policy,
                        TAO_PriorityModelPolicy,
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());

      return policy;
    }

  if (type == RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE)
    {
      ACE_NEW_THROW_EX (policy,
                        TAO_PriorityBandedConnectionPolicy,
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());

      return policy;
    }

  if (type == RTCORBA::CLIENT_PROTOCOL_POLICY_TYPE)
    {
      ACE_NEW_THROW_EX (policy,
                        TAO_ClientProtocolPolicy,
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());

      return policy;
    }

  ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                    CORBA::Policy::_nil ());
}

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
