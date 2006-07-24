//
// $Id$
//

#include "Reply_Handler.h"

ACE_RCSID(Bug_1XXX_Regression, Reply_Handler, "$Id$")

Reply_Handler::Reply_Handler (
    Test::Hello_ptr hello, CORBA::ORB_ptr orb)
  : counter_(1000)
  , hello_(Test::Hello::_duplicate(hello))
  , orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Reply_Handler::short_sleep (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  check_counter(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  Test::AMI_HelloHandler_var current =
    _this(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  hello_->sendc_short_sleep(current.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
Reply_Handler::short_sleep_excep (
    ::Messaging::ExceptionHolder *ex
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  check_counter(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_TRY
    {
      if (ex)
        {
          ex->raise_exception (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      else
        {
          // Problem with the exceptionholder, instead of crashing, print
          // an error to the log, this normally shouldn't happen but when
          // there is a bug in TAO it can happen :-(
          ACE_ERROR ((LM_ERROR, "ERROR: Got nill exceptionholder\n"));
        }
    }
  ACE_CATCHANY
    {
      // Exceptions are expected and thus ignored in normal runs:
#if 0
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "(%P|%t) Reply_Handler - caught exception:");
#endif /* 0 */
    }
  ACE_ENDTRY;
  ACE_CHECK;

  Test::AMI_HelloHandler_var current =
    _this(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  hello_->sendc_short_sleep(current.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void Reply_Handler::
check_counter(ACE_ENV_SINGLE_ARG_DECL)
{
  long const count = --counter_;
  if(count == 0)
  {
    // ACE_DEBUG((LM_DEBUG, "(%P|%t) Shut down client thread\n"));
    orb_->shutdown(0 ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
    return;
  }
#if 0
  if(count % 100 == 0)
  {
    ACE_DEBUG((LM_DEBUG, "(%P|%t) count = %d\n", count));
  }
#endif /* 0 */
}
