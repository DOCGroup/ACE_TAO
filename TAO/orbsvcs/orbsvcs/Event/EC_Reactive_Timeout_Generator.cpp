// $Id$

#include "EC_Reactive_Timeout_Generator.h"

#include "ace/Reactor.h"
#include "ace/Thread_Manager.h"

#if ! defined (__ACE_INLINE__)
#include "EC_Reactive_Timeout_Generator.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID (Event, 
           EC_Reactive_Timeout_Generator, 
           "$Id$")

TAO_EC_Reactive_Timeout_Generator::
      TAO_EC_Reactive_Timeout_Generator (ACE_Reactor *reactor)
 : reactor_ (reactor),
   reactor_impl_(0)
{
  if(!reactor_)
  {
   reactor_impl_ = new ACE_TP_Reactor();
   reactor_ = new ACE_Reactor(reactor_impl_);
  }
}

TAO_EC_Reactive_Timeout_Generator::~TAO_EC_Reactive_Timeout_Generator (void)
{
  if(reactor_impl_)
  {
    reactor_->end_reactor_event_loop();

   delete reactor_;
   delete reactor_impl_;                
  }
}

void
TAO_EC_Reactive_Timeout_Generator::activate (void)
{
  ACE_Thread_Manager::instance()->spawn((ACE_THR_FUNC) run_svc, this);
}

void 
TAO_EC_Reactive_Timeout_Generator::run_svc(void *arg)
{
 TAO_EC_Reactive_Timeout_Generator *obj
   = ACE_static_cast(TAO_EC_Reactive_Timeout_Generator*, arg);

 obj->reactor_->run_reactor_event_loop();

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
