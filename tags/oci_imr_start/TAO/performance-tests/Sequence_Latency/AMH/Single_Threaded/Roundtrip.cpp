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
Roundtrip::test_octet_method (
                Test::AMH_RoundtripResponseHandler_ptr _tao_rh,
                const Test::octet_load &,
                Test::Timestamp send_time
                ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  //ACE_DEBUG ((LM_DEBUG, "Test_Octet_Method called\n"));
  ACE_DECLARE_NEW_CORBA_ENV;
  _tao_rh->test_octet_method (send_time ACE_ENV_ARG_PARAMETER);
  //ACE_DEBUG ((LM_DEBUG, "RH completed\n"));
}

void
Roundtrip::test_long_method (
                Test::AMH_RoundtripResponseHandler_ptr _tao_rh,
                const Test::long_load &,
                Test::Timestamp send_time
                ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  //ACE_DEBUG ((LM_DEBUG, "Test_Long_Method called\n"));
  ACE_DECLARE_NEW_CORBA_ENV;
  _tao_rh->test_long_method (send_time ACE_ENV_ARG_PARAMETER);
  //ACE_DEBUG ((LM_DEBUG, "RH completed\n"));
}

void
Roundtrip::test_short_method (
                Test::AMH_RoundtripResponseHandler_ptr _tao_rh,
                const Test::short_load &,
                Test::Timestamp send_time
                ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  //ACE_DEBUG ((LM_DEBUG, "Test_Short_Method called\n"));
  ACE_DECLARE_NEW_CORBA_ENV;
  _tao_rh->test_short_method (send_time ACE_ENV_ARG_PARAMETER);
  //ACE_DEBUG ((LM_DEBUG, "RH completed\n"));
}

void
Roundtrip::test_char_method (
                Test::AMH_RoundtripResponseHandler_ptr _tao_rh,
                const Test::char_load &,
                Test::Timestamp send_time
                ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  //ACE_DEBUG ((LM_DEBUG, "Test_Char_Method called\n"));
  ACE_DECLARE_NEW_CORBA_ENV;
  _tao_rh->test_char_method (send_time ACE_ENV_ARG_PARAMETER);
  //ACE_DEBUG ((LM_DEBUG, "RH completed\n"));
}


void
Roundtrip::test_longlong_method (
                Test::AMH_RoundtripResponseHandler_ptr _tao_rh,
                const Test::longlong_load &,
                Test::Timestamp send_time
                ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  //ACE_DEBUG ((LM_DEBUG, "Test_Longlong_Method called\n"));
  ACE_DECLARE_NEW_CORBA_ENV;
  _tao_rh->test_longlong_method (send_time ACE_ENV_ARG_PARAMETER);
  //ACE_DEBUG ((LM_DEBUG, "RH completed\n"));
}

void
Roundtrip::test_double_method (
                Test::AMH_RoundtripResponseHandler_ptr _tao_rh,
                const Test::double_load &,
                Test::Timestamp send_time
                ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  //ACE_DEBUG ((LM_DEBUG, "Test_Double_Method called\n"));
  ACE_DECLARE_NEW_CORBA_ENV;
  _tao_rh->test_double_method (send_time ACE_ENV_ARG_PARAMETER);
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
