// $Id$

#include "ServerInterceptorAdapter.h"

#if TAO_HAS_INTERCEPTORS == 1

ACE_RCSID (PortableServer,
           ServerInterceptorAdapter,
           "$Id$")

#include "ServerRequestInfo.h"
#include "PICurrent_Guard.h"

#include "tao/ServerRequestInterceptor_Adapter.h"
#include "tao/TAO_Server_Request.h"
#include "tao/ORB_Core.h"


TAO::ServerRequestInterceptor_Adapter_Impl::ServerRequestInterceptor_Adapter_Impl (void)
{
}

#if TAO_HAS_EXTENDED_FT_INTERCEPTORS == 1
void
TAO::ServerRequestInterceptor_Adapter_Impl::tao_ft_interception_point (
  TAO_ServerRequest &server_request,
  TAO::Argument * const args[],
  size_t nargs,
  void * servant_upcall,
  CORBA::TypeCode_ptr const * exceptions,
  size_t nexceptions,
  CORBA::OctetSeq_out oc
  ACE_ENV_ARG_DECL)
{
  // This method implements one of the "starting" server side
  // interception point.

  ACE_TRY
    {
      // Copy the request scope current (RSC) to the thread scope
      // current (TSC) upon leaving this scope, i.e. just after the
      // receive_request_service_contexts() completes.  A "guard" is
      // used to make the copy also occur if an exception is thrown.
      TAO::PICurrent_Guard const pi_guard (ri->server_request (),
                                           false /* Copy RSC to TSC */);

      oc = 0;

      for (size_t i = 0 ; i < this->interceptor_list_.size(); ++i)
        {
          this->interceptors_[i]->tao_ft_interception_point (
            ri,
            oc
            ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (oc != 0)
            {
              (void) this->send_other (ri
                                       ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              return;
            }

          // The starting interception point completed successfully.
          // Push the interceptor on to the flow stack.
          ++server_request.interceptor_count ();
        }
    }
  ACE_CATCH (PortableInterceptor::ForwardRequest, exc)
    {
      ri->forward_reference (exc);
      this->send_other (ri
                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->location_forwarded_ = true;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

void
TAO::ServerRequestInterceptor_Adapter_Impl::receive_request_service_contexts (
  TAO_ServerRequest &server_request,
  TAO::Argument * const args[],
  size_t nargs,
  void * servant_upcall,
  CORBA::TypeCode_ptr const * exceptions,
  size_t nexceptions
  ACE_ENV_ARG_DECL)
{
  // This method implements one of the "intermediate" server side
  // interception point.
  if (this->interceptor_list_.size() != this->server_request.interceptor_count ())
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
      for (size_t i = 0 ; i < server_request.interceptor_count (); ++i)
        {
          this->interceptors_[i]->receive_request_service_contexts (
            ri
            ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCH (PortableInterceptor::ForwardRequest, exc)
    {
      ri->forward_reference (exc);
      this->send_other (ri
                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->location_forwarded_ = true;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

#elif TAO_HAS_EXTENDED_FT_INTERCEPTORS == 0

/// NOTE: Yes, we have two versions of this. This is easier than
/// messing around things in the same function, which is harder to
/// read and could make the code buggier.
void
TAO::ServerRequestInterceptor_Adapter_Impl::receive_request_service_contexts (
  TAO_ServerRequest &server_request,
  TAO::Argument * const args[],
  size_t nargs,
  void * servant_upcall,
  CORBA::TypeCode_ptr const * exceptions,
  size_t nexceptions
  ACE_ENV_ARG_DECL)
{

  // This method implements one of the "starting" server side
  // interception point if extended interceptors are not in place.

  TAO::ServerRequestInfo request_info (server_request,
                                       args,
                                       nargs,
                                       servant_upcall,
                                       exceptions,
                                       nexceptions);

  ACE_TRY
    {

      // Copy the request scope current (RSC) to the thread scope
      // current (TSC) upon leaving this scope, i.e. just after the
      // receive_request_service_contexts() completes.  A "guard" is
      // used to make the copy also occur if an exception is thrown.
      TAO::PICurrent_Guard const pi_guard (server_request,
                                           false /* Copy RSC to TSC */);

      for (size_t i = 0 ; i < this->interceptor_list_.size(); ++i)
        {
          this->interceptor_list_.interceptor(i)->
            receive_request_service_contexts (
              &request_info
              ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // The starting interception point completed successfully.
          // Push the interceptor on to the flow stack.
          ++server_request.interceptor_count ();
        }
    }
  ACE_CATCH (PortableInterceptor::ForwardRequest, exc)
    {
      server_request.forward_location (exc.forward.in ());
      server_request.reply_status (PortableInterceptor::LOCATION_FORWARD);
      this->send_other (server_request,
                        args,
                        nargs,
                        servant_upcall,
                        exceptions,
                        nexceptions
                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

#endif /*TAO_HAS_EXTENDED_FT_INTERCEPTORS*/

void
TAO::ServerRequestInterceptor_Adapter_Impl::receive_request (
  TAO_ServerRequest &server_request,
  TAO::Argument * const args[],
  size_t nargs,
  void * servant_upcall,
  CORBA::TypeCode_ptr const * exceptions,
  size_t nexceptions
  ACE_ENV_ARG_DECL)
{
  // This method implements an "intermediate" server side interception
  // point.  Interceptors are invoked in the same order they were
  // pushed on to the flow stack.

  if (this->interceptor_list_.size() != server_request.interceptor_count ())
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

  TAO::ServerRequestInfo request_info (server_request,
                                       args,
                                       nargs,
                                       servant_upcall,
                                       exceptions,
                                       nexceptions);

  ACE_TRY
    {
      for (size_t i = 0; i < server_request.interceptor_count (); ++i)
        {
          this->interceptor_list_.interceptor(i)->receive_request (
            &request_info
            ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // Note that no interceptors are pushed on to or popped off
          // of the flow stack in this interception point since it is
          // an intermediate interception point.
        }
    }
  ACE_CATCH (PortableInterceptor::ForwardRequest, exc)
    {
      server_request.forward_location (exc.forward.in ());
      server_request.reply_status (PortableInterceptor::LOCATION_FORWARD);
      this->send_other (server_request,
                        args,
                        nargs,
                        servant_upcall,
                        exceptions,
                        nexceptions
                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_ENDTRY;
  ACE_CHECK;

}

void
TAO::ServerRequestInterceptor_Adapter_Impl::send_reply (
  TAO_ServerRequest &server_request,
  TAO::Argument * const args[],
  size_t nargs,
  void * servant_upcall,
  CORBA::TypeCode_ptr const * exceptions,
  size_t nexceptions
  ACE_ENV_ARG_DECL)
{
  // This is an "ending" interception point so we only process the
  // interceptors pushed on to the flow stack.

  // Notice that the interceptors are processed in the opposite order
  // they were pushed onto the stack since this is an "ending"
  // interception point.

  TAO::ServerRequestInfo request_info (server_request,
                                       args,
                                       nargs,
                                       servant_upcall,
                                       exceptions,
                                       nexceptions);

  // Unwind the stack.
  size_t const len = server_request.interceptor_count ();
  for (size_t i = 0; i < len; ++i)
    {
      // Pop the interceptor off of the flow stack before it is
      // invoked.  This is necessary to prevent an interceptor already
      // invoked in this "ending" interception point from being
      // invoked in another "ending" interception point.
      --server_request.interceptor_count ();

      this->interceptor_list_.interceptor (
        server_request.interceptor_count ())->send_reply (
          &request_info
          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  // The send_reply() interception point does not raise a
  // PortableInterceptor::ForwardRequest exception so there is no need
  // to attempt to catch it here.
}

void
TAO::ServerRequestInterceptor_Adapter_Impl::send_exception (
  TAO_ServerRequest &server_request,
  TAO::Argument * const args[],
  size_t nargs,
  void * servant_upcall,
  CORBA::TypeCode_ptr const * exceptions,
  size_t nexceptions
  ACE_ENV_ARG_DECL)
{
  // This is an "ending" server side interception point so we only
  // process the interceptors pushed on to the flow stack.

  // Notice that the interceptors are processed in the opposite order
  // they were pushed onto the stack since this is an "ending" server
  // side interception point.

  TAO::ServerRequestInfo request_info (server_request,
                                       args,
                                       nargs,
                                       servant_upcall,
                                       exceptions,
                                       nexceptions);

  ACE_TRY
    {
      // Unwind the flow stack.
      size_t const len = server_request.interceptor_count ();
      for (size_t i = 0; i < len; ++i)
        {
          // Pop the interceptor off of the flow stack before it is
          // invoked.  This is necessary to prevent an interceptor
          // already invoked in this "ending" interception point from
          // being invoked in another "ending" interception point.
          --server_request.interceptor_count ();

          this->interceptor_list_.interceptor (
            server_request.interceptor_count ())->send_exception (
              &request_info
              ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCH (PortableInterceptor::ForwardRequest, exc)
    {
      server_request.forward_location (exc.forward.in ());
      server_request.reply_status (PortableInterceptor::LOCATION_FORWARD);
      this->send_other (server_request,
                        args,
                        nargs,
                        servant_upcall,
                        exceptions,
                        nexceptions
                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
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

      server_request.caught_exception (&ACE_ANY_EXCEPTION);

      this->send_exception (server_request,
                            args,
                            nargs,
                            servant_upcall,
                            exceptions,
                            nexceptions
                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableInterceptor::ReplyStatus status =
        server_request.reply_status ();

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
TAO::ServerRequestInterceptor_Adapter_Impl::send_other (
  TAO_ServerRequest &server_request,
  TAO::Argument * const args[],
  size_t nargs,
  void * servant_upcall,
  CORBA::TypeCode_ptr const * exceptions,
  size_t nexceptions
  ACE_ENV_ARG_DECL)
{
  // This is an "ending" server side interception point so we only
  // process the interceptors pushed on to the flow stack.

  TAO::ServerRequestInfo request_info (server_request,
                                       args,
                                       nargs,
                                       servant_upcall,
                                       exceptions,
                                       nexceptions);

  // Notice that the interceptors are processed in the opposite order
  // they were pushed onto the stack since this is an "ending" server
  // side interception point.

  ACE_TRY
    {
      // Unwind the flow stack.
      size_t const len = server_request.interceptor_count ();
      for (size_t i = 0; i < len; ++i)
        {
          // Pop the interceptor off of the flow stack before it is
          // invoked.  This is necessary to prevent an interceptor
          // already invoked in this "ending" interception point from
          // being invoked in another "ending" interception point.
          --server_request.interceptor_count ();

          this->interceptor_list_.interceptor(
            server_request.interceptor_count ())->send_other (
              &request_info
              ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCH (PortableInterceptor::ForwardRequest, exc)
    {
      server_request.forward_location (exc.forward.in ());
      server_request.reply_status (PortableInterceptor::LOCATION_FORWARD);
      this->send_other (server_request,
                        args,
                        nargs,
                        servant_upcall,
                        exceptions,
                        nexceptions
                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}


void
TAO::ServerRequestInterceptor_Adapter_Impl::add_interceptor (
  PortableInterceptor::ServerRequestInterceptor_ptr interceptor
  ACE_ENV_ARG_DECL)
{
  this->interceptor_list_.add_interceptor (interceptor ACE_ENV_ARG_PARAMETER);
}

void
TAO::ServerRequestInterceptor_Adapter_Impl::destroy_interceptors (
  ACE_ENV_SINGLE_ARG_DECL)
{
  this->interceptor_list_.destroy_interceptors (ACE_ENV_SINGLE_ARG_PARAMETER);
}

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
