//
// $Id$
//
#include "Sender.h"

ACE_RCSID(Stack_Recursion,
          Sender,
          "$Id$")

Sender::Sender (CORBA::ORB_ptr orb)
  :  message_count_ (0)
  ,  byte_count_ (0)
  ,  orb_ (CORBA::ORB::_duplicate (orb))
  ,  is_done_ (0)
{
}

void
Sender::dump_results (void)
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->mutex_);
  ACE_DEBUG ((LM_DEBUG,
              "Total messages = %d\n"
              "Total bytes = %d\n",
              this->message_count_,
              this->byte_count_));
}

int
Sender::is_done (void) const
{
  return this->is_done_;
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

void
Sender::shutdown (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->is_done_ == 0)
    {
      ACE_GUARD (ACE_SYNCH_MUTEX,
                 ace_mon,
                 this->mutex_);

      if (this->is_done_ == 0)
        this->is_done_ = 1;
    }
}
