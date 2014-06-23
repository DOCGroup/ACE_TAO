// $Id$

#include "orbsvcs/CosEvent/CEC_ProxyPullConsumer.h"
#include "orbsvcs/CosEvent/CEC_EventChannel.h"
#include "orbsvcs/CosEvent/CEC_SupplierAdmin.h"
#include "orbsvcs/CosEvent/CEC_ConsumerAdmin.h"
#include "orbsvcs/CosEvent/CEC_Reactive_Pulling_Strategy.h"

#include "orbsvcs/Time_Utilities.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
#include "tao/Messaging/Messaging.h"
#endif

#include "tao/ORB_Core.h"

#include "ace/Reactor.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/CosEvent/CEC_Reactive_Pulling_Strategy.inl"
#endif /* __ACE_INLINE__ */




TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_CEC_Reactive_Pulling_Strategy::
    TAO_CEC_Reactive_Pulling_Strategy (const ACE_Time_Value &rate,
                                       const ACE_Time_Value &relative_timeout,
                                       TAO_CEC_EventChannel *event_channel,
                                       CORBA::ORB_ptr orb)
  :  adapter_ (this),
     rate_ (rate),
     relative_timeout_ (relative_timeout),
     event_channel_ (event_channel),
     orb_ (CORBA::ORB::_duplicate (orb))
#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
   // Initialise timer_id_ to an invalid timer id, so that in case we don't
   // schedule a timer, we don't cancel a random timer at shutdown
   , timer_id_ (-1)
#endif /* TAO_HAS_CORBA_MESSAGING */
{
  this->reactor_ =
    this->orb_->orb_core ()->reactor ();
}

void
TAO_CEC_Reactive_Pulling_Strategy::handle_timeout (
      const ACE_Time_Value &,
      const void *)
{
  try
    {
      // Query the state of the Current object *before* we initiate
      // the iteration...
      CORBA::PolicyTypeSeq types;
      CORBA::PolicyList_var policies =
        this->policy_current_->get_policy_overrides (types);

      // Change the timeout
      this->policy_current_->set_policy_overrides (this->policy_list_,
                                                   CORBA::ADD_OVERRIDE);

      try
        {
          TAO_CEC_Pull_Event worker (this->event_channel_->consumer_admin (),
                                     this->event_channel_->supplier_control ());

          this->event_channel_->supplier_admin ()->for_each (&worker);
        }
      catch (const CORBA::Exception&)
        {
          // Ignore all exceptions
        }

      this->policy_current_->set_policy_overrides (policies.in (),
                                                   CORBA::SET_OVERRIDE);
      for (CORBA::ULong i = 0; i != policies->length (); ++i)
        {
          policies[i]->destroy ();
        }
    }
  catch (const CORBA::Exception&)
    {
      // Ignore all exceptions
    }
}

void
TAO_CEC_Reactive_Pulling_Strategy::activate (void)
{
#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
  timer_id_ = this->reactor_->schedule_timer (&this->adapter_,
                                            0,
                                            this->rate_,
                                            this->rate_);
  if (timer_id_ == -1)
    return;

  try
    {
      // Get the PolicyCurrent object
      CORBA::Object_var tmp =
        this->orb_->resolve_initial_references ("PolicyCurrent");

      this->policy_current_ =
        CORBA::PolicyCurrent::_narrow (tmp.in ());

      // Pre-compute the policy list to the set the right timeout
      // value...
      // We need to convert the relative timeout into 100's of nano seconds.
      TimeBase::TimeT timeout;
      ORBSVCS_Time::Time_Value_to_TimeT (timeout,
                                         this->relative_timeout_);
      CORBA::Any any;
      any <<= timeout;

      this->policy_list_.length (1);
      this->policy_list_[0] =
        this->orb_->create_policy (
               Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
               any);
    }
  catch (const CORBA::Exception&)
    {
    }
#endif /* TAO_HAS_CORBA_MESSAGING */
}

void
TAO_CEC_Reactive_Pulling_Strategy::shutdown (void)
{
#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
  this->reactor_->cancel_timer (timer_id_);
#endif /* TAO_HAS_CORBA_MESSAGING */
  this->adapter_.reactor (0);
}

// ****************************************************************

TAO_CEC_Pulling_Strategy_Adapter::TAO_CEC_Pulling_Strategy_Adapter (
      TAO_CEC_Reactive_Pulling_Strategy *adaptee)
  :  adaptee_ (adaptee)
{
}

int
TAO_CEC_Pulling_Strategy_Adapter::handle_timeout (
      const ACE_Time_Value &tv,
      const void *arg)
{
  this->adaptee_->handle_timeout (tv, arg);
  return 0;
}

// ****************************************************************

void
TAO_CEC_Pull_Event::work (TAO_CEC_ProxyPullConsumer *consumer)
{
  CORBA::Boolean has_event = 0;
  CORBA::Any_var any;

  try
    {
      any = consumer->try_pull_from_supplier (has_event);
    }
  catch (const CORBA::Exception&)
    {
      // Ignore all exceptions
      return;
    }

  if (has_event)
    {
      this->consumer_admin_->push (any.in ());
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
