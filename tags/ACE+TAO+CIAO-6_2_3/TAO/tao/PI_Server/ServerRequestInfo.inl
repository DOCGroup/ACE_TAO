// -*- C++ -*-
//
// $Id$

#include "tao/PortableServer/Servant_Upcall.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO::ServerRequestInfo::ServerRequestInfo (
  TAO_ServerRequest & server_request,
  TAO::Argument * const * args,
  size_t nargs,
  TAO::Portable_Server::Servant_Upcall *servant_upcall,
  CORBA::TypeCode_ptr const * exceptions,
  CORBA::ULong nexceptions)
  : server_request_ (server_request)
  , args_ (args)
  , nargs_ (nargs)
  , servant_upcall_ (servant_upcall)
  , exceptions_ (exceptions)
  , nexceptions_ (nexceptions)
{
}

ACE_INLINE void
TAO::ServerRequestInfo::forward_reference (
  PortableInterceptor::ForwardRequest &exc)
{
  // Note that we're converting the ForwardRequest exception in to a
  // LOCATION_FORWARD reply, so we do not set the exception status.

  this->server_request_.pi_reply_status (PortableInterceptor::LOCATION_FORWARD);

  // Store the forward reference in the TAO_ServerRequest object.
  this->server_request_.forward_location (exc.forward.in ());
}

ACE_INLINE void
TAO::ServerRequestInfo::forward_reference (CORBA::Object_ptr obj)
{
  // We only get here if a servant manager threw a
  // PortableServer::ForwardRequest exception.

  this->server_request_.pi_reply_status (PortableInterceptor::LOCATION_FORWARD);

  // Store the forward reference in the TAO_ServerRequest object.
  this->server_request_.forward_location (obj);
}

ACE_INLINE TAO_ServerRequest &
TAO::ServerRequestInfo::server_request (void)
{
  return this->server_request_;
}


TAO_END_VERSIONED_NAMESPACE_DECL
