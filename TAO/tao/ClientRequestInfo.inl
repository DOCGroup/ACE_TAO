// -*- C++ -*-
//
// $Id$

ACE_INLINE void
TAO_ClientRequestInfo::exception (CORBA::Exception *exception)
{
  if (CORBA::SystemException::_downcast (exception) != 0)
    this->reply_status_ = PortableInterceptor::SYSTEM_EXCEPTION;
  else if (CORBA::UserException::_downcast (exception) != 0)
    this->reply_status_ = PortableInterceptor::USER_EXCEPTION;

  // @@ Is it possible for both of the above downcasts to fail?

  this->caught_exception_ = exception;
}

ACE_INLINE void
TAO_ClientRequestInfo::response_expected (CORBA::Boolean flag)
{
  this->response_expected_ = flag;
}

ACE_INLINE void
TAO_ClientRequestInfo::reply_status (int invoke_status)
{
  switch (invoke_status)
    {
    case TAO_INVOKE_OK:
      this->reply_status_ = PortableInterceptor::SUCCESSFUL;
      break;
    case TAO_INVOKE_RESTART:
      if (this->invocation_->received_location_forward ())
        this->reply_status_ = PortableInterceptor::LOCATION_FORWARD;
      else
        this->reply_status_ = PortableInterceptor::TRANSPORT_RETRY;
      break;
    default:
      // We should only get here if the invocation status is
      // TAO_INVOKE_EXCEPTION, i.e. a CORBA::SystemException, so set
      // the appropriate reply status.
      this->reply_status_ = PortableInterceptor::SYSTEM_EXCEPTION;
      break;
    }
}

ACE_INLINE void
TAO_ClientRequestInfo::forward_reference (
  PortableInterceptor::ForwardRequest &exc)
{
  // Note that we're converting the ForwardRequest exception in to a
  // LOCATION_FORWARD reply, so we do not set the exception status.
  //
  // The forward object reference is not handled here.  Rather, it is
  // handled by the TAO_GIOP_Invocation object so that its profiles
  // can be added to the list of forward profiles.

  if (exc.permanent)
    this->reply_status_ =
      PortableInterceptor::LOCATION_FORWARD_PERMANENT;
  else
    this->reply_status_ = 
      PortableInterceptor::LOCATION_FORWARD;
}
