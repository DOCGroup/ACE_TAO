// $Id$

// Note: This class controls the behaviour of suppliers connected to both
// the Typed and Un-typed Event Channels.  A check must be made in the code
// to ensure the correct EC is referenced.

#include "orbsvcs/CosEvent/CEC_EventChannel.h"
#include "orbsvcs/CosEvent/CEC_SupplierAdmin.h"
#include "orbsvcs/CosEvent/CEC_ProxyPushConsumer.h"
#include "orbsvcs/CosEvent/CEC_Reactive_SupplierControl.h"

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
#include "orbsvcs/CosEvent/CEC_TypedEventChannel.h"
#include "orbsvcs/CosEvent/CEC_TypedSupplierAdmin.h"
#include "orbsvcs/CosEvent/CEC_TypedProxyPushConsumer.h"
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

#include "orbsvcs/CosEvent/CEC_ProxyPullConsumer.h"

#include "orbsvcs/Time_Utilities.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
#include "tao/Messaging/Messaging.h"
#endif

#include "tao/ORB_Core.h"

#include "ace/Reactor.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/CosEvent/CEC_Reactive_SupplierControl.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// TAO_CEC_Reactive_SupplierControl constructor for the Un-typed EC
TAO_CEC_Reactive_SupplierControl::
     TAO_CEC_Reactive_SupplierControl (const ACE_Time_Value &rate,
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

// TAO_CEC_Reactive_SupplierControl constructor for the Typed EC
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
TAO_CEC_Reactive_SupplierControl::
     TAO_CEC_Reactive_SupplierControl (const ACE_Time_Value &rate,
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

TAO_CEC_Reactive_SupplierControl::~TAO_CEC_Reactive_SupplierControl (void)
{
}

void
TAO_CEC_Reactive_SupplierControl::query_suppliers ()
{
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
  if (this->typed_event_channel_)
    {
      // Typed EC
      TAO_CEC_Ping_Typed_Push_Supplier push_worker (this);

      this->typed_event_channel_->typed_supplier_admin ()->for_each (&push_worker);
    }
  else
    {
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

  // Un-typed EC
  TAO_CEC_Ping_Push_Supplier push_worker (this);
  this->event_channel_->supplier_admin ()->for_each (&push_worker);

  TAO_CEC_Ping_Pull_Supplier pull_worker (this);
  this->event_channel_->supplier_admin ()->for_each (&pull_worker);

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
    }
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */
}

bool
TAO_CEC_Reactive_SupplierControl::need_to_disconnect (
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
TAO_CEC_Reactive_SupplierControl::successful_transmission (
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
TAO_CEC_Reactive_SupplierControl::handle_timeout (
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
          // Query the state of the suppliers...
          this->query_suppliers ();
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

int
TAO_CEC_Reactive_SupplierControl::activate (void)
{
#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
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
                                         this->timeout_);
      CORBA::Any any;
      any <<= timeout;

      this->policy_list_.length (1);
      this->policy_list_[0] =
        this->orb_->create_policy (
               Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
               any);

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
  catch (const CORBA::Exception&)
    {
      return -1;
    }
#endif /* TAO_HAS_CORBA_MESSAGING */

  return 0;
}

int
TAO_CEC_Reactive_SupplierControl::shutdown (void)
{
  int r = 0;

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
  r = this->reactor_->cancel_timer (timer_id_);
#endif /* TAO_HAS_CORBA_MESSAGING */
  this->adapter_.reactor (0);
  return r;
}

void
TAO_CEC_Reactive_SupplierControl::supplier_not_exist (
      TAO_CEC_ProxyPushConsumer *proxy)
{
  try
    {
      proxy->disconnect_push_consumer ();
    }
  catch (const CORBA::Exception&)
    {
      // Ignore all exceptions..
    }
}

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
void
TAO_CEC_Reactive_SupplierControl::supplier_not_exist (
      TAO_CEC_TypedProxyPushConsumer *proxy)
{
  try
    {
      proxy->disconnect_push_consumer ();
    }
  catch (const CORBA::Exception&)
    {
      // Ignore all exceptions..
    }
}
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

void
TAO_CEC_Reactive_SupplierControl::supplier_not_exist (
      TAO_CEC_ProxyPullConsumer *proxy)
{
  try
    {
      proxy->disconnect_pull_consumer ();
    }
  catch (const CORBA::Exception&)
    {
      // Ignore all exceptions..
    }
}

void
TAO_CEC_Reactive_SupplierControl::system_exception (
      TAO_CEC_ProxyPullConsumer *proxy,
      CORBA::SystemException & /* exception */)
{
  try
    {
      if (this->need_to_disconnect (proxy))
        {
          proxy->disconnect_pull_consumer ();
        }
    }
  catch (const CORBA::Exception&)
    {
      // Ignore all exceptions..
    }
}

// ****************************************************************

TAO_CEC_SupplierControl_Adapter::TAO_CEC_SupplierControl_Adapter (
      TAO_CEC_Reactive_SupplierControl *adaptee)
  :  adaptee_ (adaptee)
{
}

int
TAO_CEC_SupplierControl_Adapter::handle_timeout (
      const ACE_Time_Value &tv,
      const void *arg)
{
  this->adaptee_->handle_timeout (tv, arg);
  return 0;
}

// ****************************************************************

void
TAO_CEC_Ping_Push_Supplier::work (TAO_CEC_ProxyPushConsumer *consumer)
{
  try
    {
      CORBA::Boolean disconnected;
      CORBA::Boolean non_existent =
        consumer->supplier_non_existent (disconnected);
      if (non_existent && !disconnected)
        {
          this->control_->supplier_not_exist (consumer);
        }
    }
  catch (const CORBA::OBJECT_NOT_EXIST& )
    {
      this->control_->supplier_not_exist (consumer);
    }
  catch (const CORBA::TRANSIENT& )
    {
      if (this->control_->need_to_disconnect (consumer))
        {
          this->control_->supplier_not_exist (consumer);
        }
    }
  catch (const CORBA::Exception&)
    {
      // Ignore all exceptions
    }
}

// ****************************************************************

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
void
TAO_CEC_Ping_Typed_Push_Supplier::work (TAO_CEC_TypedProxyPushConsumer *consumer)
{
  try
    {
      CORBA::Boolean disconnected;
      CORBA::Boolean non_existent =
        consumer->supplier_non_existent (disconnected);
      if (non_existent && !disconnected)
        {
          this->control_->supplier_not_exist (consumer);
        }
    }
  catch (const CORBA::OBJECT_NOT_EXIST& )
    {
      this->control_->supplier_not_exist (consumer);
    }
  catch (const CORBA::TRANSIENT& )
    {
      if (this->control_->need_to_disconnect (consumer))
        {
          this->control_->supplier_not_exist (consumer);
        }
    }
  catch (const CORBA::Exception&)
    {
      // Ignore all exceptions
    }
}
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

// ****************************************************************

void
TAO_CEC_Ping_Pull_Supplier::work (TAO_CEC_ProxyPullConsumer *consumer)
{
  try
    {
      CORBA::Boolean disconnected;
      CORBA::Boolean non_existent =
        consumer->supplier_non_existent (disconnected);
      if (non_existent && !disconnected)
        {
          this->control_->supplier_not_exist (consumer);
        }
    }
  catch (const CORBA::OBJECT_NOT_EXIST& )
    {
      this->control_->supplier_not_exist (consumer);
    }
  catch (const CORBA::TRANSIENT& )
    {
      if (this->control_->need_to_disconnect (consumer))
        {
          this->control_->supplier_not_exist (consumer);
        }
    }
  catch (const CORBA::Exception&)
    {
      // Ignore all exceptions
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
