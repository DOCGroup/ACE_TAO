// $Id$

#include "DII_Invocation.h"
#include "Unknown_User_Exception.h"
#include "DII_Arguments.h"
#include "ExceptionList.h"
#include "Request.h"
#include "DII_Reply_Dispatcher.h"
#include "tao/operation_details.h"
#include "tao/RequestInfo_Util.h"
#include "tao/Invocation_Utils.h"
#include "tao/debug.h"
#include "tao/Any_Unknown_IDL_Type.h"
#include "tao/Profile_Transport_Resolver.h"

#if !defined (__ACE_INLINE__)
# include "DII_Invocation.inl"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID (DynamicInterface,
           DII_Invocation,
           "$Id$")

namespace TAO
{
  DII_Invocation::DII_Invocation (CORBA::Object_ptr otarget,
                                  Profile_Transport_Resolver &resolver,
                                  TAO_Operation_Details &detail,
                                  CORBA::ExceptionList *excp,
                                  CORBA::Request_ptr req,
                                  bool response_expected)
    : Synch_Twoway_Invocation (otarget,
                               resolver,
                               detail,
                               response_expected)
      , excp_list_ (excp)
      , host_ (req)
  {
  }

#if TAO_HAS_INTERCEPTORS == 1
  Dynamic::ParameterList *
  DII_Invocation::arguments (ACE_ENV_SINGLE_ARG_DECL)
  {
    // Generate the argument list on demand.
    Dynamic::ParameterList *parameter_list =
      TAO_RequestInfo_Util::make_parameter_list (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);

    Dynamic::ParameterList_var safe_parameter_list = parameter_list;

    TAO::Argument **args =
      this->details_.args ();

    if (this->details_.args_num () > 1)
      {
        // Take the second argument since the first is a return value.
        TAO::NVList_Argument *tmp_arg =
          dynamic_cast <TAO::NVList_Argument*> (args[1]);

        tmp_arg->interceptor_paramlist (parameter_list);
      }

    return safe_parameter_list._retn ();
  }
#endif /*TAO_HAS_INTERCEPTORS*/

  Invocation_Status
  DII_Invocation::remote_invocation (ACE_Time_Value *max_wait_time
                                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::Exception))
  {
    return Synch_Twoway_Invocation::remote_twoway (max_wait_time
                                                   ACE_ENV_ARG_PARAMETER);
  }

  Invocation_Status
  DII_Invocation::handle_user_exception (TAO_InputCDR &cdr
                                         ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::Exception))
  {
    Reply_Guard mon (this,
                     TAO_INVOKE_FAILURE);

    if (TAO_debug_level > 3)
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - DII_Invocation::"
                  "handle_user_exception \n"));

    // Match the exception interface repository id with the
    // exception in the exception list.
    // This is important to decode the exception.
    CORBA::String_var buf;

    TAO_InputCDR tmp_stream (cdr,
                             cdr.start ()->length (),
                             0);

    // Pull the exception ID out of the marshaling buffer.
    if (tmp_stream.read_string (buf.inout ()) == 0)
      {
        ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                          CORBA::COMPLETED_YES),
                          TAO_INVOKE_FAILURE);
      }

    for (CORBA::ULong i = 0;
         this->excp_list_ != 0 && i < this->excp_list_->count ();
         i++)
      {
          CORBA::TypeCode_ptr tcp =
            this->excp_list_->item (i
                                    ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

          const char *xid = tcp->id (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

          if (ACE_OS::strcmp (buf.in (), xid) != 0)
            {
              continue;
            }

          CORBA::Any any;
          TAO::Unknown_IDL_Type *unk = 0;
          ACE_NEW_RETURN (unk,
                          TAO::Unknown_IDL_Type (
                              tcp,
                              cdr.start (),
                              cdr.byte_order ()
                            ),
                          TAO_INVOKE_FAILURE);

          any.replace (unk);

          mon.set_status (TAO_INVOKE_USER_EXCEPTION);

          ACE_THROW_RETURN (CORBA::UnknownUserException (any),
                            TAO_INVOKE_USER_EXCEPTION);
        }

    // If we couldn't find the right exception, report it as
    // CORBA::UNKNOWN.

    // But first, save the user exception in case we
    // are being used in a TAO gateway.
    this->host_->raw_user_exception (cdr);

    mon.set_status (TAO_INVOKE_USER_EXCEPTION);

    // @@ It would seem that if the remote exception is a
    //    UserException we can assume that the request was
    //    completed.
    ACE_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_YES),
                      TAO_INVOKE_USER_EXCEPTION);

  }


//***************************************************************************
  DII_Deferred_Invocation::DII_Deferred_Invocation (
      CORBA::Object_ptr otarget,
      Profile_Transport_Resolver &resolver,
      TAO_Operation_Details &detail,
      TAO_DII_Deferred_Reply_Dispatcher *rd,
      CORBA::Request_ptr req,
      bool response_expected)
    : Asynch_Remote_Invocation (otarget,
                                resolver,
                                detail,
                                rd,
                                response_expected)
      , host_ (req)
  {

  }

#if TAO_HAS_INTERCEPTORS == 1
  //@NOTE: Need to figure a way to share this code
  Dynamic::ParameterList *
  DII_Deferred_Invocation::arguments (ACE_ENV_SINGLE_ARG_DECL)
  {
    // Generate the argument list on demand.
    Dynamic::ParameterList *parameter_list =
      TAO_RequestInfo_Util::make_parameter_list (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);

    Dynamic::ParameterList_var safe_parameter_list = parameter_list;

    TAO::Argument **args =
      this->details_.args ();

    if (this->details_.args_num () > 1)
      {
        // Take the second argument since the first is a return value.
        TAO::NVList_Argument *tmp_arg =
          dynamic_cast <TAO::NVList_Argument*> (args[1]);

        tmp_arg->interceptor_paramlist (parameter_list);
      }

    return safe_parameter_list._retn ();
  }
#endif /*TAO_HAS_INTERCEPTORS*/

  Invocation_Status
  DII_Deferred_Invocation::remote_invocation (
      ACE_Time_Value *max_wait_time
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::Exception))
  {
    this->rd_->transport (this->resolver_.transport ());

    return Asynch_Remote_Invocation::remote_invocation (
        max_wait_time
        ACE_ENV_ARG_PARAMETER);
  }
}
