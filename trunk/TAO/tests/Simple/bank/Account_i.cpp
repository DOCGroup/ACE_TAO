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

  : name_ (CORBA::string_dup (name)),
    balance_ (balance)
{
}

// Destructor

Account_i::~Account_i (void)
{
  // @@ Make sure that you do a CORBA::string_free() on name_ when
  // this exits.

  // @@ Make sure that you run Purify on your program when you've got
  // it working.
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
		    CORBA::Environment &env)
{
  balance_ = balance_ + deposit ;
}

void
Account_i::withdraw (CORBA::Float withdrawl,
		     CORBA::Environment &env)
{
  // @@ Here's where you'd handle the exception...
  if (balance_ >= withdrawl)
    balance_ = balance_ - withdrawl;
  else
    env.exception (new Bank::Account::Overdraft("Exception from server::Overdraft"));
}

char *
Account_i::name (CORBA::Environment &env)
{
  return CORBA::string_dup (name_);
}

void
Account_i::name (const char *name,
		 CORBA::Environment &env)
{
  name_ = CORBA::string_dup (name);
}
