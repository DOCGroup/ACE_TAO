// -*- C++ -*-
#include "orbsvcs/Security/SL2_EstablishTrustPolicy.h"
#include "tao/ORB_Constants.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::Security::EstablishTrustPolicy::EstablishTrustPolicy (
  ::Security::EstablishTrust const & trust)
  : trust_ (trust)
{
}

CORBA::PolicyType
TAO::Security::EstablishTrustPolicy::policy_type ()
{
  return ::Security::SecEstablishTrustPolicy;
}

CORBA::Policy_ptr
TAO::Security::EstablishTrustPolicy::copy ()
{
  TAO::Security::EstablishTrustPolicy *policy = nullptr;
  ACE_NEW_THROW_EX (policy,
                    TAO::Security::EstablishTrustPolicy (this->trust_),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  return policy;
}

void
TAO::Security::EstablishTrustPolicy::destroy ()
{
}

::Security::EstablishTrust
TAO::Security::EstablishTrustPolicy::trust ()
{
  return this->trust_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
