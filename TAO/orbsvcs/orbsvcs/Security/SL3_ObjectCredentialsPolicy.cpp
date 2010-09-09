// $Id$

#include "orbsvcs/Security/SL3_ObjectCredentialsPolicy.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::SL3::ObjectCredentialsPolicy::ObjectCredentialsPolicy (
  const SecurityLevel3::OwnCredentialsList & creds)
  : creds_list_ (creds)
{
}

TAO::SL3::ObjectCredentialsPolicy::~ObjectCredentialsPolicy (void)
{
}

SecurityLevel3::OwnCredentialsList *
TAO::SL3::ObjectCredentialsPolicy::creds_list (void)
{
  SecurityLevel3::OwnCredentialsList * creds = 0;

  ACE_NEW_THROW_EX (creds,
                    SecurityLevel3::OwnCredentialsList (this->creds_list_),
                    CORBA::NO_MEMORY ());

  return creds;
}

CORBA::PolicyType
TAO::SL3::ObjectCredentialsPolicy::policy_type ()
{
  return SecurityLevel3::ObjectCredentialsPolicyType;
}

CORBA::Policy_ptr
TAO::SL3::ObjectCredentialsPolicy::copy (void)
{
  CORBA::Policy_ptr p = CORBA::Policy_ptr ();
  ACE_NEW_THROW_EX (p,
                    TAO::SL3::ObjectCredentialsPolicy (this->creds_list_),
                    CORBA::NO_MEMORY ());

  return p;
}

void
TAO::SL3::ObjectCredentialsPolicy::destroy (void)
{
  this->creds_list_.length (0);
}

TAO_END_VERSIONED_NAMESPACE_DECL
