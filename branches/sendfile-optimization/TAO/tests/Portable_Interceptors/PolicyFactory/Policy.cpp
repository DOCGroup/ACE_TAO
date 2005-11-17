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
Policy::value (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->value_;
}

CORBA::PolicyType
Policy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return Test::POLICY_TYPE;
}

CORBA::Policy_ptr
Policy::copy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Policy_ptr p;
  ACE_NEW_THROW_EX (p,
                    Policy (this->value_),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return p;
}

void
Policy::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}
