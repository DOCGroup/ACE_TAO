//
// $Id$
//
#include "Sender.h"

ACE_RCSID(Stack_Recursion,
          Sender,
          "$Id$")

Sender::Sender (void)
  :  message_count_ (0)
  ,  byte_count_ (0)
{
}

void
Sender::dump_results ()
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->mutex_);
  ACE_DEBUG ((LM_DEBUG,
              "Total messages = %d\n"
              "Total bytes = %d\n",
              this->message_count_,
              this->byte_count_));
}

CORBA::Boolean
Sender::get_data (CORBA::ULong size,
                  Test::Payload_out payload
                  ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                    ace_mon,
                    this->mutex_,
                    0);

  this->message_count_++;
  payload =
    new Test::Payload (size);
  payload->length (size);
  this->byte_count_ += size;

  return 1;
}

CORBA::Long
Sender::get_event_count (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                    ace_mon,
                    this->mutex_,
                    0);
  return this->message_count_;
}


void
Sender::ping (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return;
}
