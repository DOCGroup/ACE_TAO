//
// $Id$
//
#include "Roundtrip.h"

Roundtrip::Roundtrip (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

Test::Timestamp
Roundtrip::test_octet_method (const Test::octet_load &,
                              Test::Timestamp send_time)
{
  return send_time;
}

Test::Timestamp
Roundtrip::test_long_method (const Test::long_load &,
                              Test::Timestamp send_time)
{
  return send_time;
}

Test::Timestamp
Roundtrip::test_char_method (const Test::char_load &,
                             Test::Timestamp send_time)
{
  return send_time;
}

Test::Timestamp
Roundtrip::test_longlong_method (const Test::longlong_load &,
                                 Test::Timestamp send_time)
{
  return send_time;
}

Test::Timestamp
Roundtrip::test_double_method (const Test::double_load &,
                               Test::Timestamp send_time)
{
  return send_time;
}


Test::Timestamp
Roundtrip::test_short_method (const Test::short_load &,
                              Test::Timestamp send_time)
{
  return send_time;
}

void
Roundtrip::shutdown (void)
{
  this->orb_->shutdown (0);
}
