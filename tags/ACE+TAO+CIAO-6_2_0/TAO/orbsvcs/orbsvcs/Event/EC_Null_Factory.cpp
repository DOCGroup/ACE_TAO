// $Id$

#include "orbsvcs/Event/EC_Null_Factory.h"
#include "orbsvcs/Event/EC_Reactive_Dispatching.h"
#include "orbsvcs/Event/EC_Filter_Builder.h"
#include "orbsvcs/Event/EC_Trivial_Supplier_Filter.h"
#include "orbsvcs/Event/EC_ConsumerAdmin.h"
#include "orbsvcs/Event/EC_SupplierAdmin.h"
#include "orbsvcs/Event/EC_Default_ProxyConsumer.h"
#include "orbsvcs/Event/EC_Default_ProxySupplier.h"
#include "orbsvcs/Event/EC_ObserverStrategy.h"
#include "orbsvcs/Event/EC_Null_Scheduling.h"
#include "orbsvcs/Event/EC_Reactive_Timeout_Generator.h"
#include "orbsvcs/Event/EC_ConsumerControl.h"
#include "orbsvcs/Event/EC_SupplierControl.h"
#include "orbsvcs/Event/EC_Event_Channel_Base.h" // @@ MSVC 6 bug
#include "orbsvcs/ESF/ESF_Proxy_List.h"
#include "orbsvcs/ESF/ESF_Immediate_Changes.h"

#include "tao/ORB_Core.h"



TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_EC_Null_Factory::TAO_EC_Null_Factory (void)
{
}

TAO_EC_Null_Factory::~TAO_EC_Null_Factory (void)
{
}

TAO_EC_Dispatching*
TAO_EC_Null_Factory::create_dispatching (TAO_EC_Event_Channel_Base *)
{
  return new TAO_EC_Reactive_Dispatching ();
}

void
TAO_EC_Null_Factory::destroy_dispatching (TAO_EC_Dispatching *x)
{
  delete x;
}

TAO_EC_Filter_Builder*
TAO_EC_Null_Factory::create_filter_builder (TAO_EC_Event_Channel_Base *)
{
  return new TAO_EC_Null_Filter_Builder;
}

void
TAO_EC_Null_Factory::destroy_filter_builder (TAO_EC_Filter_Builder *x)
{
  delete x;
}

TAO_EC_Supplier_Filter_Builder*
TAO_EC_Null_Factory::create_supplier_filter_builder (TAO_EC_Event_Channel_Base *ec)
{
  return new TAO_EC_Trivial_Supplier_Filter_Builder (ec);
}

void
TAO_EC_Null_Factory::destroy_supplier_filter_builder (TAO_EC_Supplier_Filter_Builder *x)
{
  delete x;
}

TAO_EC_ConsumerAdmin*
TAO_EC_Null_Factory::create_consumer_admin (TAO_EC_Event_Channel_Base *ec)
{
  return new TAO_EC_ConsumerAdmin (ec);
}

void
TAO_EC_Null_Factory::destroy_consumer_admin (TAO_EC_ConsumerAdmin *x)
{
  delete x;
}

TAO_EC_SupplierAdmin*
TAO_EC_Null_Factory::create_supplier_admin (TAO_EC_Event_Channel_Base *ec)
{
  return new TAO_EC_SupplierAdmin (ec);
}

void
TAO_EC_Null_Factory::destroy_supplier_admin (TAO_EC_SupplierAdmin *x)
{
  delete x;
}

TAO_EC_ProxyPushSupplier*
TAO_EC_Null_Factory::create_proxy_push_supplier (TAO_EC_Event_Channel_Base *ec)
{
  return new TAO_EC_Default_ProxyPushSupplier (ec, TAO_EC_DEFAULT_CONSUMER_VALIDATE_CONNECTION);
}

void
TAO_EC_Null_Factory::destroy_proxy_push_supplier (TAO_EC_ProxyPushSupplier *x)
{
  delete x;
}

TAO_EC_ProxyPushConsumer*
TAO_EC_Null_Factory::create_proxy_push_consumer (TAO_EC_Event_Channel_Base *ec)
{
  return new TAO_EC_Default_ProxyPushConsumer (ec);
}

