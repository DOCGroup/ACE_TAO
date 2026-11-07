#include "Roundtrip.h"

Roundtrip::Roundtrip (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

Test::Timestamp
Roundtrip::test_octet_method (Test::octet_load const &,
                              Test::Timestamp send_time)
{
  return send_time;
}

Test::Timestamp
Roundtrip::test_long_method (Test::long_load const &,
                              Test::Timestamp send_time)
{
  return send_time;
}

Test::Timestamp
Roundtrip::test_char_method (Test::char_load const &,
                             Test::Timestamp send_time)
{
  return send_time;
}

Test::Timestamp
Roundtrip::test_longlong_method (Test::longlong_load const &,
                                 Test::Timestamp send_time)
{
  return send_time;
}

Test::Timestamp
Roundtrip::test_double_method (Test::double_load const &,
                               Test::Timestamp send_time)
{
  return send_time;
}

Test::Timestamp
Roundtrip::test_short_method (Test::short_load const &,
                              Test::Timestamp send_time)
{
  return send_time;
}

void
Roundtrip::shutdown ()
{
  this->orb_->shutdown (false);
}
