// $Id$

#include "orbsvcs/Notify/ProxySupplier.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Notify/ProxySupplier.inl"
#endif /* __ACE_INLINE__ */

#include "orbsvcs/Notify/Event_Manager.h"
#include "orbsvcs/Notify/AdminProperties.h"
#include "orbsvcs/Notify/Consumer.h"
#include "orbsvcs/Notify/Method_Request_Dispatch.h"
#include "orbsvcs/Notify/Worker_Task.h"
#include "orbsvcs/Notify/Buffering_Strategy.h"
#include "orbsvcs/Notify/Properties.h"
#include "orbsvcs/Notify/ConsumerAdmin.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_ProxySupplier::TAO_Notify_ProxySupplier (void)
  : consumer_admin_ (0)
{
}

TAO_Notify_ProxySupplier::~TAO_Notify_ProxySupplier ()
{
}

void
TAO_Notify_ProxySupplier::init (TAO_Notify_ConsumerAdmin* consumer_admin)
{
  ACE_ASSERT (consumer_admin != 0 && this->consumer_admin_.get() == 0);

  TAO_Notify_Proxy::initialize (consumer_admin);

  this->consumer_admin_.reset (consumer_admin);

  this->filter_admin_.event_channel (
    this->consumer_admin_->event_channel());

  const CosNotification::QoSProperties &default_ps_qos =
    TAO_Notify_PROPERTIES::instance ()->default_proxy_supplier_qos_properties ();

  {
    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                          CORBA::INTERNAL ());
    this->TAO_Notify_Object::set_qos (default_ps_qos);
  }
}

TAO_Notify_Peer*
TAO_Notify_ProxySupplier:: peer (void)
{
  return this->consumer ();
}

void
TAO_Notify_ProxySupplier::connect (TAO_Notify_Consumer *consumer)
{
  // Adopt the consumer
  TAO_Notify_Consumer::Ptr auto_consumer (consumer);

  TAO_Notify_Atomic_Property_Long& consumer_count = this->admin_properties().consumers ();
  const TAO_Notify_Property_Long& max_consumers = this->admin_properties().max_consumers ();

  if (max_consumers != 0 && consumer_count >= max_consumers.value ())
  {
    throw CORBA::IMP_LIMIT (); // we've reached the limit of consumers connected.
  }

  {
    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                        CORBA::INTERNAL ());

    // if consumer is set and reconnect not allowed we get out.
    if (this->is_connected () && TAO_Notify_PROPERTIES::instance()->allow_reconnect() == false)
      {
          throw CosEventChannelAdmin::AlreadyConnected ();
      }

    // Adopt the consumer
    if (this->consumer_.get() != 0)
      auto_consumer->assume_pending_events (*this->consumer_.get ());
    this->consumer_ = auto_consumer;

    this->consumer_admin_->subscribed_types (this->subscribed_types_); // get the parents subscribed types.
  }

  // Inform QoS values.
  ACE_ASSERT (this->consumer_.get() != 0);
  this->consumer_->qos_changed (this->qos_properties_);

  TAO_Notify_EventTypeSeq removed;

  this->event_manager().subscription_change (this, this->subscribed_types_, removed);

  this->event_manager().connect (this);

  // Increment the global consumer count
  ++consumer_count;
}

void
TAO_Notify_ProxySupplier::disconnect (void)
{
  TAO_Notify_EventTypeSeq added;

  this->event_manager().subscription_change (this, added, this->subscribed_types_);

  this->event_manager().disconnect (this);

  // Decrement the global consumer count
  this->admin_properties().consumers ()--;
}

int
TAO_Notify_ProxySupplier::shutdown (void)
{
  if (this->TAO_Notify_Object::shutdown () == 1)
    return 1;


  this->disconnect ();

  if (this->consumer_.get() != 0)
  {
    this->consumer_->shutdown ();
  }
  return 0;
}

void
TAO_Notify_ProxySupplier::destroy (void)
{
  this->destroy (false);
}

void
TAO_Notify_ProxySupplier::destroy (bool from_timeout)
{
  this->shutdown ();
  this->consumer_admin_->cleanup_proxy (this, true, from_timeout);

  // Do not reset this->consumer_.
  // It is not safe to delete the non-refcounted consumer here.
}

void
TAO_Notify_ProxySupplier::deliver (TAO_Notify_Method_Request_Dispatch_No_Copy & request)
{
  this->execute_task (request);
}

void
TAO_Notify_ProxySupplier::qos_changed (const TAO_Notify_QoSProperties& qos_properties)
{
  TAO_Notify_Proxy::qos_changed (qos_properties);
}

TAO_END_VERSIONED_NAMESPACE_DECL
