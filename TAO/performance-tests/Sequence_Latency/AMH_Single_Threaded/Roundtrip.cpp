#include "Roundtrip.h"

Roundtrip::Roundtrip (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Roundtrip::test_octet_method (Test::AMH_RoundtripResponseHandler_ptr response_handler,
                              Test::octet_load const &,
                              Test::Timestamp send_time)
{
  response_handler->test_octet_method (send_time);
}

void
Roundtrip::test_long_method (Test::AMH_RoundtripResponseHandler_ptr response_handler,
                             Test::long_load const &,
                             Test::Timestamp send_time)
{
  response_handler->test_long_method (send_time);
}

void
Roundtrip::test_short_method (Test::AMH_RoundtripResponseHandler_ptr response_handler,
                              Test::short_load const &,
                              Test::Timestamp send_time)
{
  response_handler->test_short_method (send_time);
}

void
Roundtrip::test_char_method (Test::AMH_RoundtripResponseHandler_ptr response_handler,
                             Test::char_load const &,
                             Test::Timestamp send_time)
{
  response_handler->test_char_method (send_time);
}

void
Roundtrip::test_longlong_method (Test::AMH_RoundtripResponseHandler_ptr response_handler,
                                 Test::longlong_load const &,
                                 Test::Timestamp send_time)
{
  response_handler->test_longlong_method (send_time);
}

void
Roundtrip::test_double_method (Test::AMH_RoundtripResponseHandler_ptr response_handler,
                               Test::double_load const &,
                               Test::Timestamp send_time)
{
  response_handler->test_double_method (send_time);
}

void
Roundtrip::shutdown (Test::AMH_RoundtripResponseHandler_ptr response_handler)
{
  response_handler->shutdown ();
  this->orb_->shutdown (false);
}
