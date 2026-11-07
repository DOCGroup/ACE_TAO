#include "Receiver_i.h"

void
Receiver_i::receive_payload (Receiver::Payload const &payload)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
  this->message_count_++;
  this->byte_count_ += payload.length ();
}

CORBA::Long
Receiver_i::get_event_count ()
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->mutex_, 0);
  return this->message_count_;
}
