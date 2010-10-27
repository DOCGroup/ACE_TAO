// -*- C++ -*-
// $Id$

#include "tao/PI_Server/ServerInterceptorAdapter.h"

#if TAO_HAS_INTERCEPTORS == 1

#include "tao/PI_Server/ServerRequestInfo.h"
#include "tao/PI_Server/PICurrent_Guard.h"

#include "tao/ServerRequestInterceptor_Adapter.h"
#include "tao/TAO_Server_Request.h"
#include "tao/ORB_Core.h"
#include "tao/PI/PICurrent_Impl.h"
#include "tao/PortableServer/Upcall_Command.h"
#include "tao/PortableInterceptor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
  CORBA::ULong nexceptions,
  CORBA::OctetSeq_out oc)
{
  // This method implements one of the "starting" server side
  // interception point.

  try
    {
      oc = 0;

      bool is_remote_request = !server_request.collocated ();
      TAO::ServerRequestInfo request_info (server_request,
                                          args,
                                          nargs,
                                          servant_upcall,
                                          exceptions,
                                          nexceptions);

      for (size_t i = 0 ; i < this->interceptor_list_.size(); ++i)
        {
          ServerRequestInterceptor_List::RegisteredInterceptor& registered =
            this->interceptor_list_.registered_interceptor (i);

          if (registered.details_.should_be_processed (is_remote_request))
            {
              registered.interceptor_->
                tao_ft_interception_point (&request_info, oc);
            }

          if (oc != 0)
            {
              (void) this->send_other (server_request,
                                       args,
                                       nargs,
                                       servant_upcall,
                                       exceptions,
                                       nexceptions);

              return;
            }

          // The starting interception point completed successfully.
          // Push the interceptor on to the flow stack.
          ++server_request.interceptor_count ();
        }
    }
  catch (const ::PortableInterceptor::ForwardRequest& exc)
    {
      server_request.forward_location (exc.forward.in ());
      server_request.pi_reply_status (PortableInterceptor::LOCATION_FORWARD);
      server_request.reply_status (GIOP::LOCATION_FORWARD);
      (void) this->send_other (server_request,
                               args,
                               nargs,
                               servant_upcall,
                               exceptions,
                               nexceptions);
    }
}

void
TAO::ServerRequestInterceptor_Adapter_Impl::receive_request_service_contexts (
  TAO_ServerRequest &server_request,
  TAO::Argument * const args[],
  size_t nargs,
  void * servant_upcall,
  CORBA::TypeCode_ptr const * exceptions,
  CORBA::ULong nexceptions)
{
  // This method implements one of the "intermediate" server side
  // interception point.

  if (this->interceptor_list_.size() != server_request.interceptor_count ())
    {
      // This method (i.e. the receive_request() interception point)
      // should only be invoked if all of the interceptors registered
      // with the ORB were pushed on to the flow stack by one of the
      // starting endpoints (such as
      // tao_ft_interception_point()).  If the above condition
      // evaluates to "true," then it is likely that a starting
      // interception point was never invoked.  This is of course, an
      // internal error that must be corrected.
      throw ::CORBA::INTERNAL ();
    }

  try
    {
      // Copy the request scope current (RSC) to the thread scope
      // current (TSC) upon leaving this scope, i.e. just after the
      // receive_request_service_contexts() completes.  A "guard" is
      // used to make the copy also occur if an exception is thrown.
      TAO::PICurrent_Guard const pi_guard (server_request,
                                           false /* Copy RSC to TSC */);

      bool is_remote_request = !server_request.collocated ();
      TAO::ServerRequestInfo request_info (server_request,
                                           args,
                                           nargs,
                                           servant_upcall,
                                           exceptions,
                                           nexceptions);

      for (size_t i = 0 ; i < server_request.interceptor_count (); ++i)
        {
          ServerRequestInterceptor_List::RegisteredInterceptor& registered =
            this->interceptor_list_.registered_interceptor (i);

          if (registered.details_.should_be_processed (is_remote_request))
            {
              registered.interceptor_->
                receive_request_service_contexts (&request_info);
            }
        }
    }
  catch (const ::PortableInterceptor::ForwardRequest& exc)
    {
      server_request.forward_location (exc.forward.in ());
      server_request.pi_reply_status (PortableInterceptor::LOCATION_FORWARD);
      server_request.reply_status (GIOP::LOCATION_FORWARD);
      (void) this->send_other (server_request,
                               args,
                               nargs,
                               servant_upcall,
                               exceptions,
                               nexceptions);
    }
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
  CORBA::ULong nexceptions)
{
  // This method implements one of the "starting" server side
  // interception point if extended interceptors are not in place.

  try
    {
      // Copy the request scope current (RSC) to the thread scope
      // current (TSC) upon leaving this scope, i.e. just after the
      // receive_request_service_contexts() completes.  A "guard" is
      // used to make the copy also occur if an exception is thrown.
      TAO::PICurrent_Guard const pi_guard (server_request,
                                           false /* Copy RSC to TSC */);

      bool is_remote_request = !server_request.collocated ();

      TAO::ServerRequestInfo request_info (server_request,
                                           args,
                                           nargs,
                                           servant_upcall,
                                           exceptions,
                                           nexceptions);

      for (size_t i = 0 ; i < this->interceptor_list_.size(); ++i)
        {
          ServerRequestInterceptor_List::RegisteredInterceptor& registered =
            this->interceptor_list_.registered_interceptor (i);

          if (registered.details_.should_be_processed (is_remote_request))
            {
              registered.interceptor_->
                receive_request_service_contexts (&request_info);
            }

          // The starting interception point completed successfully.
          // Push the interceptor on to the flow stack.
          ++server_request.interceptor_count ();
        }
    }
  catch (const ::PortableInterceptor::ForwardRequest& exc)
    {
      server_request.forward_location (exc.forward.in ());
      server_request.pi_reply_status (PortableInterceptor::LOCATION_FORWARD);
      server_request.reply_status (GIOP::LOCATION_FORWARD);
      (void) this->send_other (server_request,
                               args,
                               nargs,
                               servant_upcall,
                               exceptions,
                               nexceptions);
    }
}

