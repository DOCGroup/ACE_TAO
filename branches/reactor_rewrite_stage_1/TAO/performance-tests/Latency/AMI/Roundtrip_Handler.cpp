//
// $Id$
//
#include "Roundtrip_Handler.h"

ACE_RCSID(AMI_Latency, Roundtrip, "$Id$")

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
  this->latency_stats_.dump_results ("AMI Latency", gsf);
}

void
Roundtrip_Handler::test_method (Test::Timestamp send_time
                                TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->pending_callbacks_--;

  ACE_hrtime_t now = ACE_OS::gethrtime ();
  this->latency_stats_.sample (now - send_time);
}

void
Roundtrip_Handler::test_method_excep (Test::AMI_RoundtripExceptionHolder *holder
                                      TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      this->pending_callbacks_--;
      holder->raise_test_method (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "test_method:");
    }
  ACE_ENDTRY;
}

void
Roundtrip_Handler::shutdown (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Roundtrip_Handler::shutdown_excep (Test::AMI_RoundtripExceptionHolder *holder
                                   TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      holder->raise_shutdown (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "shutdown:");
    }
  ACE_ENDTRY;
}
