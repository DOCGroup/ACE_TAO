// $Id$

#include "EC_ProxyConsumer.h"
#include "EC_Event_Channel.h"

#if ! defined (__ACE_INLINE__)
#include "EC_ProxyConsumer.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_ProxyConsumer, "$Id$")

TAO_EC_ProxyPushConsumer::TAO_EC_ProxyPushConsumer (TAO_EC_Event_Channel* ec)
  : event_channel_ (ec)
{
}

TAO_EC_ProxyPushConsumer::~TAO_EC_ProxyPushConsumer (void)
{
}

void
TAO_EC_ProxyPushConsumer::set_default_POA (PortableServer::POA_ptr poa)
{
  this->default_POA_ =
    PortableServer::POA::_duplicate (poa);
}

PortableServer::POA_ptr
TAO_EC_ProxyPushConsumer::_default_POA (CORBA::Environment&)
{
  return PortableServer::POA::_duplicate (this->default_POA_);
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

void
TAO_EC_ProxyPushConsumer::connect_push_supplier (
      RtecEventComm::PushSupplier_ptr push_supplier,
      const RtecEventChannelAdmin::SupplierQOS& qos,
      CORBA::Environment &ACE_TRY_ENV)
{
  if (this->is_connected ())
    ACE_THROW (RtecEventChannelAdmin::AlreadyConnected ());

  this->supplier_ =
    RtecEventComm::PushSupplier::_duplicate (push_supplier);
  this->qos_ = qos;

  // Notify the event channel...
  this->event_channel_->connected (this, ACE_TRY_ENV);
}

void
TAO_EC_ProxyPushConsumer::disconnect_push_consumer (
      CORBA::Environment &ACE_TRY_ENV)
{
  this->supplier_ = 
    RtecEventComm::PushSupplier::_nil ();

  PortableServer::POA_var poa =
    this->_default_POA (ACE_TRY_ENV);
  ACE_CHECK;
  PortableServer::ObjectId_var id =
    poa->servant_to_id (this, ACE_TRY_ENV);
  ACE_CHECK;
  poa->deactivate_object (id.in (), ACE_TRY_ENV);
  ACE_CHECK;

  this->event_channel_->destroy_proxy_push_consumer (this);
}

