// $Id$

#include "Policy.h"

#include "tao/SystemException.h"
#include "ace/Log_Msg.h"

Policy::Policy (CORBA::ULong val)
  : value_ (val)
{
}

Policy::~Policy (void)
{
}

CORBA::ULong
Policy::value (void)
{
  return this->value_;
}

CORBA::PolicyType
Policy::policy_type (void)
{
  return Test::POLICY_TYPE;
}

CORBA::Policy_ptr
Policy::copy (void)
{
  CORBA::Policy_ptr p;
  ACE_NEW_THROW_EX (p,
                    Policy (this->value_),
                    CORBA::NO_MEMORY ());

  return p;
}

void
Policy::destroy (void)
{
}
