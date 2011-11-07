//
// $Id$
//
#include "Roundtrip.h"

Roundtrip::Roundtrip (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Roundtrip::test_method (
                Test::AMH_RoundtripResponseHandler_ptr _tao_rh,
                Test::Timestamp send_time)
{
  //ACE_DEBUG ((LM_DEBUG, "Test_Method called\n"));
  _tao_rh->test_method (send_time);
  //ACE_DEBUG ((LM_DEBUG, "RH completed\n"));
}

void
Roundtrip::shutdown (
       Test::AMH_RoundtripResponseHandler_ptr _tao_rh)
{
  _tao_rh->shutdown ();
  this->orb_->shutdown (0);
}
