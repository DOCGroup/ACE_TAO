// -*- C++ -*-

#include "InvocationCredentialsPolicy.h"


ACE_RCSID (Security,
           InvocationCredentialsPolicy,
           "$Id$")


TAO_InvocationCredentialsPolicy::TAO_InvocationCredentialsPolicy (
  const SecurityLevel2::CredentialsList &creds)
  : creds_ (creds)
{
}

TAO_InvocationCredentialsPolicy::~TAO_InvocationCredentialsPolicy (void)
{
}

CORBA::PolicyType
TAO_InvocationCredentialsPolicy::policy_type (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return Security::SecInvocationCredentialsPolicy;
}

CORBA::Policy_ptr
TAO_InvocationCredentialsPolicy::copy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_InvocationCredentialsPolicy *policy = 0;
  ACE_NEW_THROW_EX (policy,
                    TAO_InvocationCredentialsPolicy (this->creds_),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return policy;
}

void
TAO_InvocationCredentialsPolicy::destroy (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

SecurityLevel2::CredentialsList *
TAO_InvocationCredentialsPolicy::creds (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  SecurityLevel2::CredentialsList *creds = 0;
  ACE_NEW_THROW_EX (creds,
                    SecurityLevel2::CredentialsList (this->creds_),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (0);

  return creds;
}
