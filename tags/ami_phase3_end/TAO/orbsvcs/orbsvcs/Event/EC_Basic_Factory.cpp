// $Id$

#include "EC_Basic_Factory.h"
#include "EC_Dispatching.h"
#include "EC_Basic_Filter_Builder.h"
#include "EC_Trivial_Supplier_Filter.h"
#include "EC_ConsumerAdmin.h"
#include "EC_SupplierAdmin.h"
#include "EC_ProxyConsumer.h"
#include "EC_ProxySupplier.h"
#include "EC_SupplierFiltering.h"
#include "EC_ObserverStrategy.h"
#include "EC_Null_Scheduling.h"
#include "EC_ProxyPushSupplier_Set_T.h"
#include "EC_Reactive_Timeout_Generator.h"

#if ! defined (__ACE_INLINE__)
#include "EC_Basic_Factory.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Basic_Factory, "$Id$")

TAO_EC_Basic_Factory::~TAO_EC_Basic_Factory (void)
{
}

TAO_EC_Dispatching*
TAO_EC_Basic_Factory::create_dispatching (TAO_EC_Event_Channel *)
{
  return new TAO_EC_Reactive_Dispatching ();
}

void
TAO_EC_Basic_Factory::destroy_dispatching (TAO_EC_Dispatching *x)
{
  delete x;
}

TAO_EC_Filter_Builder*
TAO_EC_Basic_Factory::create_filter_builder (TAO_EC_Event_Channel *ec)
{
  return new TAO_EC_Basic_Filter_Builder (ec);
}

void
TAO_EC_Basic_Factory::destroy_filter_builder (TAO_EC_Filter_Builder *x)
{
  delete x;
}

TAO_EC_Supplier_Filter_Builder*
TAO_EC_Basic_Factory::create_supplier_filter_builder (TAO_EC_Event_Channel *ec)
{
  return new TAO_EC_Trivial_Supplier_Filter_Builder (ec);
}

void
TAO_EC_Basic_Factory::destroy_supplier_filter_builder (TAO_EC_Supplier_Filter_Builder *x)
{
  delete x;
}

TAO_EC_ConsumerAdmin*
TAO_EC_Basic_Factory::create_consumer_admin (TAO_EC_Event_Channel *ec)
{
  return new TAO_EC_ConsumerAdmin (ec);
}

void
TAO_EC_Basic_Factory::destroy_consumer_admin (TAO_EC_ConsumerAdmin *x)
{
  delete x;
}

TAO_EC_SupplierAdmin*
TAO_EC_Basic_Factory::create_supplier_admin (TAO_EC_Event_Channel *ec)
{
  return new TAO_EC_SupplierAdmin (ec);
}

void
TAO_EC_Basic_Factory::destroy_supplier_admin (TAO_EC_SupplierAdmin *x)
{
  delete x;
}

TAO_EC_ProxyPushSupplier*
TAO_EC_Basic_Factory::create_proxy_push_supplier (TAO_EC_Event_Channel *ec)
{
  return new TAO_EC_ProxyPushSupplier (ec);
}

void
TAO_EC_Basic_Factory::destroy_proxy_push_supplier (TAO_EC_ProxyPushSupplier *x)
{
  delete x;
}

TAO_EC_ProxyPushConsumer*
TAO_EC_Basic_Factory::create_proxy_push_consumer (TAO_EC_Event_Channel *ec)
{
  return new TAO_EC_ProxyPushConsumer (ec);
}

void
TAO_EC_Basic_Factory::destroy_proxy_push_consumer (TAO_EC_ProxyPushConsumer *x)
{
  delete x;
}

TAO_EC_Timeout_Generator*
TAO_EC_Basic_Factory::create_timeout_generator (TAO_EC_Event_Channel *ec)
{
  // @@ TODO fixme
  TAO_ORB_Core* orb_core = TAO_ORB_Core_instance ();
  return new TAO_EC_Reactive_Timeout_Generator (orb_core->reactor ());
}

void
TAO_EC_Basic_Factory::destroy_timeout_generator (TAO_EC_Timeout_Generator *x)
{
  delete x;
}

TAO_EC_ObserverStrategy*
TAO_EC_Basic_Factory::create_observer_strategy (TAO_EC_Event_Channel *ec)
{
  ACE_Lock* lock;
  ACE_NEW_RETURN (lock, ACE_Lock_Adapter<ACE_SYNCH_MUTEX>, 0);
  return new TAO_EC_Basic_ObserverStrategy (ec, lock);
}

void
TAO_EC_Basic_Factory::destroy_observer_strategy (TAO_EC_ObserverStrategy *x)
{
  delete x;
}

TAO_EC_ProxyPushSupplier_Set*
TAO_EC_Basic_Factory::create_proxy_push_supplier_set (TAO_EC_Event_Channel *)
{
  return new TAO_EC_ProxyPushSupplier_Set_Delayed<ACE_SYNCH> ();
}

TAO_EC_Scheduling_Strategy*
TAO_EC_Basic_Factory::create_scheduling_strategy (TAO_EC_Event_Channel*)
{
  return new TAO_EC_Null_Scheduling;
}

void
TAO_EC_Basic_Factory::destroy_scheduling_strategy (TAO_EC_Scheduling_Strategy* x)
{
  delete x;
}

void
TAO_EC_Basic_Factory::destroy_proxy_push_supplier_set (TAO_EC_ProxyPushSupplier_Set *x)
{
  delete x;
}

ACE_Lock*
TAO_EC_Basic_Factory::create_consumer_lock (void)
{
  return new ACE_Lock_Adapter<ACE_SYNCH_MUTEX> ();
}

void
TAO_EC_Basic_Factory::destroy_consumer_lock (ACE_Lock* x)
{
  delete x;
}

ACE_Lock*
TAO_EC_Basic_Factory::create_supplier_lock (void)
{
  return new ACE_Lock_Adapter<ACE_SYNCH_RECURSIVE_MUTEX> ();
}

void
TAO_EC_Basic_Factory::destroy_supplier_lock (ACE_Lock* x)
{
  delete x;
}

ACE_Lock*
TAO_EC_Basic_Factory::create_consumer_admin_lock (void)
{
  return new ACE_Lock_Adapter<ACE_SYNCH_MUTEX> ();
}

void
TAO_EC_Basic_Factory::destroy_consumer_admin_lock (ACE_Lock* x)
{
  delete x;
}

ACE_Lock*
TAO_EC_Basic_Factory::create_supplier_admin_lock (void)
{
  return new ACE_Lock_Adapter<ACE_SYNCH_MUTEX> ();
}

void
TAO_EC_Basic_Factory::destroy_supplier_admin_lock (ACE_Lock* x)
{
  delete x;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_EC_ProxyPushSupplier_Set_Delayed<ACE_SYNCH>;
template class ACE_Node<ACE_Command_Base*>;
template class ACE_Unbounded_Queue<ACE_Command_Base*>;
template class ACE_Unbounded_Queue_Iterator<ACE_Command_Base*>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_EC_ProxyPushSupplier_Set_Delayed<ACE_SYNCH>
#pragma instantiate ACE_Node<ACE_Command_Base*>
#pragma instantiate ACE_Unbounded_Queue<ACE_Command_Base*>
#pragma instantiate ACE_Unbounded_Queue_Iterator<ACE_Command_Base*>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
