#include "Invocation_Base.h"
#include "Stub.h"
#include "operation_details.h"
#include "ORB_Core.h"
#include "Typecode.h"
#include "DynamicC.h"

#if TAO_HAS_INTERCEPTORS == 1
#include "RequestInfo_Util.h"
#endif /*TAO_HAS_INTERCEPTORS*/

#if !defined (__ACE_INLINE__)
# include "Invocation_Base.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           Invocation_Base,
           "$Id$")

namespace TAO
{
  Invocation_Base::Invocation_Base (CORBA::Object_ptr ot,
                                    CORBA::Object_ptr t,
                                    TAO_Stub *stub,
                                    TAO_Operation_Details &details,
                                    bool response_expected)
    : details_ (details)
    , forwarded_to_ (0)
    , response_expected_ (response_expected)
    , otarget_ (ot)
    , target_ (t)
    , orb_core_ (stub->orb_core ())
    , stub_ (stub)
#if TAO_HAS_INTERCEPTORS == 1
    , adapter_ (orb_core_->client_request_interceptors (),
                this)
    , req_info_ (this)
#endif /*TAO_HAS_INTERCEPTORS == 1*/
  {
    ACE_UNUSED_ARG (response_expected);
  }

  Invocation_Base::~Invocation_Base (void)
  {
  }

  TAO_Service_Context &
  Invocation_Base::request_service_context (void)
  {
    return this->details_.request_service_context ();
  }

  TAO_Service_Context &
  Invocation_Base::reply_service_context (void)
  {
    return this->details_.reply_service_context ();
  }

#if TAO_HAS_INTERCEPTORS == 1

  char *
  Invocation_Base::operation_name (void)
  {
    return ACE_const_cast (char *, this->details_.opname ());
  }

  Dynamic::ParameterList *
  Invocation_Base::arguments (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    // Generate the argument list on demand.
    Dynamic::ParameterList *parameter_list =
      TAO_RequestInfo_Util::make_parameter_list (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);

    Dynamic::ParameterList_var safe_parameter_list = parameter_list;

    if (this->details_.parameter_list (*parameter_list) == false)
      ACE_THROW_RETURN (CORBA::MARSHAL (),
                        0);

    return safe_parameter_list._retn ();
  }

  Dynamic::ExceptionList *
  Invocation_Base::exceptions (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    // Generate the argument list on demand.
    Dynamic::ExceptionList *exception_list =
      TAO_RequestInfo_Util::make_exception_list (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);

    Dynamic::ExceptionList_var safe_exception_list = exception_list;

    if (this->details_.exception_list (*exception_list) == false)
      ACE_THROW_RETURN (CORBA::MARSHAL (),
                        0);

    return safe_exception_list._retn ();
  }

  CORBA::Any *
  Invocation_Base::result (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    // Generate the result on demand.
    CORBA::Boolean tk_void_any = 0;
    CORBA::Any *result_any =
      TAO_RequestInfo_Util::make_any (tk_void_any ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);

    CORBA::Any_var safe_result_any = result_any;

    if (this->details_.result (result_any) == false)
      ACE_THROW_RETURN (CORBA::MARSHAL (),
                        0);

    return safe_result_any._retn ();
  }

  CORBA::Octet
  Invocation_Base::sync_scope (void) const
  {
    return this->details_.response_flags ();
  }

  Invocation_Status
  Invocation_Base::send_request_interception (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ACE_TRY
      {
        this->adapter_.send_request (&this->req_info_
                                     ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }
    ACE_CATCHANY
      {
        (void) this->handle_any_exception (&ACE_ANY_EXCEPTION
                                           ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        // This is a begin interception point
        ACE_RE_THROW;
      }
# if defined (ACE_HAS_EXCEPTIONS) \
     && defined (ACE_HAS_BROKEN_UNEXPECTED_EXCEPTIONS)
    ACE_CATCHALL
      {
        (void) this->handle_all_exception (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;

        // This is a begin interception point
        ACE_RE_THROW;
      }
# endif  /* ACE_HAS_EXCEPTIONS && ACE_HAS_BROKEN_UNEXPECTED_EXCEPTIONS */
    ACE_ENDTRY;
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

    if (this->forwarded_to_.in ())
      return TAO_INVOKE_RESTART;

    // What are the other cases??
    return TAO_INVOKE_SUCCESS;
  }

  Invocation_Status
  Invocation_Base::receive_reply_interception (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ACE_TRY
      {
        this->adapter_.receive_reply (&this->req_info_
                                      ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }
    ACE_CATCHANY
      {
        (void) this->handle_any_exception (&ACE_ANY_EXCEPTION
                                           ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        ACE_RE_THROW;
      }
# if defined (ACE_HAS_EXCEPTIONS) \
     && defined (ACE_HAS_BROKEN_UNEXPECTED_EXCEPTIONS)
    ACE_CATCHALL
      {
        (void) this->handle_all_exception (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;

        ACE_RE_THROW;
      }
# endif  /* ACE_HAS_EXCEPTIONS && ACE_HAS_BROKEN_UNEXPECTED_EXCEPTIONS */
    ACE_ENDTRY;
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

    PortableInterceptor::ReplyStatus status =
      this->req_info_.reply_status (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

    if (status == PortableInterceptor::LOCATION_FORWARD ||
        status == PortableInterceptor::TRANSPORT_RETRY)
      return TAO_INVOKE_RESTART;

    return TAO_INVOKE_SUCCESS;
  }


  Invocation_Status
  Invocation_Base::receive_other_interception (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ACE_TRY
      {
        this->adapter_.receive_other (&this->req_info_
                                      ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }
    ACE_CATCHANY
      {
        (void) this->handle_any_exception (&ACE_ANY_EXCEPTION
                                           ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        ACE_RE_THROW;
      }
# if defined (ACE_HAS_EXCEPTIONS) \
     && defined (ACE_HAS_BROKEN_UNEXPECTED_EXCEPTIONS)
    ACE_CATCHALL
      {
        (void) this->handle_all_exception (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;

        ACE_RE_THROW;
      }
# endif  /* ACE_HAS_EXCEPTIONS && ACE_HAS_BROKEN_UNEXPECTED_EXCEPTIONS */
    ACE_ENDTRY;
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

    if (this->forwarded_to_.in ())
      return TAO_INVOKE_RESTART;

    return TAO_INVOKE_SUCCESS;
  }

  PortableInterceptor::ReplyStatus
  Invocation_Base::handle_any_exception (CORBA::Exception *ex
                                         ACE_ENV_ARG_DECL)
  {
    this->req_info_.exception (ex);

    this->adapter_.receive_exception (&this->req_info_
                                      ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (PortableInterceptor::UNKNOWN);

    PortableInterceptor::ReplyStatus status =
      this->req_info_.reply_status (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (status);

    return status;
  }

  PortableInterceptor::ReplyStatus
  Invocation_Base::handle_all_exception (ACE_ENV_SINGLE_ARG_DECL)
  {
    CORBA::UNKNOWN ex;

    this->req_info_.exception (&ex);

    this->adapter_.receive_exception (&this->req_info_
                                      ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (PortableInterceptor::UNKNOWN);

    PortableInterceptor::ReplyStatus status =
      this->req_info_.reply_status (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (status);

    return status;
  }

#endif /*TAO_HAS_INTERCEPTORS == 1*/
}
