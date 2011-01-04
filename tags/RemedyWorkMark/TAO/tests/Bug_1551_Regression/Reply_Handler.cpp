//
// $Id$
//

#include "Reply_Handler.h"

Reply_Handler::Reply_Handler (
    Test::Hello_ptr hello, CORBA::ORB_ptr orb)
  : counter_(1000)
  , hello_(Test::Hello::_duplicate(hello))
  , orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Reply_Handler::short_sleep (void)
{
  check_counter();

  Test::AMI_HelloHandler_var current =
    _this();

  hello_->sendc_short_sleep(current.in());
}

void
Reply_Handler::short_sleep_excep (
    ::Messaging::ExceptionHolder *ex)
{
  check_counter();

  try
    {
      if (ex)
        {
          ex->raise_exception ();
        }
      else
        {
          // Problem with the exceptionholder, instead of crashing, print
          // an error to the log, this normally shouldn't happen but when
          // there is a bug in TAO it can happen :-(
          ACE_ERROR ((LM_ERROR, "ERROR: Got nill exceptionholder\n"));
        }
    }
  catch (const CORBA::Exception&)
    {
      // Exceptions are expected and thus ignored in normal runs:
#if 0
      ex._tao_print_exception ("(%P|%t) Reply_Handler - caught exception:");
#endif /* 0 */
    }

  Test::AMI_HelloHandler_var current =
    _this();

  hello_->sendc_short_sleep(current.in());
}

void Reply_Handler::
check_counter(void)
{
  long const count = --counter_;
  if(count == 0)
  {
    // ACE_DEBUG((LM_DEBUG, "(%P|%t) Shut down client thread\n"));
    orb_->shutdown(0);
    return;
  }
#if 0
  if(count % 100 == 0)
  {
    ACE_DEBUG((LM_DEBUG, "(%P|%t) count = %d\n", count));
  }
#endif /* 0 */
}