#endif /*TAO_HAS_EXTENDED_FT_INTERCEPTORS*/

void
TAO::ServerRequestInterceptor_Adapter_Impl::receive_request (
  TAO_ServerRequest &server_request,
  TAO::Argument * const args[],
  size_t nargs,
  void * servant_upcall,
  CORBA::TypeCode_ptr const * exceptions,
  CORBA::ULong nexceptions
  )
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
      throw ::CORBA::INTERNAL ();
    }

  TAO::ServerRequestInfo request_info (server_request,
                                       args,
                                       nargs,
                                       servant_upcall,
                                       exceptions,
                                       nexceptions);

  try
    {
      bool is_remote_request = !server_request.collocated ();

      for (size_t i = 0; i < server_request.interceptor_count (); ++i)
        {
          ServerRequestInterceptor_List::RegisteredInterceptor& registered =
            this->interceptor_list_.registered_interceptor (i);

          if (registered.details_.should_be_processed (is_remote_request))
            {
              registered.interceptor_->receive_request (&request_info);
            }

          // Note that no interceptors are pushed on to or popped off
          // of the flow stack in this interception point since it is
          // an intermediate interception point.
        }
    }
  catch (const ::PortableInterceptor::ForwardRequest& exc)
    {
      server_request.forward_location (exc.forward.in ());
      server_request.pi_reply_status (PortableInterceptor::LOCATION_FORWARD);
      server_request.reply_status (GIOP::LOCATION_FORWARD);
      this->send_other (server_request,
                        args,
                        nargs,
                        servant_upcall,
                        exceptions,
                        nexceptions);
    }

}

