// $Id$

#include "EC_ProxySupplier.h"
#include "EC_Dispatching.h"
#include "EC_Filter_Builder.h"
#include "EC_Event_Channel.h"

#if ! defined (__ACE_INLINE__)
#include "EC_ProxySupplier.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_ProxySupplier, "$Id$")

typedef ACE_Reverse_Lock<ACE_Lock> TAO_EC_Unlock;

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
  delete this->child_;
  this->child_ = 0;
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
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
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
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

    if (this->is_connected_i ())
      ACE_THROW (RtecEventChannelAdmin::AlreadyConnected ());

    this->consumer_ =
      RtecEventComm::PushConsumer::_duplicate (push_consumer);
    this->qos_ = qos;

    this->child_ =
      this->event_channel_->filter_builder ()->build (this,
                                                      this->qos_);
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
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

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

  // Notify the event channel...
  this->event_channel_->disconnected (this, ACE_TRY_ENV);

  this->_decr_refcnt ();
}

void
TAO_EC_ProxyPushSupplier::suspend_connection (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD_THROW_EX (
            ACE_Lock, ace_mon, *this->lock_,
            RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
  ACE_CHECK;

  this->suspended_ = 1;
}

void
TAO_EC_ProxyPushSupplier::resume_connection (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD_THROW_EX (
            ACE_Lock, ace_mon, *this->lock_,
            RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
  ACE_CHECK;

  this->suspended_ = 0;
}

int
TAO_EC_ProxyPushSupplier::filter (const RtecEventComm::EventSet& event,
                                  TAO_EC_QOS_Info& qos_info,
                                  CORBA::Environment& ACE_TRY_ENV)
{
  ACE_GUARD_THROW_EX (
            ACE_Lock, ace_mon, *this->lock_,
            RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
  ACE_CHECK_RETURN (0);

  return this->child_->filter (event, qos_info, ACE_TRY_ENV);
}

int
TAO_EC_ProxyPushSupplier::filter_nocopy (RtecEventComm::EventSet& event,
                                         TAO_EC_QOS_Info& qos_info,
                                         CORBA::Environment& ACE_TRY_ENV)
{
  ACE_GUARD_THROW_EX (
            ACE_Lock, ace_mon, *this->lock_,
            RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
  ACE_CHECK_RETURN (0);

  return this->child_->filter_nocopy (event, qos_info, ACE_TRY_ENV);
}

void
TAO_EC_ProxyPushSupplier::push (const RtecEventComm::EventSet& event,
                                TAO_EC_QOS_Info& qos_info,
                                CORBA::Environment& ACE_TRY_ENV)
{
  this->event_channel_->dispatching ()->push (this,
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
  this->event_channel_->dispatching ()->push_nocopy (this,
                                                     event,
                                                     qos_info,
                                                     ACE_TRY_ENV);
  this->child_->clear ();
}

void
TAO_EC_ProxyPushSupplier::push_to_consumer (const RtecEventComm::EventSet& event,
                                            CORBA::Environment& ACE_TRY_ENV)
{
  RtecEventComm::PushConsumer_var consumer;
  {
    ACE_GUARD_THROW_EX (
            ACE_Lock, ace_mon, *this->lock_,
            RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

    if (this->is_connected_i () == 0)
      return; // TAO_THROW (RtecEventComm::Disconnected ());????

    if (this->suspended_ != 0)
      return;

    consumer =
      RtecEventComm::PushConsumer::_duplicate (this->consumer_.in ());
  }

  consumer->push (event, ACE_TRY_ENV);
}

void
TAO_EC_ProxyPushSupplier::reactive_push_to_consumer (
    const RtecEventComm::EventSet& event,
    CORBA::Environment& ACE_TRY_ENV)
{
  if (this->is_connected_i () == 0)
    return; // TAO_THROW (RtecEventComm::Disconnected ());????

  if (this->suspended_ != 0)
    return;

  RtecEventComm::PushConsumer_var consumer =
    RtecEventComm::PushConsumer::_duplicate (this->consumer_.in ());
  {
    TAO_EC_Unlock reverse_lock (*this->lock_);

    ACE_GUARD_THROW_EX (TAO_EC_Unlock, ace_mon, reverse_lock,
                        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;
    consumer->push (event, ACE_TRY_ENV);
  }
}

void
TAO_EC_ProxyPushSupplier::push_timeout (
    TAO_EC_Filter* timeout_filter,
    const RtecEventComm::EventSet &event,
    TAO_EC_QOS_Info& qos_info,
    CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD_THROW_EX (
          ACE_Lock, ace_mon, *this->lock_,
          RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
  ACE_CHECK;

  timeout_filter->push (event, qos_info, ACE_TRY_ENV);
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Reverse_Lock<ACE_Lock>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Reverse_Lock<ACE_Lock>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
