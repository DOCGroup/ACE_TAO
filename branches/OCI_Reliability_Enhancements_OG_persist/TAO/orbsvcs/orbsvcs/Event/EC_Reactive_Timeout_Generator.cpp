// $Id$

#include "orbsvcs/Event/EC_Reactive_Timeout_Generator.h"
#include "ace/Reactor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_EC_Reactive_Timeout_Generator::
      TAO_EC_Reactive_Timeout_Generator (ACE_Reactor *reactor)
 : reactor_ (reactor)
{
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
  this->event_handler_.reactor (0);
}

int
TAO_EC_Reactive_Timeout_Generator::schedule_timer (
      TAO_EC_Timeout_Filter* filter,
      const ACE_Time_Value& delta,
      const ACE_Time_Value& interval)
{
  return this->reactor_->schedule_timer (&this->event_handler_,
                                         static_cast<void*> (filter),
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

TAO_END_VERSIONED_NAMESPACE_DECL
