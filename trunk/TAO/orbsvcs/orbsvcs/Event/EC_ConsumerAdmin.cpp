// $Id$

#include "EC_ConsumerAdmin.h"
#include "EC_Event_Channel.h"

#if ! defined (__ACE_INLINE__)
#include "EC_ConsumerAdmin.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_ConsumerAdmin, "$Id$")

TAO_EC_ConsumerAdmin::TAO_EC_ConsumerAdmin (TAO_EC_Event_Channel *ec)
  :  event_channel_ (ec)
{
}

TAO_EC_ConsumerAdmin::~TAO_EC_ConsumerAdmin (void)
{
}

void
TAO_EC_ConsumerAdmin::set_default_POA (PortableServer::POA_ptr poa)
{
  this->default_POA_ = 
    PortableServer::POA::_duplicate (poa);
}

void
TAO_EC_ConsumerAdmin::connected (TAO_EC_ProxyPushConsumer *consumer,
				 CORBA::Environment &ACE_TRY_ENV)
{
  SupplierSetIterator end = this->all_suppliers_.end ();
  for (SupplierSetIterator i = this->all_suppliers_.begin ();
       i != end;
       ++i)
    {
      (*i)->connected (consumer, ACE_TRY_ENV);
    }
}

void
TAO_EC_ConsumerAdmin::disconnected (TAO_EC_ProxyPushConsumer *consumer,
				    CORBA::Environment &ACE_TRY_ENV)
{
  SupplierSetIterator end = this->all_suppliers_.end ();
  for (SupplierSetIterator i = this->all_suppliers_.begin ();
       i != end;
       ++i)
    {
      (*i)->disconnected (consumer, ACE_TRY_ENV);
    }
}

void
TAO_EC_ConsumerAdmin::connected (TAO_EC_ProxyPushSupplier *supplier,
				 CORBA::Environment &ACE_TRY_ENV)
{
  if (this->all_suppliers_.insert (supplier) != 0)
    ACE_THROW (CORBA::NO_MEMORY (CORBA::COMPLETED_NO));
}

void
TAO_EC_ConsumerAdmin::disconnected (TAO_EC_ProxyPushSupplier *supplier,
				    CORBA::Environment &ACE_TRY_ENV)
{
  if (this->all_suppliers_.remove (supplier) != 0)
    ACE_THROW (RtecEventChannelAdmin::SUBSCRIPTION_ERROR ());
}

RtecEventChannelAdmin::ProxyPushSupplier_ptr
TAO_EC_ConsumerAdmin::obtain_push_supplier (CORBA::Environment &ACE_TRY_ENV)
{
  TAO_EC_ProxyPushSupplier* supplier = 
    this->event_channel_->create_proxy_push_supplier (void);

  supplier->set_default_POA (
        this->event_channel_->factory ()->supplier_POA ());

  return supplier->_this (ACE_TRY_ENV);
}

PortableServer::POA_ptr
TAO_EC_ConsumerAdmin::_default_POA (CORBA::Environment&)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}
