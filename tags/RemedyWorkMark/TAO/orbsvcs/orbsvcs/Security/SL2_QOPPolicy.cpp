// -*- C++ -*-
// $Id$

#include "orbsvcs/Security/SL2_QOPPolicy.h"
#include "tao/ORB_Constants.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::Security::QOPPolicy::QOPPolicy (::Security::QOP qop)
  : qop_ (qop)
{
}

TAO::Security::QOPPolicy::~QOPPolicy (void)
{
}

CORBA::PolicyType
TAO::Security::QOPPolicy::policy_type (void)
{
  return ::Security::SecQOPPolicy;
}

CORBA::Policy_ptr
TAO::Security::QOPPolicy::copy (void)
{
  TAO::Security::QOPPolicy * policy = 0;
  ACE_NEW_THROW_EX (policy,
                    TAO::Security::QOPPolicy (this->qop_),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  return policy;
}

void
TAO::Security::QOPPolicy::destroy (void)
{
}

Security::QOP
TAO::Security::QOPPolicy::qop (void)
{
  return this->qop_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
