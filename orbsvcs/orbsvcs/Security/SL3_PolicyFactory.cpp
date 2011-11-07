// $Id$

#include "orbsvcs/Security/SL3_PolicyFactory.h"
#include "orbsvcs/Security/SL3_ContextEstablishmentPolicy.h"
#include "orbsvcs/Security/SL3_ObjectCredentialsPolicy.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::Policy_ptr
TAO::SL3::PolicyFactory::create_policy (CORBA::PolicyType type,
                                        const CORBA::Any & value)
{
  CORBA::Policy_ptr policy = CORBA::Policy_ptr ();

  if (type == SecurityLevel3::ContextEstablishmentPolicyType)
    {
      SecurityLevel3::ContextEstablishmentPolicyArgument * arg = 0;
      if (!(value >>= arg))
        throw CORBA::INTERNAL ();

      ACE_NEW_THROW_EX (policy,
                        TAO::SL3::ContextEstablishmentPolicy (
                          arg->creds_directive,
                          arg->creds_list,
                          arg->use_client_auth,
                          arg->use_target_auth,
                          arg->use_confidentiality,
                          arg->use_integrity),
                        CORBA::NO_MEMORY ());
    }
  else if (type == SecurityLevel3::ObjectCredentialsPolicyType)
    {
      SecurityLevel3::ObjectCredentialsPolicyArgument * creds = 0;
      if (!(value >>= creds))
        throw CORBA::INTERNAL ();

      ACE_NEW_THROW_EX (policy,
                        TAO::SL3::ObjectCredentialsPolicy (*creds),
                        CORBA::NO_MEMORY ());
    }
  else
    {
      throw CORBA::PolicyError (CORBA::BAD_POLICY_TYPE);
    }

  return policy;
}

TAO_END_VERSIONED_NAMESPACE_DECL
