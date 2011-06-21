// $Id$

#include "Time_i.h"
#include "ace/OS_NS_time.h"

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
Time_i::current_time (void)
{
  return CORBA::Long (ACE_OS::time (0));
}

// Shutdown.
void
Time_i::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Time_i is shutting down\n")));

  // Instruct the ORB to shutdown.
  this->orb_->shutdown ();
}
