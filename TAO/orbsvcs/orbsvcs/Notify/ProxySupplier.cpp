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
#include "Proxy.h"
#include "Admin.h"
#include "EventChannel.h"
#include "EventChannelFactory.h"
#include "Notify_Service.h"
#include "Method_Request_Dispatch.h"
#include "Worker_Task.h"

TAO_NS_ProxySupplier::TAO_NS_ProxySupplier (void)
  :consumer_ (0)
{
}

TAO_NS_ProxySupplier::~TAO_NS_ProxySupplier ()
{
}

void
TAO_NS_ProxySupplier::init_ps (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
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

  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  if (this->is_connected ())
    {
      consumer->release ();
      ACE_THROW (CosEventChannelAdmin::AlreadyConnected ());
    }
  else
    {
      consumer_ = consumer;

      // Inform QoS values.
      consumer_->qos_changed (this->qos_properties_);

      this->parent_->subscribed_types (this->subscribed_types_ ACE_ENV_ARG_PARAMETER); // get the parents subscribed types.
      ACE_CHECK;

      TAO_NS_EventTypeSeq removed;

      this->event_manager_->subscription_change (this, this->subscribed_types_, removed ACE_ENV_ARG_PARAMETER);

      this->event_manager_->connect (this ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // Increment the global consumer count
      ++consumer_count;
    }
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

void
TAO_NS_ProxySupplier::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  this->disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);

  this->inherited::shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);

  if (this->consumer_ != 0)
    this->consumer_->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_NS_ProxySupplier::push (TAO_NS_Event_var &event)
{
  TAO_NS_Method_Request_Dispatch request (event, this);

  this->worker_task ()->exec (request);
}

void
TAO_NS_ProxySupplier::push_no_filtering (TAO_NS_Event_var &event)
{
  TAO_NS_Method_Request_Dispatch_No_Filtering request (event, this);

  this->worker_task ()->exec (request);
}
