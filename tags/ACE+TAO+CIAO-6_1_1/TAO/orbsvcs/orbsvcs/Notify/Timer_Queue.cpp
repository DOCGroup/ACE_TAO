// $Id$

#include "orbsvcs/Notify/Timer_Queue.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_Timer_Queue::TAO_Notify_Timer_Queue (void)
{
}

TAO_Notify_Timer_Queue::~TAO_Notify_Timer_Queue ()
{
}

void
TAO_Notify_Timer_Queue::release (void)
{
  delete this;
  //@@ inform factory
}

long
TAO_Notify_Timer_Queue::schedule_timer (ACE_Event_Handler *handler,
                                    const ACE_Time_Value &delay_time,
                                    const ACE_Time_Value &interval)
{
  return
    this->timer_queue_.schedule (handler,
                                 0,
                                 timer_queue_.gettimeofday () + delay_time,
                                 interval);
}

int
TAO_Notify_Timer_Queue::cancel_timer (long timer_id)
{
  return this->timer_queue_.cancel (timer_id);
}

ACE_Timer_Queue&
TAO_Notify_Timer_Queue::impl (void)
{
  return this->timer_queue_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
