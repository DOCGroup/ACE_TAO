// $Id$

#include "ProxySupplier.h"

#if ! defined (__ACE_INLINE__)
#include "ProxySupplier.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_ProxySupplier, "$Id$")

#include "Method_Request_Dispatch_No_Filtering.h"
#include "Event_Manager.h"
#include "AdminProperties.h"
#include "Consumer.h"
#include "Method_Request_Dispatch.h"
#include "Worker_Task.h"
#include "Buffering_Strategy.h"
#include "Properties.h"
#include "ConsumerAdmin.h"

TAO_NS_ProxySupplier::TAO_NS_ProxySupplier (void)
  : consumer_admin_ (0)
  , consumer_ (0)
{
}

TAO_NS_ProxySupplier::~TAO_NS_ProxySupplier ()
{
  this->consumer_admin_->_decr_refcnt ();
}

void
TAO_NS_ProxySupplier::init (TAO_NS_ConsumerAdmin* consumer_admin ACE_ENV_ARG_DECL)
{
  TAO_NS_Object::init (consumer_admin);

  this->consumer_admin_ = consumer_admin;

  this->consumer_admin_->_incr_refcnt ();

  const CosNotification::QoSProperties &default_ps_qos =
    TAO_NS_PROPERTIES::instance ()->default_proxy_supplier_qos_properties ();

  this->set_qos (default_ps_qos ACE_ENV_ARG_PARAMETER);
}

TAO_NS_Peer*
TAO_NS_ProxySupplier:: peer (void)
{
  return this->consumer ();
}

void
TAO_NS_ProxySupplier::connect (TAO_NS_Consumer *consumer ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosEventChannelAdmin::AlreadyConnected
                   ))
{
  TAO_NS_Atomic_Property_Long& consumer_count = this->admin_properties_->consumers ();
  const TAO_NS_Property_Long& max_consumers = this->admin_properties_->max_consumers ();

  if (max_consumers != 0 &&
      consumer_count >= max_consumers.value ())
    ACE_THROW (CORBA::IMP_LIMIT ()); // we've reached the limit of consumers connected.

  {
    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                        CORBA::INTERNAL ());
    ACE_CHECK;

    if (this->is_connected ())
      {
        consumer->release ();
        ACE_THROW (CosEventChannelAdmin::AlreadyConnected ());
      }

    consumer_ = consumer;

    this->consumer_admin_->subscribed_types (this->subscribed_types_ ACE_ENV_ARG_PARAMETER); // get the parents subscribed types.
    ACE_CHECK;
  }

  // Inform QoS values.
  consumer_->qos_changed (this->qos_properties_);

  TAO_NS_EventTypeSeq removed;

  this->event_manager_->subscription_change (this, this->subscribed_types_, removed ACE_ENV_ARG_PARAMETER);

  this->event_manager_->connect (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Increment the global consumer count
  ++consumer_count;
}

void
TAO_NS_ProxySupplier::disconnect (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_NS_EventTypeSeq added;

  this->event_manager_->subscription_change (this, added, this->subscribed_types_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->event_manager_->disconnect (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Decrement the global consumer count
  this->admin_properties_->consumers ()--;
}

int
TAO_NS_ProxySupplier::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->TAO_NS_Object::shutdown (ACE_ENV_SINGLE_ARG_PARAMETER) == 1)
    return 1;

  ACE_CHECK_RETURN (1);

  this->disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (1);

  if (this->consumer_ != 0)
    this->consumer_->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);

  return 0;
}

void
TAO_NS_ProxySupplier::destroy (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER) == 1)
    return;

  ACE_CHECK;

  this->consumer_admin_->remove (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_ProxySupplier::push (const TAO_NS_Event_var &event)
{
  TAO_NS_Method_Request_Dispatch request (event, this);

  this->worker_task ()->exec (request);
}

void
TAO_NS_ProxySupplier::push_no_filtering (const TAO_NS_Event_var &event)
{
  TAO_NS_Method_Request_Dispatch_No_Filtering request (event, this);

  this->worker_task ()->exec (request);
}

void
TAO_NS_ProxySupplier::qos_changed (const TAO_NS_QoSProperties& qos_properties)
{
  TAO_NS_Property_Long mepc_qos (CosNotification::MaxEventsPerConsumer);

  if (mepc_qos.set (qos_properties) != -1)
    {
      // Does the Proxy own the Worker Task?
      if (own_worker_task_)
        {
          TAO_NS_Buffering_Strategy* bs = this->worker_task_->buffering_strategy ();

          // Apply this QoS to the Proxy's Buffering Strategy.
          if (bs)
            {
              bs->max_local_queue_length (mepc_qos.value ());
            }
        }
    }

  TAO_NS_Proxy::qos_changed (qos_properties);
}
