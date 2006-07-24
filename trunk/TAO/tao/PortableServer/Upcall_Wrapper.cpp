// $Id$

#include "tao/PortableServer/Upcall_Wrapper.h"
#include "tao/PortableServer/Upcall_Command.h"

#if TAO_HAS_INTERCEPTORS == 1
# include "tao/ServerRequestInterceptor_Adapter.h"
# include "tao/PortableInterceptorC.h"
# include "tao/ORB_Core.h"
#endif  /* TAO_HAS_INTERCEPTORS == 1 */

#include "tao/PortableInterceptor.h"

#include "tao/TAO_Server_Request.h"
#include "tao/CDR.h"
#include "tao/Argument.h"
#include "tao/operation_details.h"
#include "ace/Log_Msg.h"

ACE_RCSID (PortableServer,
           Upcall_Wrapper,
           "$Id$")

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

                             ACE_ENV_ARG_DECL
                             )
{
  if (server_request.incoming ())
    {
      this->pre_upcall (*server_request.incoming (),
                        args,
                        nargs
                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
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

  ACE_TRY
    {
      {
        if (interceptor_adapter != 0)
          {
            // Invoke intermediate server side interception points.
            interceptor_adapter->receive_request (server_request,
                                                  the_args,
                                                  the_nargs,
                                                  servant_upcall,
                                                  exceptions,
                                                  nexceptions
                                                  ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;
          }

        // Don't bother performing the upcall if an interceptor caused a
        // location forward.
        CORBA::Object_var forward_to = server_request.forward_location ();
        if (CORBA::is_nil (forward_to.in ()))
          {
            if (interceptor_adapter != 0)
              {
                interceptor_adapter->execute_command (server_request,
                                                      command
                                                      ACE_ENV_ARG_PARAMETER);
                ACE_TRY_CHECK;
              }
            else
#endif /* TAO_HAS_INTERCEPTORS */
              {
                // The actual upcall.
                command.execute (ACE_ENV_SINGLE_ARG_PARAMETER);
                TAO_INTERCEPTOR_CHECK;
              }

#if TAO_HAS_INTERCEPTORS == 1
          }
      }

      if (interceptor_adapter == 0)
        {
          server_request.reply_status (PortableInterceptor::SUCCESSFUL);
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
          if (CORBA::is_nil (forward_to_after.in ()))
            {
              // No location forward by interceptors and successful upcall.
              server_request.reply_status (PortableInterceptor::SUCCESSFUL);
              interceptor_adapter->send_reply (server_request,
                                               the_args,
                                               the_nargs,
                                               servant_upcall,
                                               exceptions,
                                               nexceptions
                                               ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
        }
    }
  ACE_CATCHANY
    {
      // Just assume the current exception is a system exception, the
      // status can only change when the interceptor changes this
      // and this is only done when the sri_adapter is available. If we
      // don't have an sri_adapter we just rethrow the exception
      PortableInterceptor::ReplyStatus status =
        PortableInterceptor::SYSTEM_EXCEPTION;

      server_request.caught_exception (&ACE_ANY_EXCEPTION);

      if (interceptor_adapter != 0)
        {
          interceptor_adapter->send_exception (server_request,
                                               the_args,
                                               the_nargs,
                                               servant_upcall,
                                               exceptions,
                                               nexceptions
                                               ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          status =
            server_request.reply_status ();
        }

      if (status == PortableInterceptor::SYSTEM_EXCEPTION
          || status == PortableInterceptor::USER_EXCEPTION)
        {
          ACE_RE_THROW;
        }
    }
# if defined (ACE_HAS_EXCEPTIONS) \
  && defined (ACE_HAS_BROKEN_UNEXPECTED_EXCEPTIONS)
  ACE_CATCHALL
    {
      // Just assume the current exception is a system exception, the
      // status can only change when the interceptor changes this
      // and this is only done when the sri_adapter is available. If we
      // don't have an sri_adapter we just rethrow the exception
      PortableInterceptor::ReplyStatus status =
        PortableInterceptor::SYSTEM_EXCEPTION;

      CORBA::UNKNOWN ex;

      server_request.caught_exception (&ex);

      if (interceptor_adapter != 0)
        {
          interceptor_adapter->send_exception (server_request,
                                               the_args,
                                               the_nargs,
                                               servant_upcall,
                                               exceptions,
                                               nexceptions
                                               ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          status =
            server_request.reply_status ();
        }

      if (status == PortableInterceptor::SYSTEM_EXCEPTION)
        ACE_TRY_THROW (ex);
    }
# endif  /* ACE_HAS_EXCEPTIONS && ACE_HAS_BROKEN_UNEXPECTED_EXCEPTIONS */
  ACE_ENDTRY;
  ACE_CHECK;
#endif  /* TAO_HAS_INTERCEPTORS == 1 */

  if (server_request.response_expected ()
      && !server_request.sync_with_server ())
    {
      server_request.init_reply ();
    }

#if TAO_HAS_INTERCEPTORS == 1
  // Don't bother marshaling inout/out/return values if an interceptor
  // caused a location forward.
 CORBA::Object_var forward_to_end = server_request.forward_location ();
  if (CORBA::is_nil (forward_to_end.in ()))
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
    {
      if (server_request.outgoing ())
        {
          this->post_upcall (*server_request.outgoing (),
                             args,
                             nargs
                             ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
    }
}

void
TAO::Upcall_Wrapper::pre_upcall (TAO_InputCDR & cdr,
                                 TAO::Argument * const * args,
                                 size_t nargs
                                 ACE_ENV_ARG_DECL)
{
  // Demarshal the operation "in" and "inout" arguments, if any.

  // NOTE:  The TAO::Argument corresponding to the return value is
  //        always the first element in the array, regardless of
  //        whether or not the return type is void.

  ACE_ASSERT (nargs != 0);

  TAO::Argument * const * const begin = args + 1;  // Skip the return value.
  TAO::Argument * const * const end   = args + nargs;

  for (TAO::Argument * const * i = begin; i != end; ++i)
    {
      if (!(*i)->demarshal (cdr))
        {
          TAO_InputCDR::throw_skel_exception (errno
                                              ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
    }
}

void
TAO::Upcall_Wrapper::post_upcall (TAO_OutputCDR & cdr,
                                  TAO::Argument * const * args,
                                  size_t nargs
                                  ACE_ENV_ARG_DECL)
{
  // Marshal the operation "inout" and "out" arguments and return
  // value, if any.

  TAO::Argument * const * const begin = args;
  TAO::Argument * const * const end   = args + nargs;

  for (TAO::Argument * const * i = begin; i != end; ++i)
    {
      if (!(*i)->marshal (cdr))
        {
          TAO_OutputCDR::throw_skel_exception (errno
                                               ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
    }

 // Reply body marshaling completed.  No other fragments to send.
  cdr.more_fragments (false);
}

TAO_END_VERSIONED_NAMESPACE_DECL

