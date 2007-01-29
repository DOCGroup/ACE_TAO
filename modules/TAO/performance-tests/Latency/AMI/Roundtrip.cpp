//
// $Id$
//
#include "Roundtrip.h"

ACE_RCSID(AMI_Latency, Roundtrip, "$Id$")

Roundtrip::Roundtrip (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

Test::Timestamp
Roundtrip::test_method (Test::Timestamp send_time)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return send_time;
}

void
Roundtrip::shutdown (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0);
}
