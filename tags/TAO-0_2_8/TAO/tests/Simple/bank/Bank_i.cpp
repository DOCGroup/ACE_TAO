// $Id$

#include "Bank_i.h"

ACE_RCSID(Bank, Bank_i, "$Id$")

// Constructor

Bank_i::Bank_i (void)
{
  // no-op
}

// Destructor

Bank_i::~Bank_i (void)
{
  // no-op
}

// Set the ORB pointer.

void
Bank_i::orb (CORBA::ORB_ptr o)
{
  this->orb_ = CORBA::ORB::_duplicate (o);
}

// Return the current balance on the server.

CORBA::Float
Bank_i::balance (CORBA::Environment &)
{
  return CORBA::Float (100);
}

// Shutdown.

void Bank_i::shutdown (CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG,
              "%s\n",
              "Bank_i is shutting down"));

  // Instruct the ORB to shutdown.
  this->orb_->shutdown ();
}
