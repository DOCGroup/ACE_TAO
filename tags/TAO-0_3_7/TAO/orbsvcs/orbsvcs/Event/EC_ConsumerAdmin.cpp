// $Id$

#include "orbsvcs/RtecEventChannelAdminC.h"
#include "EC_ConsumerAdmin.h"
#include "EC_ProxySupplier.h"
#include "EC_Event_Channel.h"
#include "EC_Command.h"

#if ! defined (__ACE_INLINE__)
#include "EC_ConsumerAdmin.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_ConsumerAdmin, "$Id$")

TAO_EC_ConsumerAdmin::TAO_EC_ConsumerAdmin (TAO_EC_Event_Channel *ec)
  :  busy_lock_ (this),
     event_channel_ (ec),
     busy_hwm_ (1)
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
  ACE_GUARD_THROW (TAO_EC_ConsumerAdmin::Busy_Lock,
      ace_mon, this->busy_lock_,
      RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

  SupplierSetIterator end = this->end ();
  for (SupplierSetIterator i = this->begin ();
       i != end;
       ++i)
    {
      (*i)->connected (consumer, ACE_TRY_ENV);
      ACE_CHECK;
    }
}

void
TAO_EC_ConsumerAdmin::disconnected (TAO_EC_ProxyPushConsumer *consumer,
				    CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD_THROW (TAO_EC_ConsumerAdmin::Busy_Lock,
      ace_mon, this->busy_lock_,
      RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

  SupplierSetIterator end = this->end ();
  for (SupplierSetIterator i = this->begin ();
       i != end;
       ++i)
    {
      (*i)->disconnected (consumer, ACE_TRY_ENV);
      ACE_CHECK;
    }
}

void
TAO_EC_ConsumerAdmin::connected_i (TAO_EC_ProxyPushSupplier *supplier,
                                   CORBA::Environment &ACE_TRY_ENV)
{
  if (this->all_suppliers_.insert (supplier) != 0)
    ACE_THROW (CORBA::NO_MEMORY (CORBA::COMPLETED_NO));
}

void
TAO_EC_ConsumerAdmin::disconnected_i (TAO_EC_ProxyPushSupplier *supplier,
                                      CORBA::Environment &ACE_TRY_ENV)
{
  if (this->all_suppliers_.remove (supplier) != 0)
    ACE_THROW (RtecEventChannelAdmin::EventChannel::SUBSCRIPTION_ERROR ());
  supplier->_decr_refcnt ();
}

RtecEventChannelAdmin::ProxyPushSupplier_ptr
TAO_EC_ConsumerAdmin::obtain_push_supplier (CORBA::Environment &ACE_TRY_ENV)
{
  TAO_EC_ProxyPushSupplier* supplier =
    this->event_channel_->create_proxy_push_supplier ();

  PortableServer::POA_var poa =
    this->event_channel_->supplier_poa (ACE_TRY_ENV);
  supplier->set_default_POA (poa.in ());

  return supplier->_this (ACE_TRY_ENV);
}

PortableServer::POA_ptr
TAO_EC_ConsumerAdmin::_default_POA (CORBA::Environment&)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

void
TAO_EC_ConsumerAdmin::execute_delayed_operations (void)
{
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Node<TAO_EC_ProxyPushSupplier*>;
template class ACE_Unbounded_Set<TAO_EC_ProxyPushSupplier*>;
template class ACE_Unbounded_Set_Iterator<TAO_EC_ProxyPushSupplier*>;
template class TAO_EC_Busy_Lock_Adapter<TAO_EC_ConsumerAdmin>;
template class TAO_EC_Connected_Command<TAO_EC_ConsumerAdmin,TAO_EC_ProxyPushSupplier>;
template class TAO_EC_Disconnected_Command<TAO_EC_ConsumerAdmin,TAO_EC_ProxyPushSupplier>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Node<TAO_EC_ProxyPushSupplier*>
#pragma instantiate ACE_Unbounded_Set<TAO_EC_ProxyPushSupplier*>
#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_EC_ProxyPushSupplier*>
#pragma instantiate TAO_EC_Busy_Lock_Adapter<TAO_EC_ConsumerAdmin>
#pragma instantiate TAO_EC_Connected_Command<TAO_EC_ConsumerAdmin,TAO_EC_ProxyPushSupplier>
#pragma instantiate TAO_EC_Disconnected_Command<TAO_EC_ConsumerAdmin,TAO_EC_ProxyPushSupplier>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
