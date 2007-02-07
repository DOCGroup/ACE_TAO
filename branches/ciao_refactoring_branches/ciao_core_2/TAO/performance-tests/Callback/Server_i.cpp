// $Id$

#include "Server_i.h"

#if !defined(__ACE_INLINE__)
#include "Server_i.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Callback, Server_i, "$Id$")

void
Server_i::set_callback (Test::Callback_ptr callback)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->callback_ = Test::Callback::_duplicate (callback);
}

void
Server_i::request (Test::TimeStamp time_stamp,
                   const Test::Payload &payload)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (CORBA::is_nil (this->callback_.in ()))
    return;

  this->callback_->response (time_stamp, payload);
}

void
Server_i::shutdown (void)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->done_ = 1;
}
