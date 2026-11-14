#include "Receiver.h"

Receiver::Receiver (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Receiver::dump_results ()
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
  ACE_DEBUG ((LM_DEBUG,
              "Total messages = %d\n"
              "Total bytes = %d\n",
              this->message_count_,
              this->byte_count_));
}

void
Receiver::receive_data (Test::Payload const &payload)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
  ++this->message_count_;
  this->byte_count_ += payload.length ();
}

CORBA::Long
Receiver::get_event_count ()
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->mutex_, 0);
  return this->message_count_;
}

void
Receiver::shutdown ()
{
  this->orb_->shutdown (false);
}
