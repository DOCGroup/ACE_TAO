// $Id$

#include "tao/PortableServer/Upcall_Wrapper.h"
#include "tao/PortableServer/Upcall_Command.h"
#include "tao/PortableServer/Collocated_Arguments_Converter.h"
#include "tao/SystemException.h"

#if TAO_HAS_INTERCEPTORS == 1
# include "tao/ServerRequestInterceptor_Adapter.h"
# include "tao/ORB_Core.h"
#endif  /* TAO_HAS_INTERCEPTORS == 1 */

#include "tao/PortableInterceptorC.h"
#include "tao/PortableInterceptor.h"

#include "tao/TAO_Server_Request.h"
#include "tao/CDR.h"
#include "tao/Argument.h"
#include "tao/operation_details.h"
#include "ace/Log_Msg.h"
#include "tao/debug.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO::Upcall_Wrapper::upcall (TAO_ServerRequest & server_request,
                             TAO::Argument * const args[],
                             size_t nargs,
                             TAO::Upcall_Command & command
#if TAO_HAS_INTERCEPTORS == 1
                             , void * servant_upcall
                             , CORBA::TypeCode_ptr const * exceptions
                             , CORBA::ULong nexceptions
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
                             )
{
  if (server_request.collocated ()
    && server_request.operation_details ()->cac () != 0)
    {
      server_request.operation_details ()->cac ()->convert_request (
        server_request, args, nargs);
    }

  if (server_request.incoming ())
    {
      this->pre_upcall (*server_request.incoming (), args, nargs);
    }

#if TAO_HAS_INTERCEPTORS == 1

  // Make sure that, for the collocated case, we use the client-side
  // arguments.  For the non-collocated case, we will use the server-side
  // arguments since they got set up in our pre_upcall() method.  Note that
  // our pre_upcall() method doesn't get invoked in the collocated case,
  // and is the reason why we need to provide the client-side args instead
  // of the (never set or initialized) server-side args.
  //
  // Before the following logic was added, the
  // $TAO_ROOT/tests/Portable_Interceptors/Collocated/run_test.pl
  // showed that the server-side request interceptor was getting bogus
  // values when it took a look at the request arguments.  Some
  // additional testing revealed that this only occurred in the
  // collocated request case.

  // By default, we assume that we will use the server-side args.
  TAO::Argument * const * the_args = args;
  size_t the_nargs = nargs;

  if (server_request.collocated())
    {
      // It is a collocated request so we need to use the client-side
      // args instead.
      the_args = server_request.operation_details()->args();
      the_nargs = server_request.operation_details()->args_num();
    }

  TAO::ServerRequestInterceptor_Adapter *interceptor_adapter =
    server_request.orb_core ()->serverrequestinterceptor_adapter ();
#endif /* TAO_HAS_INTERCEPTORS */

  try
    {
      {
#if TAO_HAS_INTERCEPTORS == 1
        if (interceptor_adapter != 0)
          {
            // Invoke intermediate server side interception points.
            interceptor_adapter->receive_request (server_request,
                                                  the_args,
                                                  the_nargs,
                                                  servant_upcall,
                                                  exceptions,
                                                  nexceptions);
          }

        // Don't bother performing the upcall if an interceptor caused a
        // location forward.
        CORBA::Object_var forward_to = server_request.forward_location ();
        if (!server_request.is_forwarded ())
          {
            if (interceptor_adapter != 0)
              {
                interceptor_adapter->execute_command (server_request, command);
              }
            else
#endif /* TAO_HAS_INTERCEPTORS */
              {
                // The actual upcall.
                command.execute ();
              }
#if TAO_HAS_INTERCEPTORS == 1
          }
#endif /* TAO_HAS_INTERCEPTORS */
      }
#if TAO_HAS_INTERCEPTORS == 1
      if (interceptor_adapter == 0)
        {
          server_request.pi_reply_status (PortableInterceptor::SUCCESSFUL);
        }
      else
        {
          // Do not execute the send_reply() interception point if an
          // interceptor caused a location forward.  The send_other()
          // interception point should already have been executed by the
          // ServerRequestInterceptor_Adapter object.
          //
          // It should actually be safe to call this interception point,
          // regardless, since the interceptor flow stack should have been
          // emptied by the send_other() interception point.  Note that
          // we'd still need to avoid resetting the reply status to
          // SUCCESSFUL, however.
          CORBA::Object_var forward_to_after = server_request.forward_location ();
          if (!server_request.is_forwarded ())
            {
              // No location forward by interceptors and successful upcall.
              server_request.pi_reply_status (PortableInterceptor::SUCCESSFUL);
              interceptor_adapter->send_reply (server_request,
                                               the_args,
                                               the_nargs,
                                               servant_upcall,
                                               exceptions,
                                               nexceptions);
            }
        }
#endif /* TAO_HAS_INTERCEPTORS */
    }
  catch ( ::CORBA::Exception& ex)
    {
      // Just assume the current exception is a system exception, the
      // status can only change when the interceptor changes this
      // and this is only done when the sri_adapter is available. If we
      // don't have an sri_adapter we just rethrow the exception
      PortableInterceptor::ReplyStatus status =
        PortableInterceptor::SYSTEM_EXCEPTION;

#if TAO_HAS_INTERCEPTORS == 1
      server_request.caught_exception (&ex);

      if (interceptor_adapter != 0)
        {
          interceptor_adapter->send_exception (server_request,
                                               the_args,
                                               the_nargs,
                                               servant_upcall,
                                               exceptions,
                                               nexceptions);

          status = server_request.pi_reply_status ();
        }
#endif /* TAO_HAS_INTERCEPTORS */

      if (status == PortableInterceptor::SYSTEM_EXCEPTION
          || status == PortableInterceptor::USER_EXCEPTION)
        {
          if (server_request.collocated ()
               && server_request.operation_details ()->cac () != 0)
            {
              // If we have a cac it will handle the exception and no
              // need to do any further processing
              server_request.operation_details ()->cac ()->handle_corba_exception (
                server_request, &ex);
              return;
            }
          else
            {
              throw;
            }
        }
    }

  if (server_request.response_expected ()
      && !server_request.sync_with_server ())
    {
      server_request.init_reply ();
    }

#if TAO_HAS_INTERCEPTORS == 1
  // Don't bother marshaling inout/out/return values if an interceptor
  // caused a location forward.
  if (!server_request.is_forwarded ())
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
    {
      if (server_request.outgoing ())
        {
          this->post_upcall (server_request, args, nargs);
        }
    }

  if (server_request.collocated ()
    && server_request.operation_details ()->cac () != 0)
    {
      server_request.operation_details ()->cac ()->convert_reply (
        server_request, args, nargs);
    }
}

