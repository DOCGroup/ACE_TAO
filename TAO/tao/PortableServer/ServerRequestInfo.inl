// -*- C++ -*-
//
// $Id$

#include "tao/PortableServer/Servant_Upcall.h"


ACE_INLINE
TAO::ServerRequestInfo::ServerRequestInfo (
  TAO_ServerRequest & server_request,
  TAO::Argument * const * args,
  size_t nargs,
  void * servant_upcall,
  CORBA::TypeCode_ptr const * exceptions,
  size_t nexceptions)
  : server_request_ (server_request)
  , args_ (args)
  , nargs_ (nargs)
  , servant_upcall_ (
      static_cast<TAO::Portable_Server::Servant_Upcall *> (servant_upcall))
  , exceptions_ (exceptions)
  , nexceptions_ (nexceptions)
  , caught_exception_ (0)
  , reply_status_ (-1)
{
}

ACE_INLINE void
TAO::ServerRequestInfo::exception (CORBA::Exception * exception)
{
  if (CORBA::SystemException::_downcast (exception) != 0)
    this->reply_status_ = PortableInterceptor::SYSTEM_EXCEPTION;
  else if (CORBA::UserException::_downcast (exception) != 0)
    this->reply_status_ = PortableInterceptor::USER_EXCEPTION;

  // @@ Is it possible for both of the above downcasts to fail?

  this->caught_exception_ = exception;
}

ACE_INLINE void
TAO::ServerRequestInfo::reply_status (PortableInterceptor::ReplyStatus s)
{
  this->reply_status_ = s;
}

ACE_INLINE void
TAO::ServerRequestInfo::forward_reference (
  PortableInterceptor::ForwardRequest &exc)
{
  // Note that we're converting the ForwardRequest exception in to a
  // LOCATION_FORWARD reply, so we do not set the exception status.

  this->reply_status_ = PortableInterceptor::LOCATION_FORWARD;

  // Store the forward reference in the TAO_ServerRequest object.
  this->server_request_.forward_location (exc.forward.in ());
}

ACE_INLINE void
TAO::ServerRequestInfo::forward_reference (CORBA::Object_ptr obj)
{
  // We only get here if a servant manager threw a
  // PortableServer::ForwardRequest exception.

  this->reply_status_ = PortableInterceptor::LOCATION_FORWARD;

  // Store the forward reference in the TAO_ServerRequest object.
  this->server_request_.forward_location (obj);
}

ACE_INLINE TAO_ServerRequest &
TAO::ServerRequestInfo::server_request (void)
{
  return this->server_request_;
}

