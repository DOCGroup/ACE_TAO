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

// = The skeleton methods for test octet load

void
Roundtrip_Handler::test_octet_method (Test::Timestamp send_time
                                      ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->pending_callbacks_--;

  ACE_hrtime_t now = ACE_OS::gethrtime ();
  this->latency_stats_.sample (now - send_time);
}

void
Roundtrip_Handler::test_octet_method_excep (Test::AMI_RoundtripExceptionHolder *holder
                                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      this->pending_callbacks_--;
      holder->raise_test_octet_method (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "test_octet_method:");
    }
  ACE_ENDTRY;
}

// = The skeleton methods for test char load

void
Roundtrip_Handler::test_char_method (Test::Timestamp send_time
                                     ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->pending_callbacks_--;

  ACE_hrtime_t now = ACE_OS::gethrtime ();
  this->latency_stats_.sample (now - send_time);
}

void
Roundtrip_Handler::test_char_method_excep (Test::AMI_RoundtripExceptionHolder *holder
                                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      this->pending_callbacks_--;
      holder->raise_test_char_method (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "test_char_method:");
    }
  ACE_ENDTRY;
}

// = The skeleton methods for test long load

void
Roundtrip_Handler::test_long_method (Test::Timestamp send_time
                                     ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->pending_callbacks_--;

  ACE_hrtime_t now = ACE_OS::gethrtime ();
  this->latency_stats_.sample (now - send_time);
}

void
Roundtrip_Handler::test_long_method_excep (Test::AMI_RoundtripExceptionHolder *holder
                                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      this->pending_callbacks_--;
      holder->raise_test_long_method (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "test_long_method:");
    }
  ACE_ENDTRY;
}

// = The skeleton methods for test short load

void
Roundtrip_Handler::test_short_method (Test::Timestamp send_time
                                      ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->pending_callbacks_--;

  ACE_hrtime_t now = ACE_OS::gethrtime ();
  this->latency_stats_.sample (now - send_time);
}

void
Roundtrip_Handler::test_short_method_excep (Test::AMI_RoundtripExceptionHolder *holder
                                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      this->pending_callbacks_--;
      holder->raise_test_short_method (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "test_short_method:");
    }
  ACE_ENDTRY;
}

// = The skeleton methods for test double load

void
Roundtrip_Handler::test_double_method (Test::Timestamp send_time
                                       ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->pending_callbacks_--;

  ACE_hrtime_t now = ACE_OS::gethrtime ();
  this->latency_stats_.sample (now - send_time);
}

void
Roundtrip_Handler::test_double_method_excep (Test::AMI_RoundtripExceptionHolder *holder
                                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      this->pending_callbacks_--;
      holder->raise_test_double_method (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "test_double_method:");
    }
  ACE_ENDTRY;
}

// = The skeleton methods for test longlong load

void
Roundtrip_Handler::test_longlong_method (Test::Timestamp send_time
                                         ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->pending_callbacks_--;

  ACE_hrtime_t now = ACE_OS::gethrtime ();
  this->latency_stats_.sample (now - send_time);
}

void
Roundtrip_Handler::test_longlong_method_excep (Test::AMI_RoundtripExceptionHolder *holder
                                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      this->pending_callbacks_--;
      holder->raise_test_longlong_method (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "test_longlong_method:");
    }
  ACE_ENDTRY;
}


// = Other methods

void
Roundtrip_Handler::shutdown (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Roundtrip_Handler::shutdown_excep (Test::AMI_RoundtripExceptionHolder *holder
                                   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      holder->raise_shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "shutdown:");
    }
  ACE_ENDTRY;
}