void
TAO::ServerRequestInterceptor_Adapter_Impl::send_reply (
  TAO_ServerRequest &server_request,
  TAO::Argument * const args[],
  size_t nargs,
  void * servant_upcall,
  CORBA::TypeCode_ptr const * exceptions,
  CORBA::ULong nexceptions)
{
  // This is an "ending" interception point so we only process the
  // interceptors pushed on to the flow stack.

  bool const is_remote_request = !server_request.collocated ();

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

      ServerRequestInterceptor_List::RegisteredInterceptor& registered =
        this->interceptor_list_.registered_interceptor (
          server_request.interceptor_count ());

      if (registered.details_.should_be_processed (is_remote_request))
        {
          registered.interceptor_->send_reply (&request_info);
        }
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
  CORBA::ULong nexceptions)
{
  // This is an "ending" server side interception point so we only
  // process the interceptors pushed on to the flow stack.

  bool is_remote_request = !server_request.collocated ();

  // Notice that the interceptors are processed in the opposite order
  // they were pushed onto the stack since this is an "ending" server
  // side interception point.

  TAO::ServerRequestInfo request_info (server_request,
                                       args,
                                       nargs,
                                       servant_upcall,
                                       exceptions,
                                       nexceptions);

  try
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

          ServerRequestInterceptor_List::RegisteredInterceptor& registered =
            this->interceptor_list_.registered_interceptor (
              server_request.interceptor_count ());

          if (registered.details_.should_be_processed (is_remote_request))
            {
              registered.interceptor_->send_exception (&request_info);
            }
        }
    }
  catch (const ::PortableInterceptor::ForwardRequest& exc)
    {
      server_request.forward_location (exc.forward.in ());
      server_request.pi_reply_status (PortableInterceptor::LOCATION_FORWARD);
      server_request.reply_status (GIOP::LOCATION_FORWARD);
      this->send_other (server_request,
                        args,
                        nargs,
                        servant_upcall,
                        exceptions,
                        nexceptions);
    }
  catch ( ::CORBA::Exception& ex)
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

      server_request.caught_exception (&ex);

      this->send_exception (server_request,
                            args,
                            nargs,
                            servant_upcall,
                            exceptions,
                            nexceptions);

      PortableInterceptor::ReplyStatus status =
        server_request.pi_reply_status ();

      // Only re-throw the exception if it hasn't been transformed by
      // the send_exception() interception point (e.g. to a
      // LOCATION_FORWARD).
      if (status == PortableInterceptor::SYSTEM_EXCEPTION
          || status == PortableInterceptor::USER_EXCEPTION)
        throw;
    }
}

void
TAO::ServerRequestInterceptor_Adapter_Impl::send_other (
  TAO_ServerRequest &server_request,
  TAO::Argument * const args[],
  size_t nargs,
  void * servant_upcall,
  CORBA::TypeCode_ptr const * exceptions,
  CORBA::ULong nexceptions)
{
  // This is an "ending" server side interception point so we only
  // process the interceptors pushed on to the flow stack.

  bool is_remote_request = !server_request.collocated ();

  TAO::ServerRequestInfo request_info (server_request,
                                       args,
                                       nargs,
                                       servant_upcall,
                                       exceptions,
                                       nexceptions);

  // Notice that the interceptors are processed in the opposite order
  // they were pushed onto the stack since this is an "ending" server
  // side interception point.

  try
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

          ServerRequestInterceptor_List::RegisteredInterceptor& registered =
            this->interceptor_list_.registered_interceptor (
              server_request.interceptor_count ());

          if (registered.details_.should_be_processed (is_remote_request))
            {
              registered.interceptor_->send_other (&request_info);
            }
        }
    }
  catch (const ::PortableInterceptor::ForwardRequest& exc)
    {
      server_request.forward_location (exc.forward.in ());
      server_request.pi_reply_status (PortableInterceptor::LOCATION_FORWARD);
      server_request.reply_status (GIOP::LOCATION_FORWARD);
      this->send_other (server_request,
                        args,
                        nargs,
                        servant_upcall,
                        exceptions,
                        nexceptions);
    }
}

void
TAO::ServerRequestInterceptor_Adapter_Impl::add_interceptor (
  PortableInterceptor::ServerRequestInterceptor_ptr interceptor)
{
  this->interceptor_list_.add_interceptor (interceptor);
}

void
TAO::ServerRequestInterceptor_Adapter_Impl::add_interceptor (
  PortableInterceptor::ServerRequestInterceptor_ptr interceptor,
  const CORBA::PolicyList& policies)
{
  this->interceptor_list_.add_interceptor (interceptor, policies);
}

void
TAO::ServerRequestInterceptor_Adapter_Impl::destroy_interceptors (void)
{
  this->interceptor_list_.destroy_interceptors ();
}

TAO::PICurrent_Impl *
TAO::ServerRequestInterceptor_Adapter_Impl::allocate_pi_current (void)
{
  TAO::PICurrent_Impl *pi = 0;
  ACE_NEW_RETURN (pi,
                  TAO::PICurrent_Impl,
                  pi);
  return pi;
}

void
TAO::ServerRequestInterceptor_Adapter_Impl::deallocate_pi_current (
        TAO::PICurrent_Impl *picurrent)
{
  delete picurrent;
}

void
TAO::ServerRequestInterceptor_Adapter_Impl::execute_command (
  TAO_ServerRequest & server_request,
  TAO::Upcall_Command & command)
{
  TAO::PICurrent_Guard const pi_guard (server_request,
                                       true  /* Copy TSC to RSC */);

  // The actual upcall.
  command.execute ();
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
