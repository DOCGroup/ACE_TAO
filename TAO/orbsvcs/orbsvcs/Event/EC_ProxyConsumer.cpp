// $Id$

#include "EC_ProxyConsumer.h"
#include "EC_Event_Channel.h"
#include "EC_SupplierFiltering.h"
#include "EC_Supplier_Filter_Builder.h"

#if ! defined (__ACE_INLINE__)
#include "EC_ProxyConsumer.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_ProxyConsumer, "$Id$")

TAO_EC_ProxyPushConsumer::
    TAO_EC_ProxyPushConsumer (TAO_EC_Event_Channel* ec)
  : event_channel_ (ec),
    refcount_ (1),
    filter_ (0)
{
  this->lock_ =
    this->event_channel_->create_consumer_lock ();

  this->default_POA_ =
    this->event_channel_->consumer_poa ();
}

TAO_EC_ProxyPushConsumer::~TAO_EC_ProxyPushConsumer (void)
{
  this->event_channel_->destroy_consumer_lock (this->lock_);
}

PortableServer::POA_ptr
TAO_EC_ProxyPushConsumer::_default_POA (CORBA::Environment&)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

void
TAO_EC_ProxyPushConsumer::connected (TAO_EC_ProxyPushSupplier* supplier,
                                     CORBA::Environment &ACE_TRY_ENV)
{
  if (this->is_connected ())
    this->filter_->connected (supplier, ACE_TRY_ENV);
}

void
TAO_EC_ProxyPushConsumer::disconnected (TAO_EC_ProxyPushSupplier* supplier,
                                        CORBA::Environment &ACE_TRY_ENV)
{
  if (this->is_connected ())
    this->filter_->disconnected (supplier, ACE_TRY_ENV);
}

void
TAO_EC_ProxyPushConsumer::connected (TAO_EC_ProxyPushConsumer*,
                                     CORBA::Environment &)
{
}

void
TAO_EC_ProxyPushConsumer::disconnected (TAO_EC_ProxyPushConsumer*,
                                        CORBA::Environment &)
{
}

CORBA::ULong
TAO_EC_ProxyPushConsumer::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);
  return this->refcount_++;
}

CORBA::ULong
TAO_EC_ProxyPushConsumer::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);
    this->refcount_--;
    if (this->refcount_ != 0)
      return this->refcount_;
  }

  // Notify the event channel
  this->event_channel_->destroy_proxy_push_consumer (this);
  return 0;
}

void
TAO_EC_ProxyPushConsumer::connect_push_supplier (
      RtecEventComm::PushSupplier_ptr push_supplier,
      const RtecEventChannelAdmin::SupplierQOS& qos,
      CORBA::Environment &ACE_TRY_ENV)
{
  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

    if (this->is_connected_i ())
      ACE_THROW (RtecEventChannelAdmin::AlreadyConnected ());

    this->supplier_ =
      RtecEventComm::PushSupplier::_duplicate (push_supplier);
    this->qos_ = qos;

    this->filter_ =
      this->event_channel_->supplier_filter_builder ()->create (this->qos_);
    this->filter_->bind (this);
  }

  // Notify the event channel...
  this->event_channel_->connected (this, ACE_TRY_ENV);
}

void
TAO_EC_ProxyPushConsumer::push (const RtecEventComm::EventSet& event,
                                CORBA::Environment &ACE_TRY_ENV)
{
  if (this->is_connected () == 0)
    return; // @@ THROW something???

  // No need to keep the lock, the filter_ class is supposed to be
  // thread safe....
  this->filter_->push (event, ACE_TRY_ENV);
}

void
TAO_EC_ProxyPushConsumer::disconnect_push_consumer (
      CORBA::Environment &ACE_TRY_ENV)
{
  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

    if (this->is_connected_i () == 0)
      ACE_THROW (CORBA::BAD_INV_ORDER ());

    this->supplier_ =
      RtecEventComm::PushSupplier::_nil ();

    this->filter_->unbind (this);
    this->event_channel_->supplier_filter_builder ()->destroy (this->filter_);
    this->filter_ = 0;
  }

  PortableServer::POA_var poa =
    this->_default_POA (ACE_TRY_ENV);
  ACE_CHECK;
  PortableServer::ObjectId_var id =
    poa->servant_to_id (this, ACE_TRY_ENV);
  ACE_CHECK;
  poa->deactivate_object (id.in (), ACE_TRY_ENV);
  ACE_CHECK;

  // Notify the event channel...
  this->event_channel_->disconnected (this, ACE_TRY_ENV);

  this->_decr_refcnt ();
}

void
TAO_EC_ProxyPushConsumer::_add_ref (CORBA::Environment &)
{
  this->_incr_refcnt ();
}

void
TAO_EC_ProxyPushConsumer::_remove_ref (CORBA::Environment &)
{
  this->_decr_refcnt ();
}
