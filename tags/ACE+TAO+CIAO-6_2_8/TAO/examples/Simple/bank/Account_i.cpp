// $Id$

#include "Account_i.h"

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
}

// Set the ORB pointer.
void
Account_i::orb (CORBA::ORB_ptr o)
{
  this->orb_ = CORBA::ORB::_duplicate (o);
}

// Return the current balance on the server.
CORBA::Float
Account_i::balance (void)
{
  return balance_;
}

void
Account_i::deposit (CORBA::Float deposit)
{
  balance_ += deposit;
}

void
Account_i::withdraw (CORBA::Float withdrawl)
{
  if (balance_ >= withdrawl)
    balance_ -= withdrawl;
  else
    throw Bank::Account::Overdraft ("Exception::Overdraft\n");
}

char *
Account_i::name (void)
{
  return CORBA::string_dup (this->name_.in ());
}

void
Account_i::name (const char *name)
{
  this->name_ = CORBA::string_dup (name);
}
