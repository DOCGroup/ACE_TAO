// $Id$

#include "EC_SupplierAdmin.h"
#include "EC_ProxyConsumer.h"
#include "EC_Event_Channel.h"

#if ! defined (__ACE_INLINE__)
#include "EC_SupplierAdmin.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_SupplierAdmin, "$Id$")

TAO_EC_SupplierAdmin::TAO_EC_SupplierAdmin (TAO_EC_Event_Channel *ec)
  :  event_channel_ (ec)
{
}

TAO_EC_SupplierAdmin::~TAO_EC_SupplierAdmin (void)
{
}

void
TAO_EC_SupplierAdmin::set_default_POA (PortableServer::POA_ptr poa)
{
  this->default_POA_ = 
    PortableServer::POA::_duplicate (poa);
}

PortableServer::POA_ptr
TAO_EC_SupplierAdmin::_default_POA (CORBA::Environment&)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

void
TAO_EC_SupplierAdmin::connected (TAO_EC_ProxyPushSupplier *supplier,
				 CORBA::Environment &ACE_TRY_ENV)
{
  ConsumerSetIterator end = this->all_consumers_.end ();
  for (ConsumerSetIterator i = this->all_consumers_.begin ();
       i != end;
       ++i)
    {
      (*i)->connected (supplier, ACE_TRY_ENV);
    }
}

void
TAO_EC_SupplierAdmin::disconnected (TAO_EC_ProxyPushSupplier *supplier,
				    CORBA::Environment &ACE_TRY_ENV)
{
  ConsumerSetIterator end = this->all_consumers_.end ();
  for (ConsumerSetIterator i = this->all_consumers_.begin ();
       i != end;
       ++i)
    {
      (*i)->disconnected (supplier, ACE_TRY_ENV);
    }
}

void
TAO_EC_SupplierAdmin::connected (TAO_EC_ProxyPushConsumer *consumer,
				 CORBA::Environment &ACE_TRY_ENV)
{
  if (this->all_consumers_.insert (consumer) != 0)
    ACE_THROW (CORBA::NO_MEMORY (CORBA::COMPLETED_NO));
}

void
TAO_EC_SupplierAdmin::disconnected (TAO_EC_ProxyPushConsumer *consumer,
				    CORBA::Environment &ACE_TRY_ENV)
{
  if (this->all_consumers_.remove (consumer) != 0)
    ACE_THROW (RtecEventChannelAdmin::EventChannel::SUBSCRIPTION_ERROR ());
}

RtecEventChannelAdmin::ProxyPushConsumer_ptr
TAO_EC_SupplierAdmin::obtain_push_consumer (CORBA::Environment &ACE_TRY_ENV)
{
  TAO_EC_ProxyPushConsumer* consumer = 
    this->event_channel_->create_proxy_push_consumer ();

  PortableServer::POA_var poa =
    this->event_channel_->consumer_poa (ACE_TRY_ENV);
  consumer->set_default_POA (poa.in ());

  return consumer->_this (ACE_TRY_ENV);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Node<TAO_EC_ProxyPushConsumer*>;
template class ACE_Unbounded_Set<TAO_EC_ProxyPushConsumer*>;
template class ACE_Unbounded_Set_Iterator<TAO_EC_ProxyPushConsumer*>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Node<TAO_EC_ProxyPushConsumer*>
#pragma instantiate ACE_Unbounded_Set<TAO_EC_ProxyPushConsumer*>
#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_EC_ProxyPushConsumer*>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
