// $Id$

#include "AccountManager_i.h"
#include "Bank_i.h"

ACE_RCSID(AccountManager, AccountManager_i, "$Id$")

// Constructor

AccountManager_i::AccountManager_i (void)
{
  // no-op
}

// Destructor

AccountManager_i::~AccountManager_i (void)
{
  // no-op
}

// Set the ORB pointer.

void
AccountManager_i::orb (CORBA::ORB_ptr o)
{
  this->orb_ = CORBA::ORB::_duplicate (o);
}

// Open an account for the given name

Bank::Account_ptr
AccountManager_i::open (const char * name,CORBA::Environment &env)
{
  ACE_DEBUG ((LM_DEBUG,
              "%s %s\n",
              "Opening an Account for",name));
  Bank_i* bank;
  ACE_NEW_RETURN (bank, Bank_i, Bank::Account::_nil ());
  return bank->_this (env);
}

// Shutdown.

void AccountManager_i::shutdown (CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG,
              "%s\n",
              "AccountManager_i is shutting down"));

  // Instruct the ORB to shutdown.
  this->orb_->shutdown ();
}
