//
// $Id$
//
#include "Roundtrip_Handler.h"
#include "ace/OS_NS_time.h"

Roundtrip_Handler::Roundtrip_Handler (int expected_callbacks)
  : pending_callbacks_ (expected_callbacks)
{
}

int
Roundtrip_Handler::pending_callbacks (void) const
{
  return this->pending_callbacks_;
}

void
Roundtrip_Handler::dump_results (ACE_UINT32 gsf)
{
  this->latency_stats_.dump_results (ACE_TEXT("AMI Latency"), gsf);
}

// = The skeleton methods for test octet load

void
Roundtrip_Handler::test_octet_method (Test::Timestamp send_time)
{
  --this->pending_callbacks_;

  ACE_hrtime_t now = ACE_OS::gethrtime ();
  this->latency_stats_.sample (now - send_time);
}

void
Roundtrip_Handler::test_octet_method_excep (::Messaging::ExceptionHolder *holder)
{
  try
    {
      --this->pending_callbacks_;
      holder->raise_exception ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("test_octet_method:");
    }
}

// = The skeleton methods for test char load

void
Roundtrip_Handler::test_char_method (Test::Timestamp send_time)
{
  --this->pending_callbacks_;

  ACE_hrtime_t now = ACE_OS::gethrtime ();
  this->latency_stats_.sample (now - send_time);
}

void
Roundtrip_Handler::test_char_method_excep (::Messaging::ExceptionHolder *holder)
{
  try
    {
      --this->pending_callbacks_;
      holder->raise_exception ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("test_char_method:");
    }
}

// = The skeleton methods for test long load

void
Roundtrip_Handler::test_long_method (Test::Timestamp send_time)
{
  --this->pending_callbacks_;

  ACE_hrtime_t now = ACE_OS::gethrtime ();
  this->latency_stats_.sample (now - send_time);
}

void
Roundtrip_Handler::test_long_method_excep (::Messaging::ExceptionHolder *holder)
{
  try
    {
      --this->pending_callbacks_;
      holder->raise_exception ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("test_long_method:");
    }
}

// = The skeleton methods for test short load

void
Roundtrip_Handler::test_short_method (Test::Timestamp send_time)
{
  --this->pending_callbacks_;

  ACE_hrtime_t now = ACE_OS::gethrtime ();
  this->latency_stats_.sample (now - send_time);
}

void
Roundtrip_Handler::test_short_method_excep (::Messaging::ExceptionHolder *holder)
{
  try
    {
      --this->pending_callbacks_;
      holder->raise_exception ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("test_short_method:");
    }
}

// = The skeleton methods for test double load

void
Roundtrip_Handler::test_double_method (Test::Timestamp send_time)
{
  --this->pending_callbacks_;

  ACE_hrtime_t now = ACE_OS::gethrtime ();
  this->latency_stats_.sample (now - send_time);
}

void
Roundtrip_Handler::test_double_method_excep (::Messaging::ExceptionHolder *holder)
{
  try
    {
      --this->pending_callbacks_;
      holder->raise_exception ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("test_double_method:");
    }
}

// = The skeleton methods for test longlong load

void
Roundtrip_Handler::test_longlong_method (Test::Timestamp send_time)
{
  --this->pending_callbacks_;

  ACE_hrtime_t now = ACE_OS::gethrtime ();
  this->latency_stats_.sample (now - send_time);
}

void
Roundtrip_Handler::test_longlong_method_excep (::Messaging::ExceptionHolder *holder)
{
  try
    {
      --this->pending_callbacks_;
      holder->raise_exception ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("test_longlong_method:");
    }
}


// = Other methods

void
Roundtrip_Handler::shutdown (void)
{
}

void
Roundtrip_Handler::shutdown_excep (::Messaging::ExceptionHolder *holder)
{
  try
    {
      holder->raise_exception ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("shutdown:");
    }
}
