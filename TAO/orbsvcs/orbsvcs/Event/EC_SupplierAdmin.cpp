// $Id$

#include "EC_SupplierAdmin.h"
#include "EC_ProxyConsumer.h"
#include "EC_ProxySupplier.h"
#include "EC_Event_Channel.h"

#if ! defined (__ACE_INLINE__)
#include "EC_SupplierAdmin.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_SupplierAdmin, "$Id$")

TAO_EC_SupplierAdmin::TAO_EC_SupplierAdmin (TAO_EC_Event_Channel *ec)
  :  event_channel_ (ec)
{
  this->default_POA_ =
    this->event_channel_->supplier_poa ();

  // @@ We must consider using the techniques in the ConsumerAdmin
  //    also, i.e. not using locks but delaying operations that modify
  //    the set.  I don't see much use for them right now, since there
  //    is no potential for dead-lock when dispatching events and/or
  //    adding multiple suppliers and consumers, but we could avoid
  //    some priority inversions.
  this->lock_ =
    this->event_channel_->create_supplier_admin_lock ();
}

TAO_EC_SupplierAdmin::~TAO_EC_SupplierAdmin (void)
{
    this->event_channel_->destroy_supplier_admin_lock (this->lock_);
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
  ACE_GUARD_THROW_EX (
      ACE_Lock, ace_mon, *this->lock_,
      RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
  ACE_CHECK;

  ConsumerSetIterator end = this->end ();
  for (ConsumerSetIterator i = this->begin ();
       i != end;
       ++i)
    {
      (*i)->connected (supplier, ACE_TRY_ENV);
      ACE_CHECK;
      supplier->connected (*i, ACE_TRY_ENV);
      ACE_CHECK;
    }
}

void
TAO_EC_SupplierAdmin::disconnected (TAO_EC_ProxyPushSupplier *supplier,
				    CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD_THROW_EX (
      ACE_Lock, ace_mon, *this->lock_,
      RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
  ACE_CHECK;

  ConsumerSetIterator end = this->end ();
  for (ConsumerSetIterator i = this->begin ();
       i != end;
       ++i)
    {
      (*i)->disconnected (supplier, ACE_TRY_ENV);
      ACE_CHECK;
      supplier->disconnected (*i, ACE_TRY_ENV);
      ACE_CHECK;
    }
}

void
TAO_EC_SupplierAdmin::connected (TAO_EC_ProxyPushConsumer *consumer,
				 CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD_THROW_EX (
      ACE_Lock, ace_mon, *this->lock_,
      RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
  ACE_CHECK;

  if (this->all_consumers_.insert (consumer) != 0)
    ACE_THROW (CORBA::NO_MEMORY ());
}

void
TAO_EC_SupplierAdmin::disconnected (TAO_EC_ProxyPushConsumer *consumer,
				    CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD_THROW_EX (
      ACE_Lock, ace_mon, *this->lock_,
      RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
  ACE_CHECK;

  if (this->all_consumers_.remove (consumer) != 0)
    ACE_THROW (RtecEventChannelAdmin::EventChannel::SUBSCRIPTION_ERROR ());
}

void
TAO_EC_SupplierAdmin::shutdown (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD_THROW_EX (
      ACE_Lock, ace_mon, *this->lock_,
      RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
  ACE_CHECK;

  ConsumerSetIterator end = this->end ();
  for (ConsumerSetIterator i = this->begin ();
       i != end;
       ++i)
    {
      ACE_TRY
        {
          (*i)->shutdown (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          /* ignore all exceptions */
        }
      ACE_ENDTRY;
    }
}

RtecEventChannelAdmin::ProxyPushConsumer_ptr
TAO_EC_SupplierAdmin::obtain_push_consumer (CORBA::Environment &ACE_TRY_ENV)
{
  TAO_EC_ProxyPushConsumer* consumer =
    this->event_channel_->create_proxy_push_consumer ();

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
