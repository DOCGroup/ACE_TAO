// $Id$

#include "ServerInterceptorAdapter.h"

#if TAO_HAS_INTERCEPTORS == 1

ACE_RCSID (PortableServer,
           ServerInterceptorAdapter,
           "$Id$")

#if !defined (__ACE_INLINE__)
# include "ServerInterceptorAdapter.inl"
#endif /* __ACE_INLINE__ */

#include "ServerRequestInfo.h"
//#include "PICurrent.h"


TAO_ServerRequestInterceptor_Adapter::
~TAO_ServerRequestInterceptor_Adapter (void)
{
}

void
TAO_ServerRequestInterceptor_Adapter::
receive_request_service_contexts (
  TAO_ServerRequestInfo *ri
  TAO_ENV_ARG_DECL)
{
  // This method implements one of the "starting" server side
  // interception point.

  ACE_TRY
    {
      // Copy the request scope current (RSC) to the thread scope
      // current (TSC) upon leaving this scope, i.e. just after the
      // receive_request_service_contexts() completes.  A "guard" is
      // used to make the copy also occur if an exception is thrown.
      TAO_PICurrent_Guard pi_guard (ri->server_request (),
                                    0 /* Copy RSC to TSC */);

      for (size_t i = 0 ; i < this->len_; ++i)
        {
          this->interceptors_[i]->receive_request_service_contexts (
            ri
            TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // The starting interception point completed successfully.
          // Push the interceptor on to the flow stack.
          ++this->stack_size_;
        }
    }
  ACE_CATCH (PortableInterceptor::ForwardRequest, exc)
    {
      ri->forward_reference (exc);
      this->send_other (ri
                        TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->location_forwarded_ = 1;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

void
TAO_ServerRequestInterceptor_Adapter::
receive_request (TAO_ServerRequestInfo *ri
                 TAO_ENV_ARG_DECL)
{
  // This method implements an "intermediate" server side interception
  // point.  Interceptors are invoked in the same order they were
  // pushed on to the flow stack.

  if (this->len_ != this->stack_size_)
    {
      // This method (i.e. the receive_request() interception point)
      // should only be invoked if all of the interceptors registered
      // with the ORB were pushed on to the flow stack by one of the
      // starting endpoints (such as
      // receive_request_service_contexts()).  If the above condition
      // evaluates to "true," then it is likely that a starting
      // interception point was never invoked.  This is of course, an
      // internal error that must be corrected.
      ACE_THROW (CORBA::INTERNAL ());
    }

  ACE_TRY
    {
      for (size_t i = 0; i < this->stack_size_; ++i)
        {
          this->interceptors_[i]->receive_request (ri
                                                   TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // Note that no interceptors are pushed on to or popped off
          // of the flow stack in this interception point since it is
          // an intermediate interception point.
        }
    }
  ACE_CATCH (PortableInterceptor::ForwardRequest, exc)
    {
      ri->forward_reference (exc);
      this->send_other (ri
                        TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->location_forwarded_ = 1;
    }
  ACE_ENDTRY;
  ACE_CHECK;

}

void
TAO_ServerRequestInterceptor_Adapter::
send_reply (TAO_ServerRequestInfo *ri
            TAO_ENV_ARG_DECL)
{
  // This is an "ending" interception point so we only process the
  // interceptors pushed on to the flow stack.

  // Notice that the interceptors are processed in the opposite order
  // they were pushed onto the stack since this is an "ending"
  // interception point.

  // Unwind the stack.
  size_t len = this->stack_size_;
  for (size_t i = 0; i < len; ++i)
    {
      // Pop the interceptor off of the flow stack before it is
      // invoked.  This is necessary to prevent an interceptor already
      // invoked in this "ending" interception point from being
      // invoked in another "ending" interception point.
      --this->stack_size_;

      this->interceptors_[this->stack_size_]->send_reply (
        ri
        TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  // The send_reply() interception point does not raise a
  // PortableInterceptor::ForwardRequest exception so there is no need
  // to attempt to catch it here.
}

void
TAO_ServerRequestInterceptor_Adapter::
send_exception (TAO_ServerRequestInfo *ri
                TAO_ENV_ARG_DECL)
{
  // This is an "ending" server side interception point so we only
  // process the interceptors pushed on to the flow stack.

  // Notice that the interceptors are processed in the opposite order
  // they were pushed onto the stack since this is an "ending" server
  // side interception point.

  ACE_TRY
    {
      // Unwind the flow stack.
      size_t len = this->stack_size_;
      for (size_t i = 0; i < len; ++i)
        {
          // Pop the interceptor off of the flow stack before it is
          // invoked.  This is necessary to prevent an interceptor
          // already invoked in this "ending" interception point from
          // being invoked in another "ending" interception point.
          --this->stack_size_;

          this->interceptors_[this->stack_size_]->send_exception (
            ri
            TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCH (PortableInterceptor::ForwardRequest, exc)
    {
      ri->forward_reference (exc);
      this->send_other (ri
                        TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->location_forwarded_ = 1;
    }
  ACE_CATCHANY
    {
      // The send_exception() interception point in the remaining
      // interceptors must be called so call this method (not the
      // interceptor's corresponding method) recursively.  The call is
      // made recursively since the caught exception must survive
      // until the remaining interceptors have been called.
      //
      // Note that the recursion will stop once the flow stack size
      // drops to zero, i.e., once each interceptor has been invoked.
      // This prevents infinite recursion from occuring.

      ri->exception (&ACE_ANY_EXCEPTION);

      this->send_exception (ri TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableInterceptor::ReplyStatus status =
        ri->reply_status (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Only re-throw the exception if it hasn't been transformed by
      // the send_exception() interception point (e.g. to a
      // LOCATION_FORWARD).
      if (status == PortableInterceptor::SYSTEM_EXCEPTION
          || status == PortableInterceptor::USER_EXCEPTION)
        ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

void
TAO_ServerRequestInterceptor_Adapter::
send_other (TAO_ServerRequestInfo *ri
            TAO_ENV_ARG_DECL)
{
  // This is an "ending" server side interception point so we only
  // process the interceptors pushed on to the flow stack.

  // Notice that the interceptors are processed in the opposite order
  // they were pushed onto the stack since this is an "ending" server
  // side interception point.

  ACE_TRY
    {
      // Unwind the flow stack.
      size_t len = this->stack_size_;
      for (size_t i = 0; i < len; ++i)
        {
          // Pop the interceptor off of the flow stack before it is
          // invoked.  This is necessary to prevent an interceptor
          // already invoked in this "ending" interception point from
          // being invoked in another "ending" interception point.
          --this->stack_size_;

          this->interceptors_[this->stack_size_]->send_other (
            ri
            TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCH (PortableInterceptor::ForwardRequest, exc)
    {
      ri->forward_reference (exc);
      this->send_other (ri
                        TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->location_forwarded_ = 1;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
