// $Id$

#include "Time_i.h"
#include "ace/OS_NS_time.h"

ACE_RCSID(Time, Time_i, "$Id$")

// Constructor
Time_i::Time_i (void)
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
Time_i::current_time (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::Long (ACE_OS::time (0));
}

// Shutdown.

void
Time_i::shutdown (ACE_ENV_SINGLE_ARG_DECL_NOT_USED )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "%s\n",
              "Time_i is shutting down"));

  // Instruct the ORB to shutdown.
  this->orb_->shutdown ();
}

