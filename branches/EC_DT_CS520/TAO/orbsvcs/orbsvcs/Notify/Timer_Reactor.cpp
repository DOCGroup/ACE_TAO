// $Id$

#include "Timer_Reactor.h"

#if ! defined (__ACE_INLINE__)
#include "Timer_Reactor.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_Notify_Timer_Reactor, "$Id$")

#include "ace/Reactor.h"
#include "tao/ORB_Core.h"
#include "Properties.h"

TAO_Notify_Timer_Reactor::TAO_Notify_Timer_Reactor (void)
  :reactor_ (0)
{
  // Get the ORB
  CORBA::ORB_var orb = TAO_Notify_PROPERTIES::instance()->orb ();

  this->reactor_ =  orb->orb_core ()->reactor ();
}

TAO_Notify_Timer_Reactor::~TAO_Notify_Timer_Reactor ()
{
}

void
TAO_Notify_Timer_Reactor::release (void)
{
  delete this;
  //@@ inform factory
}

long
TAO_Notify_Timer_Reactor::schedule_timer (ACE_Event_Handler *handler,
                               const ACE_Time_Value &delay_time,
                               const ACE_Time_Value &interval)
{
  return  this->reactor_->schedule_timer (handler, 0, delay_time, interval);
}

int
TAO_Notify_Timer_Reactor::cancel_timer (long timer_id)
{
  return this->reactor_->cancel_timer (timer_id);
}
