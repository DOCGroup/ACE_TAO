// $Id$

// Note: This class controls the behaviour of consumers connected to both
// the Typed and Un-typed Event Channels.  A check must be made in the code
// to ensure the correct EC is referenced. 

#include "orbsvcs/CosEvent/CEC_EventChannel.h"
#include "orbsvcs/CosEvent/CEC_ConsumerAdmin.h"
#include "orbsvcs/CosEvent/CEC_Reactive_ConsumerControl.h"

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
#include "orbsvcs/CosEvent/CEC_TypedEventChannel.h"
#include "orbsvcs/CosEvent/CEC_TypedConsumerAdmin.h"
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

#include "orbsvcs/CosEvent/CEC_ProxyPushSupplier.h"
#include "orbsvcs/CosEvent/CEC_ProxyPullSupplier.h"

#include "orbsvcs/Time_Utilities.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
#include "tao/Messaging/Messaging.h"
#endif

#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "ace/Reactor.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/CosEvent/CEC_Reactive_ConsumerControl.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID (CosEvent,
           CEC_Reactive_ConsumerControl,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// TAO_CEC_Reactive_ConsumerControl constructor for the Un-typed EC
TAO_CEC_Reactive_ConsumerControl::
     TAO_CEC_Reactive_ConsumerControl (const ACE_Time_Value &rate,
                                       const ACE_Time_Value &timeout,
                                       unsigned int retries,
                                       TAO_CEC_EventChannel *ec,
                                       CORBA::ORB_ptr orb)
  : rate_ (rate),
    timeout_ (timeout),
    retries_ (retries),
    adapter_ (this),
    event_channel_ (ec),
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
    typed_event_channel_ (0),
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */
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

// TAO_CEC_Reactive_ConsumerControl constructor for the Typed EC
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
TAO_CEC_Reactive_ConsumerControl::
     TAO_CEC_Reactive_ConsumerControl (const ACE_Time_Value &rate,
                                       const ACE_Time_Value &timeout,
                                       unsigned int retries,
                                       TAO_CEC_TypedEventChannel *ec,
                                       CORBA::ORB_ptr orb)
  : rate_ (rate),
    timeout_ (timeout),
    retries_ (retries),
    adapter_ (this),
    event_channel_ (0),
    typed_event_channel_ (ec),
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
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

TAO_CEC_Reactive_ConsumerControl::~TAO_CEC_Reactive_ConsumerControl (void)
{
}

void
TAO_CEC_Reactive_ConsumerControl::query_consumers (
      ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_CEC_Ping_Push_Consumer push_worker (this);

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
  if (this->typed_event_channel_)
    {
      // Typed EC
      this->typed_event_channel_->typed_consumer_admin ()->for_each (&push_worker
                                                                     ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

  // Un-typed EC
  this->event_channel_->consumer_admin ()->for_each (&push_worker
                                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  TAO_CEC_Ping_Pull_Consumer pull_worker (this);
  this->event_channel_->consumer_admin ()->for_each (&pull_worker
                                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
    }
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */
}

bool
TAO_CEC_Reactive_ConsumerControl::need_to_disconnect (
                                    PortableServer::ServantBase* proxy)
{
  bool disconnect = true;

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
  if (this->typed_event_channel_)
    {
      // Typed EC
      TAO_CEC_TypedEventChannel::ServantRetryMap::ENTRY* entry = 0;
      if (this->typed_event_channel_->
          get_servant_retry_map ().find (proxy, entry) == 0)
        {
          ++entry->int_id_;
          if (entry->int_id_ <= this->retries_)
            {
              disconnect = false;
            }
        }
    }
  else
    {
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

  // Un-typed EC
  TAO_CEC_EventChannel::ServantRetryMap::ENTRY* entry = 0;
  if (this->event_channel_->
      get_servant_retry_map ().find (proxy, entry) == 0)
    {
      ++entry->int_id_;
      if (entry->int_id_ <= this->retries_)
        {
          disconnect = false;
        }
    }

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
    }
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

  return disconnect;
}

void
TAO_CEC_Reactive_ConsumerControl::successful_transmission (
                                    PortableServer::ServantBase* proxy)
{

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
  if (this->typed_event_channel_)
    {
      // Typed EC
      TAO_CEC_TypedEventChannel::ServantRetryMap::ENTRY* entry = 0;
      if (this->typed_event_channel_->
          get_servant_retry_map ().find (proxy, entry) == 0)
        {
          entry->int_id_ = 0;
        }
    }
  else
    {
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

  // Un-typed EC
  TAO_CEC_EventChannel::ServantRetryMap::ENTRY* entry = 0;
  if (this->event_channel_->
      get_servant_retry_map ().find (proxy, entry) == 0)
    {
      entry->int_id_ = 0;
    }

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
    }
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

}

void
TAO_CEC_Reactive_ConsumerControl::handle_timeout (
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

      ACE_TRY_EX (query)
        {
          // Query the state of the consumers...
          this->query_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK_EX (query);
        }
      ACE_CATCHANY
        {
          // Ignore all exceptions
        }
      ACE_ENDTRY;

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

int
TAO_CEC_Reactive_ConsumerControl::activate (void)
{
#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
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
      // We need to convert the relative timeout into 100's of nano seconds.
      TimeBase::TimeT timeout;
      ORBSVCS_Time::Time_Value_to_TimeT (timeout,
                                         this->timeout_);
      CORBA::Any any;
      any <<= timeout;

      this->policy_list_.length (1);
      this->policy_list_[0] =
        this->orb_->create_policy (
               Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
               any
               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Only schedule the timer, when the rate is not zero
      if (this->rate_ != ACE_Time_Value::zero)
      {
        // Schedule the timer after these policies has been set, because the
        // handle_timeout uses these policies, if done in front, the channel
        // can crash when the timeout expires before initiazation is ready.
        timer_id_ = this->reactor_->schedule_timer (&this->adapter_,
                                                    0,
                                                    this->rate_,
                                                    this->rate_);
        if (timer_id_ == -1)
         return -1;
      }
    }
  ACE_CATCHANY
    {
      return -1;
    }
  ACE_ENDTRY;
#endif /* TAO_HAS_CORBA_MESSAGING */

  return 0;
}

int
TAO_CEC_Reactive_ConsumerControl::shutdown (void)
{
  int r = 0;

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
  r = this->reactor_->cancel_timer (timer_id_);
#endif /* TAO_HAS_CORBA_MESSAGING */
  this->adapter_.reactor (0);
  return r;
}

void
TAO_CEC_Reactive_ConsumerControl::consumer_not_exist (
      TAO_CEC_ProxyPushSupplier *proxy
      ACE_ENV_ARG_DECL)
{
  ACE_TRY
    {
      proxy->disconnect_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (TAO_debug_level >= 10)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("ProxyPushSupplier disconnected due to consumer_not_exist\n")));
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("Reactive_ConsumerControl::consumer_not_exist"));
      // Ignore all exceptions..
    }
  ACE_ENDTRY;
}

void
TAO_CEC_Reactive_ConsumerControl::consumer_not_exist (
      TAO_CEC_ProxyPullSupplier *proxy
      ACE_ENV_ARG_DECL)
{
  ACE_TRY
    {
      proxy->disconnect_pull_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("Reactive_ConsumerControl::consumer_not_exist"));
      // Ignore all exceptions..
    }
  ACE_ENDTRY;
}

