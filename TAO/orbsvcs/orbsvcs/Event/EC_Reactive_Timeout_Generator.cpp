// $Id$

#include "EC_Reactive_Timeout_Generator.h"
#include "tao/corba.h"

#if ! defined (__ACE_INLINE__)
#include "EC_Reactive_Timeout_Generator.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Reactive_Timeout_Generator, "$Id$")

TAO_EC_Reactive_Timeout_Generator::
      TAO_EC_Reactive_Timeout_Generator (ACE_Reactor *reactor)
 : reactor_ (reactor)
{
  if (this->reactor_ == 0)
    {
      this->reactor_ = TAO_ORB_Core_instance ()->reactor ();
    }
}

TAO_EC_Reactive_Timeout_Generator::~TAO_EC_Reactive_Timeout_Generator (void)
{
}

void
TAO_EC_Reactive_Timeout_Generator::activate (void)
{
}

void
TAO_EC_Reactive_Timeout_Generator::shutdown (void)
{
  this->reactor_->cancel_timer (&this->event_handler_);
}

int
TAO_EC_Reactive_Timeout_Generator::schedule_timer (
      TAO_EC_Timeout_Filter* filter,
      const ACE_Time_Value& delta,
      const ACE_Time_Value& interval)
{
  return this->reactor_->schedule_timer (&this->event_handler_,
                                         ACE_static_cast(void*,filter),
                                         delta,
                                         interval);
}

int
TAO_EC_Reactive_Timeout_Generator::cancel_timer (
      const TAO_EC_QOS_Info&,
      int id)
{
  const void *vp;

  return this->reactor_->cancel_timer (id, &vp);
}

