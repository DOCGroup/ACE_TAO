//
// $Id$
//
#include "Receiver.h"

ACE_RCSID(Muxing, Receiver, "$Id$")

Receiver::Receiver (CORBA::ORB_ptr orb)
  :  message_count_ (0)
  ,  byte_count_ (0)
  , orb_ (CORBA::ORB::_duplicate (orb))
{
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
Receiver::receive_data (const Test::Payload &payload
                        ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->mutex_);
  this->message_count_++;
  this->byte_count_ += payload.length ();
}

CORBA::Long
Receiver::get_event_count (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, 0);
  return this->message_count_;
}

void
Receiver::shutdown (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}
