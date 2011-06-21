// $Id$

#include "Callback_i.h"

#if !defined(__ACE_INLINE__)
#include "Callback_i.inl"
#endif /* __ACE_INLINE__ */

#include "ace/OS_NS_time.h"

int
Callback_i::done (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->mutex_, 0);
  return this->remaining_samples_ == 0;
}

void
Callback_i::response (Test::TimeStamp time_stamp,
                      const Test::Payload &)
{
  ACE_hrtime_t now = ACE_OS::gethrtime ();
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
  if (this->remaining_samples_ == 0)
    return;

  this->remaining_samples_--;
  this->history_.sample (ACE_HRTIME_TO_U64(now) - time_stamp);
}
