// -*- C++ -*-

#include "Security_PolicyFactory.h"

ACE_RCSID (Security,
           Security_PolicyFactory,
           "$Id$")

#include "orbsvcs/SecurityLevel2C.h"

#include "QOPPolicy.h"

CORBA::Policy_ptr
TAO_Security_PolicyFactory::create_policy (
    CORBA::PolicyType type,
    const CORBA::Any &value
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CORBA::PolicyError))
{
  TAO_ENV_ARG_DEFN;

  // Not all security policies can be created using the
  // ORB::create_policy() mechanism.  Only those that can be created
  // using that mechanism are supported by this factory.

  if (type == Security::SecQOPPolicy)
    {
      Security::QOP qop;

      // Extract the desired Quality-of-Protection value from the
      // given Any.
      if (!(value >>= qop))
        ACE_THROW_RETURN (CORBA::BAD_PARAM (
                            CORBA::SystemException::_tao_minor_code (
                              TAO_DEFAULT_MINOR_CODE,
                              EINVAL),
                            CORBA::COMPLETED_NO),
                          CORBA::Policy::_nil ());

      TAO_QOPPolicy *qop_policy = 0;
      ACE_NEW_THROW_EX (qop_policy,
                        TAO_QOPPolicy (qop),
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());

      return qop_policy;
    }
  else if (type == Security::SecMechanismsPolicy
           || type == Security::SecInvocationCredentialsPolicy
           || type == Security::SecFeaturePolicy               // Deprecated.
           || type == Security::SecDelegationDirectivePolicy
           || type == Security::SecEstablishTrustPolicy)
    ACE_THROW_RETURN (CORBA::PolicyError (CORBA::UNSUPPORTED_POLICY),
                      CORBA::Policy::_nil ());
  else
    ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                      CORBA::Policy::_nil ());
}
