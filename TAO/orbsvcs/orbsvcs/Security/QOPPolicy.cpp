// -*- C++ -*-

#include "QOPPolicy.h"

ACE_RCSID (Security,
           QOPPolicy,
           "$Id$")

TAO_QOPPolicy::TAO_QOPPolicy (Security::QOP qop)
  : qop_ (qop)
{
}

TAO_QOPPolicy::~TAO_QOPPolicy (void)
{
}

CORBA::PolicyType
TAO_QOPPolicy::policy_type (CORBA::Environment & /* ACE_TRY_ENV */)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return Security::SecQOPPolicy;
}

CORBA::Policy_ptr
TAO_QOPPolicy::copy (CORBA::Environment & /* ACE_TRY_ENV */)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::Policy::_duplicate (this);
}

void
TAO_QOPPolicy::destroy (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
}

Security::QOP
TAO_QOPPolicy::qop (CORBA::Environment & /* ACE_TRY_ENV */)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->qop_;
}
