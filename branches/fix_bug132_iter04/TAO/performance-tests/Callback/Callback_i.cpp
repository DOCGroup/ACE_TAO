// $Id$

#include "Callback_i.h"

#if !defined(__ACE_INLINE__)
#include "Callback_i.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Callback, Callback_i, "$Id$")

int
Callback_i::done (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->mutex_, 0);
  return this->remaining_samples_ == 0;
}

void
Callback_i::response (Test::TimeStamp time_stamp,
                      const Test::Payload &,
                      CORBA::Environment &)
  ACE_THROW_SPEC (())
{
  ACE_hrtime_t now = ACE_OS::gethrtime ();
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
  if (this->remaining_samples_ == 0)
    return;

  if (this->remaining_samples_ % 1000 == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Only %d messages to go\n",
                  this->remaining_samples_));
    }
  this->remaining_samples_--;
  this->history_.sample (now - time_stamp);
}
