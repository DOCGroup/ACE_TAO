// $Id$

#include "Account_i.h"

ACE_RCSID(Bank, Account_i, "$Id$")

// Constructor

Account_i::Account_i (void)
{
  // no-op
}

Account_i::Account_i (const char *name,
                      CORBA::Float balance)

  : balance_ (balance),
    name_ (CORBA::string_dup (name))
{
}

// Destructor

Account_i::~Account_i (void)
{
  CORBA::string_free (name_);
}

// Set the ORB pointer.

void
Account_i::orb (CORBA::ORB_ptr o)
{
  this->orb_ = CORBA::ORB::_duplicate (o);
}

// Return the current balance on the server.

CORBA::Float
Account_i::balance (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return balance_;
}

void
Account_i::deposit (CORBA::Float deposit
                    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  balance_ += deposit;
}

void
Account_i::withdraw (CORBA::Float withdrawl
                     TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Bank::Account::Overdraft))
{
  if (balance_ >= withdrawl)
    balance_ -= withdrawl;
  else
    ACE_THROW (Bank::Account::Overdraft ("Exception::Overdraft\n"));
}

char *
Account_i::name (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->name_.in ());
}

void
Account_i::name (const char *name
                 TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->name_ = CORBA::string_dup (name);
}
