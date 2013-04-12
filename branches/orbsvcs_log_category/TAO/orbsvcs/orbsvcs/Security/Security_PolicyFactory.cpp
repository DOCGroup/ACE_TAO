// -*- C++ -*-
// $Id$

#include "orbsvcs/Security/Security_PolicyFactory.h"
#include "orbsvcs/Security/SL2_QOPPolicy.h"
#include "orbsvcs/Security/SL2_EstablishTrustPolicy.h"

#include "orbsvcs/Security/SL3_ContextEstablishmentPolicy.h"
#include "orbsvcs/Security/SL3_ObjectCredentialsPolicy.h"

#include "orbsvcs/SecurityLevel2C.h"
#include "orbsvcs/SecurityLevel3C.h"

#include "tao/ORB_Constants.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::Policy_ptr
TAO::Security::PolicyFactory::create_policy (
    CORBA::PolicyType type,
    const CORBA::Any &value)
{
  // Not all security policies can be created using the
  // ORB::create_policy() mechanism.  Only those that can be created
  // using that mechanism are supported by this factory.

  if (type == ::Security::SecQOPPolicy)
    {
      ::Security::QOP qop;

      // Extract the desired Quality-of-Protection value from the
      // given Any.
      if (!(value >>= qop))
        throw CORBA::BAD_PARAM (
          CORBA::SystemException::_tao_minor_code (
            TAO::VMCID,
            EINVAL),
          CORBA::COMPLETED_NO);

      TAO::Security::QOPPolicy * qop_policy = 0;
      ACE_NEW_THROW_EX (qop_policy,
                        TAO::Security::QOPPolicy (qop),
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));

      return qop_policy;
    }

  else if (type == ::Security::SecEstablishTrustPolicy)
    {
      ::Security::EstablishTrust *trust = 0;

      // Extract the desired establishing of trust value from the
      // given Any.
      if (!(value >>= trust))
        throw CORBA::BAD_PARAM (
          CORBA::SystemException::_tao_minor_code (
            TAO::VMCID,
            EINVAL),
          CORBA::COMPLETED_NO);

      TAO::Security::EstablishTrustPolicy * trust_policy = 0;
      ACE_NEW_THROW_EX (trust_policy,
                        TAO::Security::EstablishTrustPolicy (*trust),
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));

      return trust_policy;
    }

  else if (type == SecurityLevel3::ContextEstablishmentPolicyType)
    {
      SecurityLevel3::ContextEstablishmentPolicyArgument * args = 0;

      // Extract the desired establishing of trust value from the
      // given Any.
      if (!(value >>= args))
        throw CORBA::BAD_PARAM (
          CORBA::SystemException::_tao_minor_code (
            TAO::VMCID,
            EINVAL),
          CORBA::COMPLETED_NO);

      TAO::SL3::ContextEstablishmentPolicy * policy = 0;
      ACE_NEW_THROW_EX (policy,
                        TAO::SL3::ContextEstablishmentPolicy (
                          args->creds_directive,
                          args->creds_list,
                          args->use_client_auth,
                          args->use_target_auth,
                          args->use_confidentiality,
                          args->use_integrity),
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));

      return policy;
    }

  else if (type == SecurityLevel3::ObjectCredentialsPolicyType)
    {
      SecurityLevel3::OwnCredentialsList * creds = 0;

      // Extract the desired establishing of trust value from the
      // given Any.
      if (!(value >>= creds))
        throw CORBA::BAD_PARAM (
          CORBA::SystemException::_tao_minor_code (
            TAO::VMCID,
            EINVAL),
          CORBA::COMPLETED_NO);

      TAO::SL3::ObjectCredentialsPolicy * policy = 0;
      ACE_NEW_THROW_EX (policy,
                        TAO::SL3::ObjectCredentialsPolicy (*creds),
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));

      return policy;
    }

  else if (type == ::Security::SecInvocationCredentialsPolicy
           || type == ::Security::SecMechanismsPolicy
           || type == ::Security::SecFeaturePolicy             // Deprecated.
           || type == ::Security::SecDelegationDirectivePolicy)
    throw CORBA::PolicyError (CORBA::UNSUPPORTED_POLICY);
  else
    throw CORBA::PolicyError (CORBA::BAD_POLICY_TYPE);
}

TAO_END_VERSIONED_NAMESPACE_DECL
