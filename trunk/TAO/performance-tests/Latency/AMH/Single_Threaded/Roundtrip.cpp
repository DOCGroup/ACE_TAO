//
// $Id$
//
#include "Roundtrip.h"

ACE_RCSID(Single_Threaded_Latency, Roundtrip, "$Id$")

Roundtrip::Roundtrip (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Roundtrip::test_method (
                Test::AMH_RoundtripResponseHandler_ptr _tao_rh,
                Test::Timestamp send_time
                TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  //ACE_DEBUG ((LM_DEBUG, "Test_Method called\n"));
  _tao_rh->test_method (send_time);
  //ACE_DEBUG ((LM_DEBUG, "RH completed\n"));
}

void
Roundtrip::shutdown (
       Test::AMH_RoundtripResponseHandler_ptr _tao_rh
       TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  _tao_rh->shutdown ();
  this->orb_->shutdown (0 TAO_ENV_ARG_PARAMETER);
}
