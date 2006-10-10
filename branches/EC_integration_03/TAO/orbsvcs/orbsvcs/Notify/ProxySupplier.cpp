// $Id$

#include "orbsvcs/Notify/ProxySupplier.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Notify/ProxySupplier.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_Notify_ProxySupplier, "$Id$")

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
TAO_Notify_ProxySupplier::init (TAO_Notify_ConsumerAdmin* consumer_admin ACE_ENV_ARG_DECL)
{
  ACE_ASSERT (consumer_admin != 0 && this->consumer_admin_.get() == 0);

  TAO_Notify_Proxy::initialize (consumer_admin ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->consumer_admin_.reset (consumer_admin);

  const CosNotification::QoSProperties &default_ps_qos =
    TAO_Notify_PROPERTIES::instance ()->default_proxy_supplier_qos_properties ();

  {
    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                          CORBA::INTERNAL ());
    this->TAO_Notify_Object::set_qos (default_ps_qos ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }
}

TAO_Notify_Peer*
TAO_Notify_ProxySupplier:: peer (void)
{
  return this->consumer ();
}

void
TAO_Notify_ProxySupplier::connect (TAO_Notify_Consumer *consumer ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosEventChannelAdmin::AlreadyConnected
                   ))
{
  // Adopt the consumer
  ACE_Auto_Ptr< TAO_Notify_Consumer > auto_consumer (consumer);

  TAO_Notify_Atomic_Property_Long& consumer_count = this->admin_properties().consumers ();
  const TAO_Notify_Property_Long& max_consumers = this->admin_properties().max_consumers ();

  if (max_consumers != 0 && consumer_count >= max_consumers.value ())
  {
    ACE_THROW (CORBA::IMP_LIMIT ()); // we've reached the limit of consumers connected.
  }

  {
    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                        CORBA::INTERNAL ());
    ACE_CHECK;

    // if consumer is set and reconnect not allowed we get out.
    if (this->is_connected () && TAO_Notify_PROPERTIES::instance()->allow_reconnect() == false)
      {
          ACE_THROW (CosEventChannelAdmin::AlreadyConnected ());
      }

    // Adopt the consumer
    this->consumer_ = auto_consumer;

    this->consumer_admin_->subscribed_types (this->subscribed_types_ ACE_ENV_ARG_PARAMETER); // get the parents subscribed types.
    ACE_CHECK;
  }

  // Inform QoS values.
  ACE_ASSERT (this->consumer_.get() != 0);
  this->consumer_->qos_changed (this->qos_properties_);

  TAO_Notify_EventTypeSeq removed;

  this->event_manager().subscription_change (this, this->subscribed_types_, removed ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->event_manager().connect (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Increment the global consumer count
  ++consumer_count;
}

void
TAO_Notify_ProxySupplier::disconnect (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_Notify_EventTypeSeq added;

  this->event_manager().subscription_change (this, added, this->subscribed_types_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->event_manager().disconnect (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Decrement the global consumer count
  this->admin_properties().consumers ()--;
}

int
TAO_Notify_ProxySupplier::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->TAO_Notify_Object::shutdown (ACE_ENV_SINGLE_ARG_PARAMETER) == 1)
    return 1;

  ACE_CHECK_RETURN (1);

  this->disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (1);

  if (this->consumer_.get() != 0)
  {
    this->consumer_->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  }
  return 0;
}

void
TAO_Notify_ProxySupplier::destroy (ACE_ENV_SINGLE_ARG_DECL)
{
  int result = this->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  if ( result == 1)
    return;

  this->consumer_admin_->remove (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Do not reset this->consumer_.
  // It is not safe to delete the non-refcounted consumer here.
}

void
TAO_Notify_ProxySupplier::deliver (TAO_Notify_Method_Request_Dispatch_No_Copy & request ACE_ENV_ARG_DECL)
{
  this->execute_task (request ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_ProxySupplier::qos_changed (const TAO_Notify_QoSProperties& qos_properties)
{
  TAO_Notify_Proxy::qos_changed (qos_properties);
}

TAO_END_VERSIONED_NAMESPACE_DECL
