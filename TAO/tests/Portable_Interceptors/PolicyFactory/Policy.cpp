// $Id$

#include "Policy.h"

#include "ace/Log_Msg.h"

ACE_RCSID (PolicyFactory,
           Policy,
           "$Id$")


Policy::Policy (CORBA::ULong val)
  : value_ (val)
{
}

Policy::~Policy (void)
{
}

CORBA::ULong
Policy::value (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->value_;
}

CORBA::PolicyType
Policy::policy_type (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return Test::POLICY_TYPE;
}

CORBA::Policy_ptr
Policy::copy (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Policy_ptr p;
  ACE_NEW_THROW_EX (p,
                    Policy (this->value_),
                    CORBA::NO_MEMORY ());

  return p;
}

void
Policy::destroy (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}
