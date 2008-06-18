//
// $Id$
//
#include "Roundtrip.h"

ACE_RCSID(Single_Threaded_Latency, Roundtrip, "$Id$")

Roundtrip::Roundtrip (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

Test::Timestamp
Roundtrip::test_method (Test::Timestamp send_time)
{
  return send_time;
}

void
Roundtrip::shutdown (void)
{
  this->orb_->shutdown (0);
}
