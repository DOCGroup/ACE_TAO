// $Id$

#include "tao/PI/ClientRequestInterceptor_Adapter_Impl.h"

#if TAO_HAS_INTERCEPTORS == 1

#if !defined (__ACE_INLINE__)
#include "tao/PI/ClientRequestInterceptor_Adapter_Impl.inl"
#endif /* defined INLINE */

#include "tao/PI/ClientRequestInfo.h"

#include "tao/Invocation_Base.h"
#include "tao/ORB_Core.h"
#include "tao/ORB_Core_TSS_Resources.h"
#include "tao/PortableInterceptorC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  void
  ClientRequestInterceptor_Adapter_Impl::send_request (
      Invocation_Base &invocation)
  {
    // This method implements one of the "starting" client side
    // interception point.

    bool const is_remote_request = invocation.is_remote_request();

    try
      {
        TAO_ClientRequestInfo ri (&invocation);

        for (size_t i = 0 ; i < this->interceptor_list_.size (); ++i)
          {
            ClientRequestInterceptor_List::RegisteredInterceptor& registered =
              this->interceptor_list_.registered_interceptor (i);

            if (registered.details_.should_be_processed (is_remote_request))
              {
                registered.interceptor_->send_request (&ri);
              }

            // The starting interception point completed successfully.
            // Push  the interceptor on to the flow stack.
            ++invocation.stack_size ();
          }
      }
    catch (const ::PortableInterceptor::ForwardRequest& exc)
      {
        this->process_forward_request (invocation, exc);
      }
  }

  void
  ClientRequestInterceptor_Adapter_Impl::receive_reply (
    Invocation_Base &invocation)
  {
    // This is an "ending" interception point so we only process the
    // interceptors pushed on to the flow stack.

    bool const is_remote_request = invocation.is_remote_request();

    // Notice that the interceptors are processed in the opposite order
    // they were pushed onto the stack since this is an "ending"
    // interception point.

    TAO_ClientRequestInfo ri (&invocation);

    // Unwind the stack.
    size_t const len = invocation.stack_size ();
    for (size_t i = 0; i < len; ++i)
      {
        // Pop the interceptor off of the flow stack before it is
        // invoked.  This is necessary to prevent an interceptor already
        // invoked in this "ending" interception point from being
        // invoked in another "ending" interception point.
        --invocation.stack_size ();

        ClientRequestInterceptor_List::RegisteredInterceptor& registered =
          this->interceptor_list_.registered_interceptor (
            invocation.stack_size ());

        if (registered.details_.should_be_processed (is_remote_request))
          {
            registered.interceptor_->receive_reply (&ri);
          }
      }

    // The receive_reply() interception point does not raise a
    // PortableInterceptor::ForwardRequest exception so there is no need
    // to attempt to catch it here.
  }

  void
  ClientRequestInterceptor_Adapter_Impl::receive_exception (
      Invocation_Base &invocation)
  {
    // This is an "ending" interception point so we only process the
    // interceptors pushed on to the flow stack.

    bool const is_remote_request = invocation.is_remote_request();

    // Notice that the interceptors are processed in the opposite order
    // they were pushed onto the stack since this is an "ending"
    // interception point.
    try
      {
        TAO_ClientRequestInfo ri (&invocation);

        // Unwind the flow stack.
        size_t const len = invocation.stack_size ();
        for (size_t i = 0; i < len; ++i)
          {
            // Pop the interceptor off of the flow stack before it is
            // invoked.  This is necessary to prevent an interceptor
            // already invoked in this "ending" interception point from
            // being invoked in another "ending" interception point.
            --invocation.stack_size ();

            ClientRequestInterceptor_List::RegisteredInterceptor& registered =
              this->interceptor_list_.registered_interceptor (
                invocation.stack_size ());

            if (registered.details_.should_be_processed (is_remote_request))
              {
                registered.interceptor_->receive_exception (&ri);
              }
          }
      }
    catch (const ::PortableInterceptor::ForwardRequest& exc)
      {
        this->process_forward_request (invocation, exc);
      }
    catch ( ::CORBA::Exception& ex)
      {
        // The receive_exception() interception point in the remaining
        // interceptors must be called so call this method (not the
        // interceptor's corresponding method) recursively.  The call is
        // made recursively since the caught exception must survive
        // until the remaining interceptors have been called.

        // Note that the recursion will stop once the flow stack size
        // drops to zero, i.e., once each interceptor has been invoked.
        // This prevents infinite recursion from occuring.

        invocation.exception (&ex);

        this->receive_exception (invocation);

        PortableInterceptor::ReplyStatus status =
          this->pi_reply_status (invocation);

        // Only re-throw the exception if it hasn't been transformed by
        // the receive_exception() interception point (e.g. to a
        // LOCATION_FORWARD).
        if (status == PortableInterceptor::SYSTEM_EXCEPTION
            || status == PortableInterceptor::USER_EXCEPTION)
          throw;
      }
  }

  void
  ClientRequestInterceptor_Adapter_Impl::receive_other (
      Invocation_Base &invocation)
  {
    // This is an "ending" interception point so we only process the
    // interceptors pushed on to the flow stack.

    bool const is_remote_request = invocation.is_remote_request();

    // Notice that the interceptors are processed in the opposite order
    // they were pushed onto the stack since this is an "ending"
    // interception point.

    try
      {
        TAO_ClientRequestInfo ri (&invocation);

        // Unwind the stack.
        size_t const len = invocation.stack_size ();
        for (size_t i = 0; i < len; ++i)
        {
          // Pop the interceptor off of the flow stack before it is
          // invoked.  This is necessary to prevent an interceptor
          // already invoked in this "ending" interception point from
          // being invoked in another "ending" interception point.
          --invocation.stack_size ();

          ClientRequestInterceptor_List::RegisteredInterceptor& registered =
            this->interceptor_list_.registered_interceptor (
              invocation.stack_size ());

          if (registered.details_.should_be_processed (is_remote_request))
            {
              registered.interceptor_->receive_other (&ri);
            }
        }
      }
    catch (const ::PortableInterceptor::ForwardRequest& exc)
      {
        this->process_forward_request (invocation, exc);
      }
    catch ( ::CORBA::Exception& ex)
      {
        // The receive_exception() interception point in the remaining
        // interceptors must be called so call this method (not the
        // interceptor's corresponding method) recursively.  The call is
        // made recursively since the caught exception must survive
        // until the remaining interceptors have been called.

        // Note that the recursion will stop once the flow stack size
        // drops to zero, i.e., once each interceptor has been invoked.
        // This prevents infinite recursion from occuring.

        invocation.exception (&ex);

        this->receive_exception (invocation);

        PortableInterceptor::ReplyStatus status =
          this->pi_reply_status (invocation);

        // Only re-throw the exception if it hasn't been transformed by
        // the receive_exception() interception point (e.g. to a
        // LOCATION_FORWARD).
        if (status == PortableInterceptor::SYSTEM_EXCEPTION
            || status == PortableInterceptor::USER_EXCEPTION)
          throw;
      }
  }

  void
  ClientRequestInterceptor_Adapter_Impl::process_forward_request (
      Invocation_Base &invocation,
      const PortableInterceptor::ForwardRequest &exc)
  {
    invocation.forwarded_reference (exc.forward.in ());

    // receive_other() is potentially invoked recursively.
    this->receive_other (invocation);
  }

  void
  ClientRequestInterceptor_Adapter_Impl::add_interceptor (
    PortableInterceptor::ClientRequestInterceptor_ptr interceptor)
  {
    this->interceptor_list_.add_interceptor (interceptor);
  }

  void
  ClientRequestInterceptor_Adapter_Impl::add_interceptor (
    PortableInterceptor::ClientRequestInterceptor_ptr interceptor,
    const CORBA::PolicyList& policies)
  {
    this->interceptor_list_.add_interceptor (interceptor, policies);
  }

  void
  ClientRequestInterceptor_Adapter_Impl::destroy_interceptors (void)
  {
    this->interceptor_list_.destroy_interceptors ();
  }

  PortableInterceptor::ReplyStatus
  ClientRequestInterceptor_Adapter_Impl::pi_reply_status (
    TAO::Invocation_Base const &invocation_base)
  {
    PortableInterceptor::ReplyStatus reply_status;

    switch (invocation_base.invoke_status ())
      {
      case TAO::TAO_INVOKE_SUCCESS:
        reply_status = PortableInterceptor::SUCCESSFUL;
        break;
      case TAO::TAO_INVOKE_RESTART:
        if (invocation_base.reply_status () == GIOP::LOCATION_FORWARD ||
            invocation_base.reply_status () == GIOP::LOCATION_FORWARD_PERM)
          reply_status = PortableInterceptor::LOCATION_FORWARD;
        else
          reply_status = PortableInterceptor::TRANSPORT_RETRY;
        break;
      case TAO::TAO_INVOKE_USER_EXCEPTION:
        reply_status = PortableInterceptor::USER_EXCEPTION;
        break;
      case TAO::TAO_INVOKE_SYSTEM_EXCEPTION:
        reply_status = PortableInterceptor::SYSTEM_EXCEPTION;
        break;
      default:
        reply_status = PortableInterceptor::UNKNOWN;
        break;
      }

    return reply_status;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
