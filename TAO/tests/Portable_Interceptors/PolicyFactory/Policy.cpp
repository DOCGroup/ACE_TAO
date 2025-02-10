#include "Policy.h"

#include "tao/SystemException.h"
#include "ace/Log_Msg.h"

Policy::Policy (CORBA::ULong val)
  : value_ (val)
{
}

Policy::~Policy ()
{
}

CORBA::ULong
Policy::value ()
{
  return this->value_;
}

CORBA::PolicyType
Policy::policy_type ()
{
  return Test::POLICY_TYPE;
}

CORBA::Policy_ptr
Policy::copy ()
{
  CORBA::Policy_ptr p;
  ACE_NEW_THROW_EX (p,
                    Policy (this->value_),
                    CORBA::NO_MEMORY ());

  return p;
}

void
Policy::destroy ()
{
}