void
TAO_EC_Null_Factory::destroy_proxy_push_consumer (TAO_EC_ProxyPushConsumer *x)
{
  delete x;
}

TAO_EC_Timeout_Generator*
TAO_EC_Null_Factory::create_timeout_generator (TAO_EC_Event_Channel_Base *)
{
  int argc = 0;
  ACE_TCHAR **argv = 0;
  CORBA::ORB_var orb =
    CORBA::ORB_init (argc, argv);
  ACE_Reactor *reactor = orb->orb_core ()->reactor ();
  return new TAO_EC_Reactive_Timeout_Generator (reactor);
}

void
TAO_EC_Null_Factory::destroy_timeout_generator (TAO_EC_Timeout_Generator *x)
{
  delete x;
}

TAO_EC_ObserverStrategy*
TAO_EC_Null_Factory::create_observer_strategy (TAO_EC_Event_Channel_Base *)
{
  return new TAO_EC_Null_ObserverStrategy;
}

void
TAO_EC_Null_Factory::destroy_observer_strategy (TAO_EC_ObserverStrategy *x)
{
  delete x;
}

TAO_EC_Scheduling_Strategy*
TAO_EC_Null_Factory::create_scheduling_strategy (TAO_EC_Event_Channel_Base*)
{
  return new TAO_EC_Null_Scheduling;
}

void
TAO_EC_Null_Factory::destroy_scheduling_strategy (TAO_EC_Scheduling_Strategy* x)
{
  delete x;
}

TAO_EC_ProxyPushConsumer_Collection*
TAO_EC_Null_Factory::create_proxy_push_consumer_collection (TAO_EC_Event_Channel_Base *)
{
  // This typedef is a workaround for a SunCC 4.2 bug
  typedef TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>::Iterator TAO_EC_Consumer_List_Iterator;
  return new TAO_ESF_Immediate_Changes<TAO_EC_ProxyPushConsumer,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushConsumer>,
      TAO_EC_Consumer_List_Iterator,
      ACE_Null_Mutex> ();
}

void
TAO_EC_Null_Factory::destroy_proxy_push_consumer_collection (TAO_EC_ProxyPushConsumer_Collection *x)
{
  delete x;
}

TAO_EC_ProxyPushSupplier_Collection*
TAO_EC_Null_Factory::create_proxy_push_supplier_collection (TAO_EC_Event_Channel_Base *)
{
  // This typedef is a workaround for a SunCC 4.2 bug
  typedef TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>::Iterator TAO_EC_Supplier_List_Iterator;
  return new TAO_ESF_Immediate_Changes<TAO_EC_ProxyPushSupplier,
      TAO_ESF_Proxy_List<TAO_EC_ProxyPushSupplier>,
      TAO_EC_Supplier_List_Iterator,
      ACE_Null_Mutex> ();
}

void
TAO_EC_Null_Factory::destroy_proxy_push_supplier_collection (TAO_EC_ProxyPushSupplier_Collection *x)
{
  delete x;
}

ACE_Lock*
TAO_EC_Null_Factory::create_consumer_lock (void)
{
  return new ACE_Lock_Adapter<ACE_Null_Mutex> ();
}

void
TAO_EC_Null_Factory::destroy_consumer_lock (ACE_Lock* x)
{
  delete x;
}

ACE_Lock*
TAO_EC_Null_Factory::create_supplier_lock (void)
{
  return new ACE_Lock_Adapter<ACE_Null_Mutex> ();
}

void
TAO_EC_Null_Factory::destroy_supplier_lock (ACE_Lock* x)
{
  delete x;
}

TAO_EC_ConsumerControl*
TAO_EC_Null_Factory::create_consumer_control (TAO_EC_Event_Channel_Base*)
{
  return new TAO_EC_ConsumerControl ();
}

void
TAO_EC_Null_Factory::destroy_consumer_control (TAO_EC_ConsumerControl* x)
{
  delete x;
}

TAO_EC_SupplierControl*
TAO_EC_Null_Factory::create_supplier_control (TAO_EC_Event_Channel_Base*)
{
  return new TAO_EC_SupplierControl ();
}

void
TAO_EC_Null_Factory::destroy_supplier_control (TAO_EC_SupplierControl* x)
{
  delete x;
}

TAO_END_VERSIONED_NAMESPACE_DECL
