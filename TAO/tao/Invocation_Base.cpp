#include "tao/Invocation_Base.h"
#include "tao/Stub.h"
#include "tao/operation_details.h"
#include "tao/ORB_Core.h"
#include "tao/SystemException.h"
#include "tao/PortableInterceptor.h"  /* Must always be visible. */

#include "ace/Dynamic_Service.h"

#if TAO_HAS_INTERCEPTORS == 1
# include "tao/PortableInterceptorC.h"
# include "tao/ClientRequestInterceptor_Adapter_Factory.h"
#endif /* TAO_HAS_INTERCEPTORS == 1*/

#if !defined (__ACE_INLINE__)
# include "tao/Invocation_Base.inl"
#endif /* __ACE_INLINE__ */


ACE_RCSID (tao,
           Invocation_Base,
           "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  Invocation_Base::Invocation_Base (CORBA::Object_ptr ot,
                                    CORBA::Object_ptr t,
                                    TAO_Stub *stub,
                                    TAO_Operation_Details &details,
                                    bool response_expected,
                                    bool request_is_remote)
    : details_ (details)
    , forwarded_to_ (0)
    , response_expected_ (response_expected)
    , otarget_ (ot)
    , target_ (t)
    , orb_core_ (stub->orb_core ())
    , stub_ (stub)
#if TAO_HAS_INTERCEPTORS == 1
    , adapter_ (orb_core_->clientrequestinterceptor_adapter ())
    , stack_size_ (0)
    , invoke_status_ (TAO_INVOKE_START)
    , caught_exception_ (0)
    , is_remote_request_ (request_is_remote)
#endif /*TAO_HAS_INTERCEPTORS == 1*/
  {
#if TAO_HAS_INTERCEPTORS == 0
    ACE_UNUSED_ARG (request_is_remote);
#endif /*TAO_HAS_INTERCEPTORS == 0*/
  }

  Invocation_Base::~Invocation_Base (void)
  {
#if TAO_HAS_INTERCEPTORS == 1
    adapter_ = 0;
#endif /*TAO_HAS_INTERCEPTORS == 1*/
  }

  void
  Invocation_Base::reply_received (Invocation_Status TAO_INTERCEPTOR (s))
  {
    TAO_INTERCEPTOR (invoke_status_ = s);
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

  Invocation_Status
  Invocation_Base::send_request_interception (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    if (adapter_ != 0)
      {
        ACE_TRY
          {
            this->adapter_->send_request (*this
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
      }

    // What are the other cases??
    return TAO_INVOKE_SUCCESS;
  }

  Invocation_Status
  Invocation_Base::receive_reply_interception (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    if (adapter_ != 0)
      {
        ACE_TRY
          {
            this->adapter_->receive_reply (*this
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

        PortableInterceptor::ReplyStatus const status =
          this->adapter_->reply_status (*this);

        if (status == PortableInterceptor::LOCATION_FORWARD ||
            status == PortableInterceptor::TRANSPORT_RETRY)
          return TAO_INVOKE_RESTART;
      }

    return TAO_INVOKE_SUCCESS;
  }


  Invocation_Status
  Invocation_Base::receive_other_interception (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    if (adapter_ != 0)
      {
        ACE_TRY
          {
            this->adapter_->receive_other (*this
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
      }

    return TAO_INVOKE_SUCCESS;
  }

  PortableInterceptor::ReplyStatus
  Invocation_Base::handle_any_exception (CORBA::Exception *ex
                                         ACE_ENV_ARG_DECL)
  {
    this->exception (ex);

    PortableInterceptor::ReplyStatus status =
      PortableInterceptor::SYSTEM_EXCEPTION;

    if (adapter_ != 0)
      {
        this->adapter_->receive_exception (*this
                                           ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (PortableInterceptor::UNKNOWN);

        if (this->forwarded_to_.in ())
          {
            status = PortableInterceptor::LOCATION_FORWARD;
          }
        else
          {
            status = this->adapter_->reply_status (*this);
          }
      }

    return status;
  }

  PortableInterceptor::ReplyStatus
  Invocation_Base::handle_all_exception (ACE_ENV_SINGLE_ARG_DECL)
  {
    CORBA::UNKNOWN ex;
    this->exception (&ex);

    PortableInterceptor::ReplyStatus status =
      PortableInterceptor::SYSTEM_EXCEPTION;

    if (adapter_ != 0)
      {
        this->adapter_->receive_exception (*this
                                           ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (PortableInterceptor::UNKNOWN);

        status =
          this->adapter_->reply_status (*this);
      }

    return status;
  }

  void
  Invocation_Base::exception (CORBA::Exception *exception)
  {
    if (CORBA::SystemException::_downcast (exception) != 0)
      this->invoke_status_ = TAO::TAO_INVOKE_SYSTEM_EXCEPTION;
    else if (CORBA::UserException::_downcast (exception) != 0)
      this->invoke_status_ = TAO::TAO_INVOKE_USER_EXCEPTION;

    this->forwarded_to_ = CORBA::Object::_nil ();      
    this->caught_exception_ = exception;
  }

  PortableInterceptor::ReplyStatus
  Invocation_Base::reply_status (void) const
  {
    if (adapter_ != 0)
      {
        return this->adapter_->reply_status (*this);
      }
    else
      {
        return -1;
      }
  }
#endif /*TAO_HAS_INTERCEPTORS == 1*/
}

TAO_END_VERSIONED_NAMESPACE_DECL
