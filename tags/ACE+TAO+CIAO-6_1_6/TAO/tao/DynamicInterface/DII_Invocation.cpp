// $Id$

#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/AnyTypeCode/DynamicC.h"

#include "tao/DynamicInterface/DII_Invocation.h"
#include "tao/DynamicInterface/Unknown_User_Exception.h"
#include "tao/DynamicInterface/DII_Arguments.h"
#include "tao/DynamicInterface/ExceptionList.h"
#include "tao/DynamicInterface/Request.h"
#include "tao/DynamicInterface/DII_Reply_Dispatcher.h"

#include "tao/operation_details.h"
#include "tao/Invocation_Utils.h"
#include "tao/debug.h"
#include "tao/AnyTypeCode/Any_Unknown_IDL_Type.h"
#include "tao/Profile_Transport_Resolver.h"
#include "tao/ORB_Constants.h"
#include "tao/SystemException.h"

#include "ace/OS_NS_string.h"

#if TAO_HAS_INTERCEPTORS == 1
# include "tao/PI/RequestInfo_Util.h"
#endif

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
  DII_Invocation::arguments (void)
  {
    Dynamic::ParameterList_var safe_parameter_list;

    TAO::Argument ** const args = this->details_.args ();

    if (this->details_.args_num () > 1)
      {
        // Take the second argument since the first is a return value.
        TAO::NVList_Argument * const tmp_arg =
          dynamic_cast <TAO::NVList_Argument*> (args[1]);

        if (tmp_arg)
          {
            // Generate the argument list on demand.
            Dynamic::ParameterList * const parameter_list =
              TAO_RequestInfo_Util::make_parameter_list ();

            safe_parameter_list = parameter_list;

            tmp_arg->interceptor_paramlist (parameter_list);
          }
      }

    return safe_parameter_list._retn ();
  }
#endif /* TAO_HAS_INTERCEPTORS == 1 */

  Invocation_Status
  DII_Invocation::remote_invocation (ACE_Time_Value *max_wait_time)
  {
    return Synch_Twoway_Invocation::remote_twoway (max_wait_time);
  }

  Invocation_Status
  DII_Invocation::handle_user_exception (TAO_InputCDR &cdr)
  {
    Reply_Guard mon (this, TAO_INVOKE_FAILURE);

    if (TAO_debug_level > 3)
      {
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - DII_Invocation::"
                    "handle_user_exception\n"));
      }

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
        throw ::CORBA::MARSHAL (TAO::VMCID, CORBA::COMPLETED_YES);
      }

    for (CORBA::ULong i = 0;
         this->excp_list_ != 0 && i < this->excp_list_->count ();
         i++)
      {
          CORBA::TypeCode_var tc = this->excp_list_->item (i);

          const char *xid = tc->id ();

          if (ACE_OS::strcmp (buf.in (), xid) != 0)
            {
              continue;
            }

          CORBA::Any any;
          TAO::Unknown_IDL_Type *unk = 0;
          ACE_NEW_RETURN (unk,
                          TAO::Unknown_IDL_Type (
                              tc.in (),
                              cdr
                            ),
                          TAO_INVOKE_FAILURE);

          any.replace (unk);

          mon.set_status (TAO_INVOKE_USER_EXCEPTION);

          throw ::CORBA::UnknownUserException (any);
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
    throw ::CORBA::UNKNOWN (TAO::VMCID, CORBA::COMPLETED_YES);

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
  DII_Deferred_Invocation::arguments (void)
  {
    Dynamic::ParameterList_var safe_parameter_list;

    TAO::Argument ** const args = this->details_.args ();

    if (this->details_.args_num () > 1)
      {
        // Take the second argument since the first is a return value.
        TAO::NVList_Argument * const tmp_arg =
          dynamic_cast <TAO::NVList_Argument*> (args[1]);

        if (tmp_arg)
          {
            // Generate the argument list on demand.
            Dynamic::ParameterList * const parameter_list =
              TAO_RequestInfo_Util::make_parameter_list ();

            safe_parameter_list = parameter_list;

            tmp_arg->interceptor_paramlist (parameter_list);
          }
      }

    return safe_parameter_list._retn ();
  }
#endif /* TAO_HAS_INTERCEPTORS == 1*/

  Invocation_Status
  DII_Deferred_Invocation::remote_invocation (ACE_Time_Value *max_wait_time)
  {
    this->safe_rd_->transport (this->resolver_.transport ());

    return Asynch_Remote_Invocation::remote_invocation (max_wait_time);
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
