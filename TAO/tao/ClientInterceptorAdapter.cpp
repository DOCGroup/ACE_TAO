#include "ClientInterceptorAdapter.h"

#if TAO_HAS_INTERCEPTORS == 1

#if !defined (__ACE_INLINE__)
#include "ClientInterceptorAdapter.inl"
#endif /* defined INLINE */

#include "ClientRequestInfo.h"
#include "ClientRequestInfo_i.h"
#include "Invocation.h"

ACE_RCSID (tao,
           ClientInterceptorAdapter,
           "$Id$")

TAO_ClientRequestInterceptor_Adapter::
~TAO_ClientRequestInterceptor_Adapter (void)
{
}

void
TAO_ClientRequestInterceptor_Adapter::
send_request (TAO_ClientRequestInfo_i *ri
              TAO_ENV_ARG_DECL)
{
  // This method implements one of the "starting" client side
  // interception point.

  ACE_TRY
    {
      // Only perform the TSS access if interceptors were registered
      // with the ORB.
      if (this->len_ > 0)
        this->info_ =
          this->invocation_->orb_core ()->
          get_tss_resources ()->client_request_info_;

      TAO_ClientRequestInfo_Guard info_guard (this->info_, ri);

      for (size_t i = 0 ; i < this->len_; ++i)
        {
          this->interceptors_[i]->send_request (this->info_
                                                TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // The starting interception point completed successfully.
          // Push  the interceptor on to the flow stack.
          ++this->stack_size_;
        }
    }
  ACE_CATCH (PortableInterceptor::ForwardRequest, exc)
    {
      this->process_forward_request (ri, exc TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

void
TAO_ClientRequestInterceptor_Adapter::
receive_reply (TAO_ClientRequestInfo_i *ri
               TAO_ENV_ARG_DECL)
{
  // This is an "ending" interception point so we only process the
  // interceptors pushed on to the flow stack.

  // Notice that the interceptors are processed in the opposite order
  // they were pushed onto the stack since this is an "ending"
  // interception point.

  TAO_ClientRequestInfo_Guard info_guard (this->info_, ri);

  // Unwind the stack.
  size_t len = this->stack_size_;
  for (size_t i = 0; i < len; ++i)
    {
      // Pop the interceptor off of the flow stack before it is
      // invoked.  This is necessary to prevent an interceptor already
      // invoked in this "ending" interception point from being
      // invoked in another "ending" interception point.
      --this->stack_size_;

      this->interceptors_[this->stack_size_]->receive_reply (
        this->info_
        TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  // The receive_reply() interception point does not raise a
  // PortableInterceptor::ForwardRequest exception so there is no need
  // to attempt to catch it here.
}

void
TAO_ClientRequestInterceptor_Adapter::
receive_exception (TAO_ClientRequestInfo_i *ri
                   TAO_ENV_ARG_DECL)
{
  // This is an "ending" interception point so we only process the
  // interceptors pushed on to the flow stack.

  // Notice that the interceptors are processed in the opposite order
  // they were pushed onto the stack since this is an "ending"
  // interception point.

  ACE_TRY
    {
      TAO_ClientRequestInfo_Guard info_guard (this->info_, ri);

      // Unwind the flow stack.
      size_t len = this->stack_size_;
      for (size_t i = 0; i < len; ++i)
        {
          // Pop the interceptor off of the flow stack before it is
          // invoked.  This is necessary to prevent an interceptor
          // already invoked in this "ending" interception point from
          // being invoked in another "ending" interception point.
          --this->stack_size_;

          this->interceptors_[this->stack_size_]->receive_exception (
            this->info_
            TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCH (PortableInterceptor::ForwardRequest, exc)
    {
      this->process_forward_request (ri, exc TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // The receive_exception() interception point in the remaining
      // interceptors must be called so call this method (not the
      // interceptor's corresponding method) recursively.  The call is
      // made recursively since the caught exception must survive
      // until the remaining interceptors have been called.

      // Note that the recursion will stop once the flow stack size
      // drops to zero, i.e., once each interceptor has been invoked.
      // This prevents infinite recursion from occuring.

      ri->exception (&ACE_ANY_EXCEPTION);

      this->receive_exception (ri TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableInterceptor::ReplyStatus status =
        ri->reply_status (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Only re-throw the exception if it hasn't been transformed by
      // the receive_exception() interception point (e.g. to a
      // LOCATION_FORWARD).
      if (status == PortableInterceptor::SYSTEM_EXCEPTION
          || status == PortableInterceptor::USER_EXCEPTION)
        ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

void
TAO_ClientRequestInterceptor_Adapter::
receive_other (TAO_ClientRequestInfo_i *ri
               TAO_ENV_ARG_DECL)
{
  // This is an "ending" interception point so we only process the
  // interceptors pushed on to the flow stack.

  // Notice that the interceptors are processed in the opposite order
  // they were pushed onto the stack since this is an "ending"
  // interception point.

  ACE_TRY
    {
      TAO_ClientRequestInfo_Guard info_guard (this->info_, ri);

      // Unwind the stack.
      size_t len = this->stack_size_;
      for (size_t i = 0; i < len; ++i)
        {
          // Pop the interceptor off of the flow stack before it is
          // invoked.  This is necessary to prevent an interceptor
          // already invoked in this "ending" interception point from
          // being invoked in another "ending" interception point.
          --this->stack_size_;

          this->interceptors_[this->stack_size_]->receive_other (
            this->info_
            TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCH (PortableInterceptor::ForwardRequest, exc)
    {
      this->process_forward_request (ri, exc TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

void
TAO_ClientRequestInterceptor_Adapter::process_forward_request (
  TAO_ClientRequestInfo_i *ri,
  PortableInterceptor::ForwardRequest &exc
  TAO_ENV_ARG_DECL)
{
  ri->forward_reference (exc);

  this->invoke_status_ =
    this->invocation_->location_forward (exc.forward.in ()
                                         TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // receive_other() is potentially invoked recursively.
  this->receive_other (ri
                       TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

#endif /* TAO_HAS_INTERCEPTORS == 1 */
