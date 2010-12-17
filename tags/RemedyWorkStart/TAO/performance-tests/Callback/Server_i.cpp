// $Id$

#include "Server_i.h"

#if !defined(__ACE_INLINE__)
#include "Server_i.inl"
#endif /* __ACE_INLINE__ */

void
Server_i::set_callback (Test::Callback_ptr callback)
{
  this->callback_ = Test::Callback::_duplicate (callback);
}

void
Server_i::request (Test::TimeStamp time_stamp,
                   const Test::Payload &payload)
{
  if (CORBA::is_nil (this->callback_.in ()))
    return;

  this->callback_->response (time_stamp, payload);
}

void
Server_i::shutdown (void)
{
  this->done_ = 1;
}
