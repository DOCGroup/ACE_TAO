// $Id$

#include "EC_ProxySupplier.h"
#include "EC_Dispatching.h"
#include "EC_Filter_Builder.h"
#include "EC_Event_Channel.h"

#if ! defined (__ACE_INLINE__)
#include "EC_ProxySupplier.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_ProxySupplier, "$Id$")

TAO_EC_ProxyPushSupplier::TAO_EC_ProxyPushSupplier (TAO_EC_Event_Channel* ec)
  : event_channel_ (ec),
    refcount_ (1),
    suspended_ (0),
    child_ (0)
{
  this->lock_ =
    this->event_channel_->create_supplier_lock ();
}

TAO_EC_ProxyPushSupplier::~TAO_EC_ProxyPushSupplier (void)
{
  this->event_channel_->destroy_supplier_lock (this->lock_);
}

CORBA::ULong
TAO_EC_ProxyPushSupplier::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);
  return this->refcount_++;
}

CORBA::ULong
TAO_EC_ProxyPushSupplier::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);
    this->refcount_--;
    if (this->refcount_ != 0)
      return this->refcount_;
  }

  // Notify the event channel
  this->event_channel_->destroy_proxy_push_supplier (this);
  return 0;
}

void
TAO_EC_ProxyPushSupplier::set_default_POA (PortableServer::POA_ptr poa)
{
  ACE_GUARD (ACE_Lock, ace_mon, *this->lock_);
  this->default_POA_ = PortableServer::POA::_duplicate (poa);
}

PortableServer::POA_ptr
TAO_EC_ProxyPushSupplier::_default_POA_i ()
{
  return PortableServer::POA::_duplicate (this->default_POA_);
}

PortableServer::POA_ptr
TAO_EC_ProxyPushSupplier::_default_POA (CORBA::Environment&)
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_,
                    PortableServer::POA::_nil ());
  return this->_default_POA_i ();
}

void
TAO_EC_ProxyPushSupplier::connected (TAO_EC_ProxyPushConsumer*,
                                     CORBA::Environment &)
{
}

void
TAO_EC_ProxyPushSupplier::disconnected (TAO_EC_ProxyPushConsumer*,
                                        CORBA::Environment &)
{
}

void
TAO_EC_ProxyPushSupplier::connected (TAO_EC_ProxyPushSupplier*,
                                     CORBA::Environment &)
{
}

void
TAO_EC_ProxyPushSupplier::disconnected (TAO_EC_ProxyPushSupplier*,
                                        CORBA::Environment &)
{
}

void
TAO_EC_ProxyPushSupplier::connect_push_consumer (
      RtecEventComm::PushConsumer_ptr push_consumer,
      const RtecEventChannelAdmin::ConsumerQOS& qos,
      CORBA::Environment &ACE_TRY_ENV)
{
  {
    ACE_GUARD_THROW (
        ACE_Lock, ace_mon, *this->lock_,
        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

    if (this->is_connected_i ())
      ACE_THROW (RtecEventChannelAdmin::AlreadyConnected ());

    this->consumer_ =
      RtecEventComm::PushConsumer::_duplicate (push_consumer);
    this->qos_ = qos;

    this->child_ =
      this->event_channel_->filter_builder ()->build (this->qos_);
    this->adopt_child (this->child_);
  }

  // Notify the event channel...
  this->event_channel_->connected (this, ACE_TRY_ENV);
}

void
TAO_EC_ProxyPushSupplier::disconnect_push_supplier (
      CORBA::Environment &ACE_TRY_ENV)
{
  {
    ACE_GUARD_THROW (
        ACE_Lock, ace_mon, *this->lock_,
        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

    this->consumer_ =
      RtecEventComm::PushConsumer::_nil ();

    PortableServer::POA_var poa =
      this->_default_POA_i ();

    PortableServer::ObjectId_var id =
      poa->servant_to_id (this, ACE_TRY_ENV);
    ACE_CHECK;
    poa->deactivate_object (id.in (), ACE_TRY_ENV);
    ACE_CHECK;
  }
  this->_decr_refcnt ();
}

void
TAO_EC_ProxyPushSupplier::suspend_connection (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD_THROW (
            ACE_Lock, ace_mon, *this->lock_,
            RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
  this->suspended_ = 1;
}

void
TAO_EC_ProxyPushSupplier::resume_connection (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD_THROW (
            ACE_Lock, ace_mon, *this->lock_,
            RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

  this->suspended_ = 0;
}

int
TAO_EC_ProxyPushSupplier::filter (const RtecEventComm::EventSet& event,
                                  TAO_EC_QOS_Info& qos_info,
                                  CORBA::Environment& ACE_TRY_ENV)
{
  ACE_GUARD_THROW_RETURN (
            ACE_Lock, ace_mon, *this->lock_,
            RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR (),
            0);

  return this->child_->filter (event, qos_info, ACE_TRY_ENV);
}

int
TAO_EC_ProxyPushSupplier::filter_nocopy (RtecEventComm::EventSet& event,
                                         TAO_EC_QOS_Info& qos_info,
                                         CORBA::Environment& ACE_TRY_ENV)
{
  ACE_GUARD_THROW_RETURN (
            ACE_Lock, ace_mon, *this->lock_,
            RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR (),
            0);

  return this->child_->filter_nocopy (event, qos_info, ACE_TRY_ENV);
}

void
TAO_EC_ProxyPushSupplier::push (const RtecEventComm::EventSet& event,
                                TAO_EC_QOS_Info& qos_info,
                                CORBA::Environment& ACE_TRY_ENV)
{
  // Do not take a lock, this is a call back from our child filter, so
  // we are holding the lock already (in the filter() method).
  this->event_channel_->dispatching ()->push (this,
                                              this->consumer_,
                                              event,
                                              qos_info,
                                              ACE_TRY_ENV);
  this->child_->clear ();
}

void
TAO_EC_ProxyPushSupplier::push_nocopy (RtecEventComm::EventSet& event,
                                       TAO_EC_QOS_Info& qos_info,
                                       CORBA::Environment& ACE_TRY_ENV)
{
  // Do not take a lock, this is a call back from our child filter, so
  // we are holding the lock already (in the filter() method).
  this->event_channel_->dispatching ()->push_nocopy (this,
                                                     this->consumer_,
                                                     event,
                                                     qos_info,
                                                     ACE_TRY_ENV);
  this->child_->clear ();
}

void
TAO_EC_ProxyPushSupplier::clear (void)
{
  ACE_GUARD (ACE_Lock, ace_mon, *this->lock_);

  this->child_->clear ();
}

CORBA::ULong
TAO_EC_ProxyPushSupplier::max_event_size (void) const
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);

  return this->child_->max_event_size ();
}

int
TAO_EC_ProxyPushSupplier::can_match (
      const RtecEventComm::EventHeader &header) const
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);

  return this->child_->can_match (header);
}
