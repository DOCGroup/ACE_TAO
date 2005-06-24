// -*- C++ -*-
//
// $Id$

#include "SystemException.h"

ACE_INLINE void
TAO_ClientRequestInfo::check_validity (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->invocation_ == 0)
    ACE_THROW (CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 14,
                                     CORBA::COMPLETED_NO));
}

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
TAO_ClientRequestInfo::forward_reference (
  PortableInterceptor::ForwardRequest &)
{
  // Note that we're converting the ForwardRequest exception in to a
  // LOCATION_FORWARD reply, so we do not set the exception status.
  //
  // The forward object reference is not handled here.  Rather, it is
  // handled by the TAO::Invocation_Base object so that its profiles
  // can be added to the list of forward profiles.

  this->reply_status_ = PortableInterceptor::LOCATION_FORWARD;
}









