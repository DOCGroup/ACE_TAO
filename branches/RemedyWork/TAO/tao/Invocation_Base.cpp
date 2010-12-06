// $Id$

#include "tao/Invocation_Base.h"
#include "tao/Stub.h"
#include "tao/operation_details.h"
#include "tao/ORB_Core.h"
#include "tao/SystemException.h"
#include "tao/PortableInterceptor.h"  /* Must always be visible. */

#include "ace/Dynamic_Service.h"

#if TAO_HAS_INTERCEPTORS == 1
# include "tao/PortableInterceptorC.h"
#endif /* TAO_HAS_INTERCEPTORS == 1 */

#if !defined (__ACE_INLINE__)
# include "tao/Invocation_Base.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace
{
  // Exception used to represent non-CORBA exceptions.  A global
  // instance is used since it will never be modified.
  CORBA::UNKNOWN /* const */ unknown_exception;
}

namespace TAO
{
  Invocation_Base::Invocation_Base (CORBA::Object_ptr ot,
                                    CORBA::Object_ptr t,
                                    TAO_Stub *stub,
                                    TAO_Operation_Details &details,
                                    bool response_expected,
                                    bool TAO_INTERCEPTOR (request_is_remote))
    : details_ (details)
    , forwarded_to_ (0)
    , response_expected_ (response_expected)
    , reply_status_ (GIOP::NO_EXCEPTION)
    , otarget_ (ot)
    , target_ (t)
    , stub_ (stub)
#if TAO_HAS_INTERCEPTORS == 1
    , cri_adapter_ (stub_->orb_core ()->clientrequestinterceptor_adapter ())
    , sri_adapter_ (stub_->orb_core ()->serverrequestinterceptor_adapter ())
    , stack_size_ (0)
    , invoke_status_ (TAO_INVOKE_START)
    , caught_exception_ (0)
    , is_remote_request_ (request_is_remote)
#endif /* TAO_HAS_INTERCEPTORS == 1 */
  {
  }

  Invocation_Base::~Invocation_Base (void)
  {
    TAO_INTERCEPTOR (cri_adapter_= 0);
    TAO_INTERCEPTOR (sri_adapter_= 0);
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
  Invocation_Base::send_request_interception (void)
  {
    if (cri_adapter_)
      {
        try
          {
            // This is a begin interception point
            this->cri_adapter_->send_request (*this);
          }
        catch ( ::CORBA::Exception& ex)
          {
            (void) this->handle_any_exception (&ex);
            throw;
          }
        catch (...)
          {
            (void) this->handle_all_exception ();
            throw;
          }

        if (this->reply_status_ == GIOP::LOCATION_FORWARD)
          return TAO_INVOKE_RESTART;

        this->cri_adapter_->pushTSC (this->stub_->orb_core ());
      }
    else if (sri_adapter_)
      {
        this->sri_adapter_->pushTSC (this->stub_->orb_core ());
      }

    // What are the other cases??
    return TAO_INVOKE_SUCCESS;
  }

  Invocation_Status
  Invocation_Base::receive_reply_interception (void)
  {
    if (cri_adapter_)
      {
        try
          {
            this->cri_adapter_->popTSC (this->stub_->orb_core ());
            this->cri_adapter_->receive_reply (*this);
          }
        catch ( ::CORBA::Exception& ex)
          {
            (void) this->handle_any_exception (&ex);
            throw;
          }
        catch (...)
          {
            (void) this->handle_all_exception ();
            throw;
          }

        if (this->reply_status_ == GIOP::LOCATION_FORWARD)
          return TAO_INVOKE_RESTART;
      }
    else if (sri_adapter_)
      {
        this->sri_adapter_->popTSC (this->stub_->orb_core ());
      }

    return TAO_INVOKE_SUCCESS;
  }

  Invocation_Status
  Invocation_Base::receive_other_interception (void)
  {
    if (cri_adapter_)
      {
        try
          {
            this->cri_adapter_->popTSC (this->stub_->orb_core ());
            this->cri_adapter_->receive_other (*this);
          }
        catch ( ::CORBA::Exception& ex)
          {
            (void) this->handle_any_exception (&ex);
            throw;
          }
        catch (...)
          {
            (void) this->handle_all_exception ();
            throw;
          }

        if (this->reply_status_ == GIOP::LOCATION_FORWARD)
          return TAO_INVOKE_RESTART;
      }
    else if (sri_adapter_)
      {
        this->sri_adapter_->popTSC (this->stub_->orb_core ());
      }

    return TAO_INVOKE_SUCCESS;
  }

  PortableInterceptor::ReplyStatus
  Invocation_Base::handle_any_exception (CORBA::Exception *ex)
  {
    this->exception (ex);

    PortableInterceptor::ReplyStatus status =
      PortableInterceptor::SYSTEM_EXCEPTION;

    if (cri_adapter_)
      {
        this->cri_adapter_->popTSC (this->stub_->orb_core ());
        this->cri_adapter_->receive_exception (*this);

        if (this->reply_status_ == GIOP::LOCATION_FORWARD)
          {
            status = PortableInterceptor::LOCATION_FORWARD;
          }
        else
          {
            status = this->cri_adapter_->pi_reply_status (*this);
          }
      }
    else if (sri_adapter_)
      {
        this->sri_adapter_->popTSC (this->stub_->orb_core ());
      }

    return status;
  }

  PortableInterceptor::ReplyStatus
  Invocation_Base::handle_all_exception (void)
  {
    this->exception (&unknown_exception);

    PortableInterceptor::ReplyStatus status =
      PortableInterceptor::SYSTEM_EXCEPTION;

    if (cri_adapter_)
      {
        this->cri_adapter_->popTSC (this->stub_->orb_core ());
        this->cri_adapter_->receive_exception (*this);

        status = this->cri_adapter_->pi_reply_status (*this);
      }
    else if (sri_adapter_)
      {
        this->sri_adapter_->popTSC (this->stub_->orb_core ());
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
    this->reply_status_ = GIOP::NO_EXCEPTION;
    this->caught_exception_ = exception;
  }

  PortableInterceptor::ReplyStatus
  Invocation_Base::pi_reply_status (void) const
  {
    if (cri_adapter_)
      {
        return this->cri_adapter_->pi_reply_status (*this);
      }
    else
      {
        return -1;
      }
  }
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
}

TAO_END_VERSIONED_NAMESPACE_DECL
