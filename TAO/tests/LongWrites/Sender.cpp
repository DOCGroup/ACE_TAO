//
// $Id$
//
#include "Sender.h"

ACE_RCSID(LongWrites, Sender, "$Id$")

Sender::Sender (void)
  : receiver_count_ (0)
  , receiver_length_ (16)
  , shutdown_called_ (0)
{
  ACE_NEW (this->receivers_, Test::Receiver_var[this->receiver_length_]);
}

Sender::~Sender (void)
{
  delete[] this->receivers_;
}


int
Sender::shutdown_called (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, 0);
  return this->shutdown_called_;
}

void
Sender::add_receiver (Test::Receiver_ptr receiver,
                           CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->receiver_count_ == this->receiver_length_)
    {
      this->receiver_length_ *= 2;
      Test::Receiver_var *tmp;
      ACE_NEW (tmp, Test::Receiver_var[this->receiver_length_]);
      for (size_t i = 0; i != this->receiver_count_; ++i)
        tmp[i] = this->receivers_[i];
      delete[] this->receivers_;
      this->receivers_ = tmp;
    }
  this->receivers_[this->receiver_count_++] =
    Test::Receiver::_duplicate (receiver);
}

void
Sender::send_events (CORBA::Long event_count,
                            CORBA::ULong event_size,
                            CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  Test::Payload payload(event_size); payload.length(event_size);
  for (CORBA::Long i = 0; i != event_count; ++i)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - running iteration %d\n", i));
      for (size_t j = 0; j != this->receiver_count_; ++j)
        {
          ACE_TRY
            {
              this->receivers_[j]->receive_data (payload,
                                                 ACE_TRY_ENV);
              ACE_TRY_CHECK;
            }
          ACE_CATCHANY
            {
            }
          ACE_ENDTRY;
        }
    }
}

void
Sender::shutdown (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) shutting down\n"));
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->mutex_);
  this->shutdown_called_ = 1;
}
