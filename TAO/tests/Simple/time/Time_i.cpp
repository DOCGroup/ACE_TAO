// $Id$

#include "Time_i.h"

ACE_RCSID(Time, Time_i, "$Id$")

// Constructor

Time_i::Time_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

// Destructor

Time_i::~Time_i (void)
{
}

// Return the current date/time on the server.

CORBA::Long
Time_i::time (CORBA::Environment &)
{
  return CORBA::Long (ACE_OS::time (0));
}

// Shutdown.

void Time_i::shutdown (CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG,
              "%s\n",
              "Time_i is shutting down"));

  this->orb_->shutdown ();
}

