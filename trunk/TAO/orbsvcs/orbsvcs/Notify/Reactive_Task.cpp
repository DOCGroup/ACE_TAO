// $Id$

#include "Reactive_Task.h"

#if ! defined (__ACE_INLINE__)
#include "Reactive_Task.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_Reactive_Task, "$Id$")

#include "Timer_Reactor.h"

TAO_NS_Reactive_Task::TAO_NS_Reactive_Task (void)
  :timer_ (0)
{
}

TAO_NS_Reactive_Task::~TAO_NS_Reactive_Task ()
{
}

void
TAO_NS_Reactive_Task::init (TAO_NS_AdminProperties_var& /*admin_properties*/ ACE_ENV_ARG_DECL)
{
  ACE_NEW_THROW_EX (this->timer_,
                    TAO_NS_Timer_Reactor (),
                    CORBA::NO_MEMORY ());
}

void
TAO_NS_Reactive_Task::shutdown (void)
{
  this->timer_->_decr_refcnt ();
  delete this; //TODO: Release via factory.
}

void
TAO_NS_Reactive_Task::exec (TAO_NS_Method_Request& method_request)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  method_request.execute (ACE_ENV_SINGLE_ARG_PARAMETER);
}

TAO_NS_Timer*
TAO_NS_Reactive_Task::timer (void)
{
  this->timer_->_incr_refcnt ();
  return this->timer_;
}
