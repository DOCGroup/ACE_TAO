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
Account_i::balance (CORBA::Environment &)
{
  return balance_;
}

void
Account_i::deposit (CORBA::Float deposit,
		    CORBA::Environment &)
{
  balance_ += deposit;
}

void
Account_i::withdraw (CORBA::Float withdrawl,
		     CORBA::Environment &env)
{
  if (balance_ >= withdrawl)
    balance_ -= withdrawl;
  else
    env.exception (new Bank::Account::Overdraft ("Exception::Overdraft\n"));
}

char *
Account_i::name (CORBA::Environment &)
{
  return CORBA::string_dup (name_);
}

void
Account_i::name (const char *name,
		 CORBA::Environment &)
{
  name_ = CORBA::string_dup (name);
}
