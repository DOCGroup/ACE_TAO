// $Id$

#include "Server_i.h"

#if !defined(__ACE_INLINE__)
#include "Server_i.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Callback, Server_i, "$Id$")

void
Server_i::set_callback (Test::Callback_ptr callback,
                        CORBA::Environment &)
  ACE_THROW_SPEC (())
{
  this->callback_ = Test::Callback::_duplicate (callback);
}

void
Server_i::request (Test::TimeStamp time_stamp,
                   const Test::Payload &payload,
                   CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (CORBA::is_nil (this->callback_.in ()))
    return;

  this->callback_->response (time_stamp, payload, ACE_TRY_ENV);
}

void
Server_i::shutdown (CORBA::Environment &)
    ACE_THROW_SPEC (())
{
  this->done_ = 1;
}
