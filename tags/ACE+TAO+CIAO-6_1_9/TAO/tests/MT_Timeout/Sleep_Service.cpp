//
// $Id$
//
#include "Sleep_Service.h"
#include "ace/OS_NS_unistd.h"

Sleep_Service::Sleep_Service (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Sleep_Service::go_to_sleep (CORBA::ULong microseconds)
{
  CORBA::ULong  secs = microseconds / 1000000;
  CORBA::ULong usecs = microseconds % 1000000;
  ACE_Time_Value tv (secs, usecs);

  ACE_OS::sleep (tv);
}

void
Sleep_Service::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG, "Shutting down the ORB\n"));
  this->orb_->shutdown (0);
}
