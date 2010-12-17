//
// $Id$
//
#include "Blocking_Sync_None.h"
#include "ace/OS_NS_unistd.h"

Blocking_Sync_None::Blocking_Sync_None (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Blocking_Sync_None::slow_operation (const Test::Payload &,
                                    CORBA::ULong sleep_microseconds)
{
  ACE_Time_Value sleep_time (0, sleep_microseconds);
  ACE_OS::sleep (sleep_time);
}

void
Blocking_Sync_None::shutdown (void)
{
  this->orb_->shutdown (0);
}
