// $Id$

#include "EC_ProxyPushSupplier_Set.h"
#include "EC_Defaults.h"
#include "EC_ProxySupplier.h"
#include "EC_Command.h"

#if ! defined (__ACE_INLINE__)
#include "EC_ProxyPushSupplier_Set.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_ProxyPushSupplier_Set, "$Id$")

TAO_EC_ProxyPushSupplier_Set::TAO_EC_ProxyPushSupplier_Set (void)
  :  busy_lock_ (this),
     busy_hwm_ (TAO_EC_DEFAULT_BUSY_HWM),
     max_write_delay_ (TAO_EC_DEFAULT_MAX_WRITE_DELAY)
{
}

TAO_EC_ProxyPushSupplier_Set::~TAO_EC_ProxyPushSupplier_Set (void)
{
}

void
TAO_EC_ProxyPushSupplier_Set::connected_i (
      TAO_EC_ProxyPushSupplier *supplier,
      CORBA::Environment &ACE_TRY_ENV)
{
  if (this->all_suppliers_.insert (supplier) != 0)
    ACE_THROW (CORBA::NO_MEMORY ());
}

void
TAO_EC_ProxyPushSupplier_Set::disconnected_i (
      TAO_EC_ProxyPushSupplier *supplier,
      CORBA::Environment &ACE_TRY_ENV)
{
  if (this->all_suppliers_.remove (supplier) != 0)
    return; // ACE_THROW (RtecEventChannelAdmin::EventChannel::SUBSCRIPTION_ERROR ());
  supplier->_decr_refcnt ();
}

void
TAO_EC_ProxyPushSupplier_Set::shutdown_i (
      CORBA::Environment &ACE_TRY_ENV)
{
  SupplierSetIterator end = this->end ();
  for (SupplierSetIterator i = this->begin (); i != end; ++i)
    {
      (*i)->_decr_refcnt ();
    }
  this->all_suppliers_.reset ();
}

void
TAO_EC_ProxyPushSupplier_Set::execute_delayed_operations (void)
{
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Node<TAO_EC_ProxyPushSupplier*>;
template class ACE_Unbounded_Set<TAO_EC_ProxyPushSupplier*>;
template class ACE_Unbounded_Set_Iterator<TAO_EC_ProxyPushSupplier*>;
template class ACE_Guard<TAO_EC_Busy_Lock_Adapter<TAO_EC_ProxyPushSupplier_Set> >;
template class TAO_EC_Busy_Lock_Adapter<TAO_EC_ProxyPushSupplier_Set>;
template class TAO_EC_Connected_Command<TAO_EC_ProxyPushSupplier_Set,TAO_EC_ProxyPushSupplier>;
template class TAO_EC_Disconnected_Command<TAO_EC_ProxyPushSupplier_Set,TAO_EC_ProxyPushSupplier>;
template class TAO_EC_Shutdown_Command<TAO_EC_ProxyPushSupplier_Set>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Node<TAO_EC_ProxyPushSupplier*>
#pragma instantiate ACE_Unbounded_Set<TAO_EC_ProxyPushSupplier*>
#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_EC_ProxyPushSupplier*>
#pragma instantiate TAO_EC_Busy_Lock_Adapter<TAO_EC_ProxyPushSupplier_Set>
#pragma instantiate ACE_Guard<TAO_EC_Busy_Lock_Adapter<TAO_EC_ProxyPushSupplier_Set> >
#pragma instantiate TAO_EC_Connected_Command<TAO_EC_ProxyPushSupplier_Set,TAO_EC_ProxyPushSupplier>
#pragma instantiate TAO_EC_Disconnected_Command<TAO_EC_ProxyPushSupplier_Set,TAO_EC_ProxyPushSupplier>
#pragma instantiate TAO_EC_Shutdown_Command<TAO_EC_ProxyPushSupplier_Set>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
