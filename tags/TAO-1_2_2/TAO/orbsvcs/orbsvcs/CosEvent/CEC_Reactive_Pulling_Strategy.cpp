// $Id$

#include "CEC_Reactive_Pulling_Strategy.h"
#include "CEC_ProxyPullConsumer.h"
#include "CEC_EventChannel.h"
#include "CEC_SupplierAdmin.h"
#include "CEC_ConsumerAdmin.h"

#include "tao/Messaging.h"
#include "tao/ORB_Core.h"

#include "ace/Reactor.h"

#if ! defined (__ACE_INLINE__)
#include "CEC_Reactive_Pulling_Strategy.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(CosEvent, CEC_Reactive_Pulling_Strategy, "$Id$")

TAO_CEC_Reactive_Pulling_Strategy::
    TAO_CEC_Reactive_Pulling_Strategy (const ACE_Time_Value &rate,
                                       TAO_CEC_EventChannel *event_channel,
                                       CORBA::ORB_ptr orb)
  :  adapter_ (this),
     rate_ (rate),
     event_channel_ (event_channel),
     orb_ (CORBA::ORB::_duplicate (orb))
{
  this->reactor_ =
    this->orb_->orb_core ()->reactor ();
}

void
TAO_CEC_Reactive_Pulling_Strategy::handle_timeout (
      const ACE_Time_Value &,
      const void *)
{
  ACE_TRY_NEW_ENV
    {
      // Query the state of the Current object *before* we initiate
      // the iteration...
      CORBA::PolicyTypeSeq types;
      CORBA::PolicyList_var policies =
        this->policy_current_->get_policy_overrides (types
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Change the timeout
      this->policy_current_->set_policy_overrides (this->policy_list_,
                                                   CORBA::ADD_OVERRIDE
                                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      TAO_CEC_Pull_Event worker (this->event_channel_->consumer_admin (),
                                 this->event_channel_->supplier_control ());

      this->event_channel_->supplier_admin ()->for_each (&worker
                                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->policy_current_->set_policy_overrides (policies.in (),
                                                   CORBA::SET_OVERRIDE
                                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      for (CORBA::ULong i = 0; i != policies->length (); ++i)
        {
          policies[i]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      // Ignore all exceptions
    }
  ACE_ENDTRY;
}

void
TAO_CEC_Reactive_Pulling_Strategy::activate (void)
{
#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
  long id = this->reactor_->schedule_timer (&this->adapter_,
                                            0,
                                            this->rate_,
                                            this->rate_);
  if (id == -1)
    return;

  ACE_TRY_NEW_ENV
    {
      // Get the PolicyCurrent object
      CORBA::Object_var tmp =
        this->orb_->resolve_initial_references ("PolicyCurrent"
                                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->policy_current_ =
        CORBA::PolicyCurrent::_narrow (tmp.in ()
                                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Pre-compute the policy list to the set the right timeout
      // value...
      // @@ TODO It is currently hard-coded to 10 milliseconds
      TimeBase::TimeT timeout = 10 * 10000;
      CORBA::Any any;
      any <<= timeout;

      this->policy_list_.length (1);
      this->policy_list_[0] =
        this->orb_->create_policy (
               Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
               any
               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;
#endif /* TAO_HAS_CORBA_MESSAGING */
}

void
TAO_CEC_Reactive_Pulling_Strategy::shutdown (void)
{
  this->reactor_->remove_handler (&this->adapter_,
                                  ACE_Event_Handler::DONT_CALL);
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
TAO_CEC_Pull_Event::work (TAO_CEC_ProxyPullConsumer *consumer
                          ACE_ENV_ARG_DECL)
{
  CORBA::Boolean has_event = 0;
  CORBA::Any_var any;

  ACE_TRY
    {
      any = consumer->try_pull_from_supplier (has_event
                                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // Ignore all exceptions
      return;
    }
  ACE_ENDTRY;

  if (has_event)
    {
      this->consumer_admin_->push (any.in () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
