//
// $Id$
//
#include "Sender.h"

Sender::Sender (CORBA::ORB_ptr orb)
  :  message_count_ (0)
  ,  byte_count_ (0)
  ,  orb_ (CORBA::ORB::_duplicate (orb))
  ,  is_done_ (false)
{
}

void
Sender::dump_results (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
  ACE_DEBUG ((LM_DEBUG,
              "Total messages = %d\n"
              "Total bytes = %d\n",
              this->message_count_,
              this->byte_count_));
}

bool
Sender::is_done (void) const
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
                    0);

  ++this->message_count_;
  payload =
    new Test::Payload (size);
  payload->length (size);
  this->byte_count_ += size;

  return 1;
}

CORBA::Long
Sender::get_event_count (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    ace_mon,
                    this->mutex_,
                    0);
  return this->message_count_;
}


void
Sender::ping (void)
{
  return;
}

void
Sender::shutdown (void)
{
  if (this->is_done_ == false)
    {
      ACE_GUARD (TAO_SYNCH_MUTEX,
                 ace_mon,
                 this->mutex_);

      if (this->is_done_ == false)
        this->is_done_ = true;
    }
}
