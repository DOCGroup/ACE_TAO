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
Roundtrip::test_octet_method (const Test::octet_load &,
                              Test::Timestamp send_time
                              ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return send_time;
}

Test::Timestamp
Roundtrip::test_long_method (const Test::long_load &,
                              Test::Timestamp send_time
                              ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return send_time;
}

Test::Timestamp
Roundtrip::test_char_method (const Test::char_load &,
                             Test::Timestamp send_time
                             ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return send_time;
}

Test::Timestamp
Roundtrip::test_longlong_method (const Test::longlong_load &,
                                 Test::Timestamp send_time
                                 ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return send_time;
}

Test::Timestamp
Roundtrip::test_double_method (const Test::double_load &,
                               Test::Timestamp send_time
                               ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return send_time;
}


Test::Timestamp
Roundtrip::test_short_method (const Test::short_load &,
                              Test::Timestamp send_time
                              ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return send_time;
}

void
Roundtrip::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}
