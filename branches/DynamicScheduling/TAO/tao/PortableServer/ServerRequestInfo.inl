// -*- C++ -*-
//
// $Id$

ACE_INLINE void
TAO_ServerRequestInfo::exception (CORBA::Exception *exception)
{
  if (CORBA::SystemException::_downcast (exception) != 0)
    this->reply_status_ = PortableInterceptor::SYSTEM_EXCEPTION;
  else if (CORBA::UserException::_downcast (exception) != 0)
    this->reply_status_ = PortableInterceptor::USER_EXCEPTION;

  // @@ Is it possible for both of the above downcasts to fail?

  this->caught_exception_ = exception;
}

ACE_INLINE void
TAO_ServerRequestInfo::reply_status (PortableInterceptor::ReplyStatus s)
{
  this->reply_status_ = s;
}

ACE_INLINE void
TAO_ServerRequestInfo::forward_reference (
  PortableInterceptor::ForwardRequest &exc)
{
  // Note that we're converting the ForwardRequest exception in to a
  // LOCATION_FORWARD reply, so we do not set the exception status.

  this->reply_status_ = PortableInterceptor::LOCATION_FORWARD;

  // Store the forward reference in the TAO_ServerRequest object.
  this->server_request_.forward_location (exc.forward.in ());
}

ACE_INLINE void
TAO_ServerRequestInfo::forward_reference (CORBA::Object_ptr obj)
{
  // We only get here if a servant manager threw a
  // PortableServer::ForwardRequest exception.

  this->reply_status_ = PortableInterceptor::LOCATION_FORWARD;

  // Store the forward reference in the TAO_ServerRequest object.
  this->server_request_.forward_location (obj);
}

ACE_INLINE TAO_ServerRequest &
TAO_ServerRequestInfo::server_request (void)
{
  return this->server_request_;
}
