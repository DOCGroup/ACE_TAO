// $Id$

#include "orbsvcs/Security/SL3_PolicyFactory.h"
#include "orbsvcs/Security/SL3_ContextEstablishmentPolicy.h"
#include "orbsvcs/Security/SL3_ObjectCredentialsPolicy.h"


ACE_RCSID (Security,
           SL3_PolicyFactory,
           "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::Policy_ptr
TAO::SL3::PolicyFactory::create_policy (CORBA::PolicyType type,
                                        const CORBA::Any & value
                                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CORBA::PolicyError))
{
  CORBA::Policy_ptr policy = CORBA::Policy::_nil ();

  if (type == SecurityLevel3::ContextEstablishmentPolicyType)
    {
      SecurityLevel3::ContextEstablishmentPolicyArgument * arg = 0;
      if (!(value >>= arg))
        ACE_THROW_RETURN (CORBA::INTERNAL (),
                          policy);

      ACE_NEW_THROW_EX (policy,
                        TAO::SL3::ContextEstablishmentPolicy (
                          arg->creds_directive,
                          arg->creds_list,
                          arg->use_client_auth,
                          arg->use_target_auth,
                          arg->use_confidentiality,
                          arg->use_integrity),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (policy);
    }
  else if (type == SecurityLevel3::ObjectCredentialsPolicyType)
    {
      SecurityLevel3::ObjectCredentialsPolicyArgument * creds = 0;
      if (!(value >>= creds))
        ACE_THROW_RETURN (CORBA::INTERNAL (),
                          policy);

      ACE_NEW_THROW_EX (policy,
                        TAO::SL3::ObjectCredentialsPolicy (*creds),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (policy);
    }
  else
    {
      ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                        policy);
    }

  return policy;
}

TAO_END_VERSIONED_NAMESPACE_DECL
