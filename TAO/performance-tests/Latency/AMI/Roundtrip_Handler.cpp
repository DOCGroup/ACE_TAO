#include "Roundtrip_Handler.h"
#include "ace/OS_NS_time.h"

Roundtrip_Handler::Roundtrip_Handler (int expected_callbacks)
  : pending_callbacks_ (expected_callbacks)
{
}

int
Roundtrip_Handler::pending_callbacks () const
{
  return this->pending_callbacks_;
}

void
Roundtrip_Handler::dump_results (
  ACE_High_Res_Timer::global_scale_factor_type gsf)
{
  this->latency_stats_.dump_results (ACE_TEXT("AMI Latency"), gsf);
}

void
Roundtrip_Handler::test_method (Test::Timestamp send_time)
{
  --this->pending_callbacks_;

  ACE_hrtime_t now = ACE_OS::gethrtime ();
  this->latency_stats_.sample (now - send_time);
}

void
Roundtrip_Handler::test_method_excep (::Messaging::ExceptionHolder *holder)
{
  try
    {
      --this->pending_callbacks_;
      holder->raise_exception ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("test_method:");
    }
}

void
Roundtrip_Handler::shutdown ()
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
