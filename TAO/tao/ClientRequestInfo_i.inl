// -*- C++ -*-
//
// $Id$

ACE_INLINE void
TAO_ClientRequestInfo_i::exception (CORBA::Exception *exception)
{
  if (CORBA::SystemException::_downcast (exception) != 0)
    this->reply_status_ = PortableInterceptor::SYSTEM_EXCEPTION;
  else if (CORBA::UserException::_downcast (exception) != 0)
    this->reply_status_ = PortableInterceptor::USER_EXCEPTION;

  // @@ Is it possible for both of the above downcasts to fail?

  this->caught_exception_ = exception;
}

ACE_INLINE void
TAO_ClientRequestInfo_i::response_expected (CORBA::Boolean flag)
{
  this->response_expected_ = flag;
}

ACE_INLINE void
TAO_ClientRequestInfo_i::forward_reference (
  PortableInterceptor::ForwardRequest &)
{
  // Note that we're converting the ForwardRequest exception in to a
  // LOCATION_FORWARD reply, so we do not set the exception status.
  //
  // The forward object reference is not handled here.  Rather, it is
  // handled by the TAO_GIOP_Invocation object so that its profiles
  // can be added to the list of forward profiles.

  this->reply_status_ = PortableInterceptor::LOCATION_FORWARD;
}
