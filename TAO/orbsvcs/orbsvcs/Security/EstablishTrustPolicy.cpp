// -*- C++ -*-

#include "EstablishTrustPolicy.h"

ACE_RCSID (Security,
           EstablishTrustPolicy,
           "$Id$")

TAO_EstablishTrustPolicy::TAO_EstablishTrustPolicy (
  const Security::EstablishTrust &trust)
  : trust_ (trust)
{
}

TAO_EstablishTrustPolicy::~TAO_EstablishTrustPolicy (void)
{
}

CORBA::PolicyType
TAO_EstablishTrustPolicy::policy_type (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return Security::SecEstablishTrustPolicy;
}

CORBA::Policy_ptr
TAO_EstablishTrustPolicy::copy (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_EstablishTrustPolicy *policy = 0;
  ACE_NEW_THROW_EX (policy,
                    TAO_EstablishTrustPolicy (this->trust_),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return policy;
}

void
TAO_EstablishTrustPolicy::destroy (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

Security::EstablishTrust
TAO_EstablishTrustPolicy::trust (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->trust_;
}
