// $Id$

#include "Time_i.h"

ACE_RCSID(Time, Time_i, "$Id$")

// Constructor
Time_i::Time_i (void)
{
  // no-op
}

// Copy  Constructor
Time_i::Time_i (Time_i & /* tim */)
{
  // no-op
}

// Destructor

Time_i::~Time_i (void)
{
  // no-op
}

// Set the ORB pointer.

void
Time_i::orb (CORBA::ORB_ptr o)
{
  this->orb_ = CORBA::ORB::_duplicate (o);
}

// Return the current date/time on the server.

CORBA::Long
Time_i::time (CORBA::Environment &)
{
  return CORBA::Long (ACE_OS::time (0));
}

// Shutdown.

void 
Time_i::shutdown (CORBA::Environment & )
{
  ACE_DEBUG ((LM_DEBUG,
              "%s\n",
              "Time_i is shutting down"));

  // Instruct the ORB to shutdown.
  this->orb_->shutdown ();
}

