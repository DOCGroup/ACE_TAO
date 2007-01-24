// $Id$

#include "orbsvcs/Security/SL3_ObjectCredentialsPolicy.h"


ACE_RCSID (Security,
           SL3_ObjectCredentialsPolicy,
           "$Id$")


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
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  SecurityLevel3::OwnCredentialsList * creds = 0;

  ACE_NEW_THROW_EX (creds,
                    SecurityLevel3::OwnCredentialsList (this->creds_list_),
                    CORBA::NO_MEMORY ());

  return creds;
}

CORBA::PolicyType
TAO::SL3::ObjectCredentialsPolicy::policy_type (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return SecurityLevel3::ObjectCredentialsPolicyType;
}

CORBA::Policy_ptr
TAO::SL3::ObjectCredentialsPolicy::copy (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Policy_ptr p = CORBA::Policy::_nil ();
  ACE_NEW_THROW_EX (p,
                    TAO::SL3::ObjectCredentialsPolicy (this->creds_list_),
                    CORBA::NO_MEMORY ());

  return p;
}

void
TAO::SL3::ObjectCredentialsPolicy::destroy (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->creds_list_.length (0);
}

TAO_END_VERSIONED_NAMESPACE_DECL
