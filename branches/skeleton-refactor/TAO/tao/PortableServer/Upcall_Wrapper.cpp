// $Id$

#include "Upcall_Wrapper.h"

#include "Upcall_Command.h"

#if TAO_HAS_INTERCEPTORS == 1
# include "PICurrent_Guard.h"

# include "tao/PortableInterceptor.h"
# include "tao/PortableServer/ServerRequestInfo.h"
# include "tao/PortableServer/ServerInterceptorAdapter.h"

#endif  /* TAO_HAS_INTERCEPTORS == 1 */

#include "tao/TAO_Server_Request.h"
#include "tao/CDR.h"


ACE_RCSID (PortableServer,
           Upcall_Wrapper,
           "$Id$")


bool
TAO::Upcall_Wrapper::upcall (TAO_ServerRequest & server_request,
                             TAO::Argument * args[],
                             size_t nargs,
                             TAO::Upcall_Command & command

#if TAO_HAS_INTERCEPTORS == 1
                             , void * servant_upcall
                             , PortableServer::ServantBase * servant
                             , CORBA::TypeCode_ptr * exceptions
                             , size_t nexceptions
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
                             )
{
  if (!this->pre_upcall (server_request.incoming (),
                         args,
                         nargs))
    return false;

#if TAO_HAS_INTERCEPTORS == 1

  TAO::ServerRequestInfo request_info (server_request,
                                       args,
                                       nargs,
                                       servant_upcall,
                                       servant,
                                       exceptions,
                                       nexceptions);

  TAO::ServerRequestInterceptor_Adapter interceptor_adapter (server_request);

  ACE_TRY
    {
      TAO::PICurrent_Guard pi_guard (server_request,
                                     true  /* Copy TSC to RSC */);

      // Invoke intermediate server side interception points.
      interceptor_adapter.receive_request (&request_info
                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!interceptor_adapter.location_forwarded ())
        {
#endif /* TAO_HAS_INTERCEPTORS */

          // The actual upcall.
          command.execute (ACE_ENV_SINGLE_ARG_PARAMETER);
          TAO_INTERCEPTOR_CHECK_RETURN (false);

#if TAO_HAS_INTERCEPTORS == 1
        }

      if (!interceptor_adapter.location_forwarded ())
        {
          request_info.reply_status (PortableInterceptor::SUCCESSFUL);
          interceptor_adapter.send_reply (&request_info
                                          ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      request_info.exception (&ACE_ANY_EXCEPTION);
      interceptor_adapter.send_exception (&request_info
                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableInterceptor::ReplyStatus status =
        request_info.reply_status (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

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
      CORBA::UNKNOWN ex;

      request_info.exception (&ex);
      interceptor_adapter.send_exception (&request_info
                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableInterceptor::ReplyStatus status =
        request_info.reply_status (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (status == PortableInterceptor::SYSTEM_EXCEPTION)
        ACE_TRY_THROW (ex);
    }
# endif  /* ACE_HAS_EXCEPTIONS && ACE_HAS_BROKEN_UNEXPECTED_EXCEPTIONS */
  ACE_ENDTRY;
  ACE_CHECK_RETURN (false);
#endif  /* TAO_HAS_INTERCEPTORS == 1 */

  server_request.init_reply ();

  return 
    TAO_INTERCEPTOR (interceptor_adapter.location_forwarded () ||)
    this->post_upcall (server_request.outgoing (),
                       args,
                       nargs);
}

bool
TAO::Upcall_Wrapper::pre_upcall (TAO_InputCDR & cdr,
                                 TAO::Argument ** args,
                                 size_t nargs)
{
  // Demarshal the operation "in" and "inout" arguments, if any.

  // NOTE:  The TAO::Argument corresponding to the return value is
  //        always the first element in the array, regardless of
  //        whether or not the return type is void.

  ACE_ASSERT (nargs != 0);

  TAO::Argument ** const begin = args + 1;  // Skip the return value.
  TAO::Argument ** const end   = args + nargs;

  ACE_DECLARE_NEW_CORBA_ENV;

  for (TAO::Argument ** i = begin; i != end; ++i)
    {
      if (!(*i)->demarshal (cdr))
        {
          TAO_InputCDR::throw_skel_exception (errno
                                              ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (false);
        }
    }

  return true;
}

bool
TAO::Upcall_Wrapper::post_upcall (TAO_OutputCDR & cdr,
                                  TAO::Argument ** args,
                                  size_t nargs)
{
  // Marshal the operation "inout" and "out" arguments and return
  // value, if any.

  TAO::Argument ** const begin = args;
  TAO::Argument ** const end   = args + nargs;

  ACE_DECLARE_NEW_CORBA_ENV;

  for (TAO::Argument ** i = begin; i != end; ++i)
    {
      if (!(*i)->marshal (cdr))
        {
          TAO_OutputCDR::throw_skel_exception (errno
                                               ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (false);
        }
    }

  return true;
}
