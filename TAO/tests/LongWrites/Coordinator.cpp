//
// $Id$
//
#include "Coordinator.h"

ACE_RCSID(LongWrites, Coordinator, "$Id$")

Coordinator::Coordinator (void)
  : shutdown_called_ (0)
  , pairs_count_ (0)
  , pairs_length_ (16)
{
  ACE_NEW (this->pairs_, Pair[this->pairs_length_]);
}

Coordinator::~Coordinator (void)
{
  delete[] this->pairs_;
}

void
Coordinator::add_pair (Test::Receiver_ptr receiver,
                       Test::Sender_ptr sender,
                       CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, " new pair "));
  if (this->pairs_length_ == this->pairs_count_)
    {
      Pair *tmp;
      this->pairs_length_ *= 2;
      ACE_NEW (tmp, Pair[this->pairs_length_]);
      for (size_t i = 0; i != this->pairs_count_; ++i)
        tmp[i] = this->pairs_[i];
      delete[] this->pairs_;
      this->pairs_ = tmp;
    }
  this->pairs_[this->pairs_count_].receiver =
    Test::Receiver::_duplicate (receiver);
  this->pairs_[this->pairs_count_].sender =
    Test::Sender::_duplicate (sender);
  this->pairs_count_++;
}

void
Coordinator::start (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  for (size_t i = 0; i != this->pairs_count_; ++i)
    {
      Test::Sender_ptr sender = this->pairs_[i].sender.in ();
      for (size_t j = 0; j != this->pairs_count_; ++j)
        {
          if (i == j)
            continue;
          sender->add_receiver (this->pairs_[j].receiver.in (),
                                ACE_TRY_ENV);
          ACE_CHECK;
        }
    }

  CORBA::ULong event_size = 256 * 1024;
  ACE_DEBUG ((LM_DEBUG, "Running with payload = %d\n",
              event_size));
  for (size_t i = 0; i != this->pairs_count_; ++i)
    {
      this->pairs_[i].sender->send_events (100, event_size,
                                           ACE_TRY_ENV);
      ACE_CHECK;
    }
}

void
Coordinator::shutdown (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  {
    ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->mutex_);
    this->shutdown_called_ = 1;
  }
  for (size_t i = 0; i != this->pairs_count_; ++i)
    {
      this->pairs_[i].sender->shutdown (ACE_TRY_ENV);
      ACE_CHECK;
    }
}
