// $Id$

#include "EC_Basic_Factory.h"
#include "EC_Dispatching.h"
#include "EC_Basic_Filter_Builder.h"
#include "EC_Trivial_Supplier_Filter.h"
#include "EC_ConsumerAdmin.h"
#include "EC_SupplierAdmin.h"
#include "EC_ProxyConsumer.h"
#include "EC_ProxySupplier.h"
#include "EC_ObserverStrategy.h"
#include "EC_Null_Scheduling.h"
#include "EC_Reactive_Timeout_Generator.h"
#include "EC_Reactive_ConsumerControl.h"
#include "EC_Reactive_SupplierControl.h"
#include "EC_Event_Channel.h" // @@ MSVC 6 bug
#include "orbsvcs/ESF/ESF_Proxy_List.h"
#include "orbsvcs/ESF/ESF_Delayed_Changes.h"

#include "tao/ORB_Core.h"

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
  return new TAO_EC_ProxyPushSupplier (ec, TAO_EC_DEFAULT_CONSUMER_VALIDATE_CONNECTION);
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
TAO_EC_Basic_Factory::create_timeout_generator (TAO_EC_Event_Channel *)
{
  int argc = 0;
  char **argv = 0;
  CORBA::ORB_var orb =
    CORBA::ORB_init (argc, argv, "");
  ACE_Reactor *reactor = orb->orb_core ()->reactor ();
  return new TAO_EC_Reactive_Timeout_Generator (reactor);
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
  ACE_NEW_RETURN (lock, ACE_Lock_Adapter<TAO_SYNCH_MUTEX>, 0);
  return new TAO_EC_Basic_ObserverStrategy (ec, lock);
}

void
TAO_EC_Basic_Factory::destroy_observer_strategy (TAO_EC_ObserverStrategy *x)
{
  delete x;
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

TAO_EC_ProxyPushConsumer_Collection*
TAO_EC_Basic_Factory::create_proxy_push_consumer_collection (TAO_EC_Event_Channel *)
{
  // This typedef is a workaround for a SunCC 4.2 bug
  typedef TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>::Iterator TAO_EC_Consumer_List_Iterator;
  return new TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>::Iterator,
      ACE_SYNCH> ();
}

void
TAO_EC_Basic_Factory::destroy_proxy_push_consumer_collection (TAO_EC_ProxyPushConsumer_Collection *x)
{
  delete x;
}

TAO_EC_ProxyPushSupplier_Collection*
TAO_EC_Basic_Factory::create_proxy_push_supplier_collection (TAO_EC_Event_Channel *)
{
  // This typedef is a workaround for a SunCC 4.2 bug
  typedef TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>::Iterator TAO_EC_Supplier_List_Iterator;
  return new TAO_ESF_Delayed_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>::Iterator,
      ACE_SYNCH> ();
}

void
TAO_EC_Basic_Factory::destroy_proxy_push_supplier_collection (TAO_EC_ProxyPushSupplier_Collection *x)
{
  delete x;
}

ACE_Lock*
TAO_EC_Basic_Factory::create_consumer_lock (void)
{
  return new ACE_Lock_Adapter<TAO_SYNCH_MUTEX> ();
}

void
TAO_EC_Basic_Factory::destroy_consumer_lock (ACE_Lock* x)
{
  delete x;
}

ACE_Lock*
TAO_EC_Basic_Factory::create_supplier_lock (void)
{
  return new ACE_Lock_Adapter<TAO_SYNCH_RECURSIVE_MUTEX> ();
}

void
TAO_EC_Basic_Factory::destroy_supplier_lock (ACE_Lock* x)
{
  delete x;
}

TAO_EC_ConsumerControl*
TAO_EC_Basic_Factory::create_consumer_control (TAO_EC_Event_Channel* ec)
{
  int argc = 0;
  char **argv = 0;
  CORBA::ORB_var orb =
    CORBA::ORB_init (argc, argv, "");
  // Hard-coded rate to 10 times a second
  ACE_Time_Value rate (0, 100000);
  // Hard-coded polling-timeout to 10 msec
  ACE_Time_Value timeout (0, TAO_EC_DEFAULT_CONSUMER_CONTROL_TIMEOUT);
  return new TAO_EC_Reactive_ConsumerControl (rate, timeout, ec, orb.in ());
}

void
TAO_EC_Basic_Factory::destroy_consumer_control (TAO_EC_ConsumerControl* x)
{
  delete x;
}

TAO_EC_SupplierControl*
TAO_EC_Basic_Factory::create_supplier_control (TAO_EC_Event_Channel* ec)
{
  int argc = 0;
  char **argv = 0;
  CORBA::ORB_var orb =
    CORBA::ORB_init (argc, argv, "");
  // Hard-coded rate to 10 times a second
  ACE_Time_Value rate (0, 100000);
  // Hard-coded polling-timeout to 10 msec
  ACE_Time_Value timeout (0, TAO_EC_DEFAULT_SUPPLIER_CONTROL_TIMEOUT);
  return new TAO_EC_Reactive_SupplierControl (rate, timeout, ec, orb.in ());
}

void
TAO_EC_Basic_Factory::destroy_supplier_control (TAO_EC_SupplierControl* x)
{
  delete x;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
