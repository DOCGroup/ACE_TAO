// $Id$

#include "ProxyConsumer.h"

#if ! defined (__ACE_INLINE__)
#include "ProxyConsumer.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_ProxyConsumer, "$Id$")

#include "tao/debug.h"
#include "ace/Atomic_Op.h"
#include "Supplier.h"
#include "AdminProperties.h"
#include "Property.h"
#include "Proxy.h"
#include "Admin.h"
#include "EventChannel.h"
#include "EventChannelFactory.h"
#include "Notify_Service.h"
#include "Event_Manager.h"
#include "Method_Request_Lookup.h"
#include "Worker_Task.h"

TAO_NS_ProxyConsumer::TAO_NS_ProxyConsumer (void)
  :supplier_ (0)
{
}

TAO_NS_ProxyConsumer::~TAO_NS_ProxyConsumer ()
{
}

TAO_NS_Peer*
TAO_NS_ProxyConsumer::peer (void)
{
  return this->supplier ();
}

void
TAO_NS_ProxyConsumer::connect (TAO_NS_Supplier *supplier ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosEventChannelAdmin::AlreadyConnected
                   ))
{
  TAO_NS_Atomic_Property_Long& supplier_count = this->admin_properties_->suppliers ();
  const TAO_NS_Property_Long& max_suppliers = this->admin_properties_->max_suppliers ();

  if (max_suppliers != 0 &&
      supplier_count >= max_suppliers.value ())
    ACE_THROW (CORBA::IMP_LIMIT ()); // we've reached the limit of suppliers connected.

  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  if (this->is_connected ())
    {
      supplier->release ();
      ACE_THROW (CosEventChannelAdmin::AlreadyConnected ());
    }
  else
    {
      supplier_ = supplier;

      supplier->updates_dispatch_observer (this->event_manager_->updates_dispatch_observer ());

      this->parent_->subscribed_types (this->subscribed_types_ ACE_ENV_ARG_PARAMETER); // get the parents subscribed types.
      ACE_CHECK;

      // Inform QoS values.
      supplier_->qos_changed (this->qos_properties_);

      event_manager_->publish (this, this->subscribed_types_ ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // Increment the global supplier count
      ++supplier_count;
    }
}

void
TAO_NS_ProxyConsumer::disconnect (ACE_ENV_SINGLE_ARG_DECL)
{
  event_manager_->un_publish (this, this->subscribed_types_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Decrement the global supplier count
  this->admin_properties_->suppliers ()--;
}

void
TAO_NS_ProxyConsumer::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  this->disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->inherited::shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->supplier_ != 0)
    this->supplier_->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_NS_ProxyConsumer::push (TAO_NS_Event_var &event)
{
  TAO_NS_Method_Request_Lookup request (event, this, this->event_manager_->consumer_map ());

  this->worker_task ()->exec (request);
}
