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
  : event_channel_ (ec)
{
}

TAO_EC_ProxyPushSupplier::~TAO_EC_ProxyPushSupplier (void)
{
}

void
TAO_EC_ProxyPushSupplier::set_default_POA (PortableServer::POA_ptr poa)
{
  this->default_POA_ = PortableServer::POA::_duplicate (poa);
}

PortableServer::POA_ptr
TAO_EC_ProxyPushSupplier::_default_POA (CORBA::Environment&)
{
  return PortableServer::POA::_duplicate (this->default_POA_);
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
  if (this->is_connected ())
    ACE_THROW (RtecEventChannelAdmin::AlreadyConnected ());

  this->consumer_ =
    RtecEventComm::PushConsumer::_duplicate (push_consumer);
  this->qos_ = qos;

  this->child_ =
    this->event_channel_->filter_builder ()->build (this->qos_);
  this->adopt_child (this->child_);

  // Notify the event channel...
  this->event_channel_->connected (this, ACE_TRY_ENV);
}

void
TAO_EC_ProxyPushSupplier::disconnect_push_supplier (
      CORBA::Environment &ACE_TRY_ENV)
{
  this->consumer_ = 
    RtecEventComm::PushConsumer::_nil ();

  PortableServer::POA_var poa =
    this->_default_POA (ACE_TRY_ENV);
  ACE_CHECK;
  PortableServer::ObjectId_var id =
    poa->servant_to_id (this, ACE_TRY_ENV);
  ACE_CHECK;
  poa->deactivate_object (id.in (), ACE_TRY_ENV);
  ACE_CHECK;

  this->event_channel_->destroy_proxy_push_supplier (this);
}

void
TAO_EC_ProxyPushSupplier::suspend_connection (CORBA::Environment &ACE_TRY_ENV)
{
  this->suspended_ = 1;
}

void
TAO_EC_ProxyPushSupplier::resume_connection (CORBA::Environment &ACE_TRY_ENV)
{
  this->suspended_ = 0;
}

int
TAO_EC_ProxyPushSupplier::filter (const RtecEventComm::EventSet& event,
                                  const TAO_EC_QOS_Info& qos_info,
                                  CORBA::Environment& ACE_TRY_ENV)
{
  return this->child_->filter (event, qos_info, ACE_TRY_ENV);
}

void
TAO_EC_ProxyPushSupplier::push (const RtecEventComm::EventSet& event,
                                const TAO_EC_QOS_Info& qos_info,
                                CORBA::Environment& ACE_TRY_ENV)
{
  this->event_channel_->dispatching ()->push (this,
                                              event,
                                              qos_info,
                                              ACE_TRY_ENV);
  this->child_->clear ();
}

void
TAO_EC_ProxyPushSupplier::clear (void)
{
  this->child_->clear ();
}

CORBA::ULong
TAO_EC_ProxyPushSupplier::max_event_size (void) const
{
  return this->child_->max_event_size ();
}

void
TAO_EC_ProxyPushSupplier::event_ids (TAO_EC_Filter::Headers& headerset)
{
  this->child_->event_ids (headerset);
}
