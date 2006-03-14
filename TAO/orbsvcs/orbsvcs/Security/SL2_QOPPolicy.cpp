// -*- C++ -*-

#include "orbsvcs/Security/SL2_QOPPolicy.h"

#include "tao/ORB_Constants.h"


ACE_RCSID (Security,
           SL2_QOPPolicy,
           "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::Security::QOPPolicy::QOPPolicy (::Security::QOP qop)
  : qop_ (qop)
{
}

TAO::Security::QOPPolicy::~QOPPolicy (void)
{
}

CORBA::PolicyType
TAO::Security::QOPPolicy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return ::Security::SecQOPPolicy;
}

CORBA::Policy_ptr
TAO::Security::QOPPolicy::copy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO::Security::QOPPolicy * policy = 0;
  ACE_NEW_THROW_EX (policy,
                    TAO::Security::QOPPolicy (this->qop_),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return policy;
}

void
TAO::Security::QOPPolicy::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
}

Security::QOP
TAO::Security::QOPPolicy::qop (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->qop_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
