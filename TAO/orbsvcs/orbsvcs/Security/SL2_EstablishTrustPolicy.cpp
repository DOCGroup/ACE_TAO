// -*- C++ -*-

#include "orbsvcs/Security/SL2_EstablishTrustPolicy.h"

#include "tao/ORB_Constants.h"


ACE_RCSID (Security,
           SL2_EstablishTrustPolicy,
           "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::Security::EstablishTrustPolicy::EstablishTrustPolicy (
  const ::Security::EstablishTrust &trust)
  : trust_ (trust)
{
}

TAO::Security::EstablishTrustPolicy::~EstablishTrustPolicy (void)
{
}

CORBA::PolicyType
TAO::Security::EstablishTrustPolicy::policy_type (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return ::Security::SecEstablishTrustPolicy;
}

CORBA::Policy_ptr
TAO::Security::EstablishTrustPolicy::copy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO::Security::EstablishTrustPolicy *policy = 0;
  ACE_NEW_THROW_EX (policy,
                    TAO::Security::EstablishTrustPolicy (this->trust_),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return policy;
}

void
TAO::Security::EstablishTrustPolicy::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

::Security::EstablishTrust
TAO::Security::EstablishTrustPolicy::trust (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->trust_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