void
TAO_CEC_Reactive_ConsumerControl::system_exception (
      TAO_CEC_ProxyPushSupplier *proxy,
      CORBA::SystemException & /* exception */
      ACE_ENV_ARG_DECL)
{
  ACE_TRY
    {
      if (this->need_to_disconnect (proxy))
        {
          proxy->disconnect_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (TAO_debug_level >= 10)
            {
              ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("ProxyPushSupplier disconnected due to consumer_not_exist\n")));
            }
        }
    }
  ACE_CATCHANY
    {
      // Ignore all exceptions..
    }
  ACE_ENDTRY;
}

// ****************************************************************

TAO_CEC_ConsumerControl_Adapter::TAO_CEC_ConsumerControl_Adapter (
      TAO_CEC_Reactive_ConsumerControl *adaptee)
  :  adaptee_ (adaptee)
{
}

int
TAO_CEC_ConsumerControl_Adapter::handle_timeout (
      const ACE_Time_Value &tv,
      const void *arg)
{
  this->adaptee_->handle_timeout (tv, arg);
  return 0;
}

// ****************************************************************

void
TAO_CEC_Ping_Push_Consumer::work (TAO_CEC_ProxyPushSupplier *supplier
                                  ACE_ENV_ARG_DECL)
{
  ACE_TRY
    {
      CORBA::Boolean disconnected;
      CORBA::Boolean non_existent =
        supplier->consumer_non_existent (disconnected
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (non_existent && !disconnected)
        {
          this->control_->consumer_not_exist (supplier ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCH (CORBA::OBJECT_NOT_EXIST, ex)
    {
      this->control_->consumer_not_exist (supplier ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::TRANSIENT, transient)
    {
      if (this->control_->need_to_disconnect (supplier))
        {
          this->control_->consumer_not_exist (supplier ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      // Ignore all exceptions
    }
  ACE_ENDTRY;
}

// ****************************************************************

void
TAO_CEC_Ping_Pull_Consumer::work (TAO_CEC_ProxyPullSupplier *supplier
                                  ACE_ENV_ARG_DECL)
{
  ACE_TRY
    {
      CORBA::Boolean disconnected;
      CORBA::Boolean non_existent =
        supplier->consumer_non_existent (disconnected
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (non_existent && !disconnected)
        {
          this->control_->consumer_not_exist (supplier ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCH (CORBA::OBJECT_NOT_EXIST, ex)
    {
      this->control_->consumer_not_exist (supplier ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::TRANSIENT, transient)
    {
      if (this->control_->need_to_disconnect (supplier))
        {
          this->control_->consumer_not_exist (supplier ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      // Ignore all exceptions
    }
  ACE_ENDTRY;
}

TAO_END_VERSIONED_NAMESPACE_DECL
