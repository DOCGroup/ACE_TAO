//
// $Id$
//
#include "Receiver.h"

ACE_RCSID(LongWrites, Receiver, "$Id$")

Receiver::Receiver (void)
  :  message_count_ (0)
  ,  byte_count_ (0)
{
}

CORBA::ULong
Receiver::message_count (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, 0);
  return this->message_count_;
}

void
Receiver::dump_results ()
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->mutex_);
  ACE_DEBUG ((LM_DEBUG,
              "Total messages = %d\n"
              "Total bytes = %d\n",
              this->message_count_,
              this->byte_count_));
}

void
Receiver::receive_data (const Test::Payload &payload,
                        CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->mutex_);
  this->message_count_++;
  this->byte_count_ += payload.length ();

  if (this->message_count_ % 100 == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Receiver::receive_data %d\n",
                  this->message_count_));
    }
}

void
Receiver::receive_data_oneway (const Test::Payload &payload,
                               CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->receive_data (payload, ACE_TRY_ENV);
}

Test::Payload *
Receiver::return_data (const Test::Payload &payload,
                       CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return new Test::Payload (payload);
}