void
TAO::Upcall_Wrapper::pre_upcall (TAO_InputCDR & cdr,
                                 TAO::Argument * const * args,
                                 size_t nargs)
{
  // Demarshal the operation "in" and "inout" arguments, if any.

  // NOTE:  The TAO::Argument corresponding to the return value is
  //        always the first element in the array, regardless of
  //        whether or not the return type is void.

  try {
    TAO::Argument * const * const begin = args + 1;  // Skip the return value.
    TAO::Argument * const * const end   = args + nargs;

    for (TAO::Argument * const * i = begin; i != end; ++i)
      {
        if (!(*i)->demarshal (cdr))
          {
            TAO_InputCDR::throw_skel_exception (errno);
          }
      }

    cdr.reset_vt_indirect_maps ();
  }
  catch (...) {
    cdr.reset_vt_indirect_maps ();
    throw;
  }
}

void
TAO::Upcall_Wrapper::post_upcall (TAO_ServerRequest& server_request,
                                  TAO::Argument * const * args,
                                  size_t nargs)
{
  TAO_OutputCDR & cdr = (*server_request.outgoing ());
  TAO::Argument * const * const begin = args;
  TAO::Argument * const * const end   = args + nargs;

  try {
    for (TAO::Argument * const * i = begin; i != end; ++i)
      {
        if (!(*i)->marshal (cdr))
          {
            TAO_OutputCDR::throw_skel_exception (errno);
          }
      }

    // Reply body marshaling completed.  No other fragments to send.
    cdr.more_fragments (false);

  #ifdef TAO_HAS_VALUETYPE_OUT_INDIRECTION
    cdr.reset_vt_indirect_maps ();
  #endif
  }
  catch (...) {
#ifdef TAO_HAS_VALUETYPE_OUT_INDIRECTION
    cdr.reset_vt_indirect_maps ();
#endif
    throw;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

