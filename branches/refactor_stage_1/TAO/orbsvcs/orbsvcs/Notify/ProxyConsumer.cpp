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
#include "Event_Manager.h"
#include "Method_Request_Lookup.h"
#include "Worker_Task.h"
#include "Properties.h"
#include "SupplierAdmin.h"

TAO_NS_ProxyConsumer::TAO_NS_ProxyConsumer (void)
  : supplier_admin_ (0)
  , supplier_ (0)
{
}

TAO_NS_ProxyConsumer::~TAO_NS_ProxyConsumer ()
{
  this->supplier_admin_->_decr_refcnt ();
}

TAO_NS_Peer*
TAO_NS_ProxyConsumer::peer (void)
{
  return this->supplier ();
}

void
TAO_NS_ProxyConsumer::init (TAO_NS_SupplierAdmin* supplier_admin ACE_ENV_ARG_DECL)
{
  TAO_NS_Object::init (supplier_admin);

  this->supplier_admin_ = supplier_admin;

  this->supplier_admin_->_incr_refcnt ();

  const CosNotification::QoSProperties &default_ps_qos =
    TAO_NS_PROPERTIES::instance ()->default_proxy_consumer_qos_properties ();

  this->set_qos (default_ps_qos ACE_ENV_ARG_PARAMETER);
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

  {
    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                        CORBA::INTERNAL ());
    ACE_CHECK;

    if (this->is_connected ())
      {
        supplier->release ();
        ACE_THROW (CosEventChannelAdmin::AlreadyConnected ());
      }

    supplier_ = supplier;

    this->supplier_admin_->subscribed_types (this->subscribed_types_ ACE_ENV_ARG_PARAMETER); // get the parents subscribed types.
    ACE_CHECK;
  }

  // Inform QoS values.
  supplier_->qos_changed (this->qos_properties_);

  TAO_NS_EventTypeSeq removed;

  this->event_manager_->offer_change (this, this->subscribed_types_, removed ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->event_manager_->connect (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Increment the global supplier count
  ++supplier_count;
}

void
TAO_NS_ProxyConsumer::disconnect (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_NS_EventTypeSeq added;

  event_manager_->offer_change (this, added, this->subscribed_types_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->event_manager_->disconnect (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Decrement the global supplier count
  this->admin_properties_->suppliers ()--;
}

int
TAO_NS_ProxyConsumer::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->TAO_NS_Object::shutdown (ACE_ENV_SINGLE_ARG_PARAMETER) == 1)
    return 1;

  ACE_CHECK_RETURN (1);

  this->disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (1);

  if (this->supplier_ != 0)
    this->supplier_->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);

  return 0;
}

void
TAO_NS_ProxyConsumer::destroy (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER) == 1)
    return;

  ACE_CHECK;

  this->supplier_admin_->remove (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_ProxyConsumer::push (TAO_NS_Event_var &event)
{
  TAO_NS_Method_Request_Lookup request (event, this, this->event_manager_->consumer_map ());

  this->worker_task ()->exec (request);
}
