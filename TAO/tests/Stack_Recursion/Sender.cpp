#include "Sender.h"

Sender::Sender (CORBA::ORB_ptr orb)
  :  message_count_ {}
  ,  byte_count_ {}
  ,  orb_ (CORBA::ORB::_duplicate (orb))
  ,  is_done_ {}
{
}

void
Sender::dump_results ()
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
  ACE_DEBUG ((LM_DEBUG,
              "Total messages = %d\n"
              "Total bytes = %d\n",
              this->message_count_,
              this->byte_count_));
}

bool
Sender::is_done () const
{
  return this->is_done_;
}

CORBA::Boolean
Sender::get_data (CORBA::ULong size,
                  Test::Payload_out payload)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    ace_mon,
                    this->mutex_,
                    false);

  ++this->message_count_;
  payload =
    new Test::Payload (size);
  payload->length (size);
  this->byte_count_ += size;

  return true;
}

CORBA::Long
Sender::get_event_count ()
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    ace_mon,
                    this->mutex_,
                    0);
  return this->message_count_;
}


void
Sender::ping ()
{
}

void
Sender::shutdown ()
{
  if (!this->is_done_)
    {
      ACE_GUARD (TAO_SYNCH_MUTEX,
                 ace_mon,
                 this->mutex_);

      if (!this->is_done_)
        this->is_done_ = true;
    }
}
