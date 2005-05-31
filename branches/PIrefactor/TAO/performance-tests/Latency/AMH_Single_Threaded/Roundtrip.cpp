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
                ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  //ACE_DEBUG ((LM_DEBUG, "Test_Method called\n"));
  ACE_DECLARE_NEW_CORBA_ENV;
  _tao_rh->test_method (send_time ACE_ENV_ARG_PARAMETER);
  //ACE_DEBUG ((LM_DEBUG, "RH completed\n"));
}

void
Roundtrip::shutdown (
       Test::AMH_RoundtripResponseHandler_ptr _tao_rh
       ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DECLARE_NEW_CORBA_ENV;
  _tao_rh->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}
