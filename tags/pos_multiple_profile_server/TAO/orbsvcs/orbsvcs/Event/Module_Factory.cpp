// $Id$

#include "orbsvcs/Event/Dispatching_Modules.h"
#include "orbsvcs/Event/Timer_Module.h"
#include "orbsvcs/Event/Event_Channel.h"
#include "orbsvcs/Event/Module_Factory.h"

ACE_RCSID(Event, Module_Factory, "$Id$")

TAO_Module_Factory::~TAO_Module_Factory (void)
{
}

// ****************************************************************

TAO_Default_Module_Factory::TAO_Default_Module_Factory (void)
{
}

TAO_Default_Module_Factory::~TAO_Default_Module_Factory (void)
{
}

ACE_ES_Dispatching_Base*
TAO_Default_Module_Factory::create_dispatching_module (ACE_EventChannel* ec)
{
  return new ACE_ES_Priority_Dispatching (ec);
}

void
TAO_Default_Module_Factory::destroy_dispatching_module (ACE_ES_Dispatching_Base* x)
{
  delete x;
}

ACE_ES_Consumer_Module*
TAO_Default_Module_Factory::create_consumer_module (ACE_EventChannel* ec)
{
  return new ACE_ES_Consumer_Module (ec);
}

void
TAO_Default_Module_Factory::destroy_consumer_module (ACE_ES_Consumer_Module* x)
{
  delete x;
}

ACE_ES_Correlation_Module*
TAO_Default_Module_Factory::create_correlation_module (ACE_EventChannel* ec)
{
  return new ACE_ES_Correlation_Module (ec);
}

void
TAO_Default_Module_Factory::destroy_correlation_module (ACE_ES_Correlation_Module* x)
{
  delete x;
}

ACE_ES_Subscription_Module*
TAO_Default_Module_Factory::create_subscription_module (ACE_EventChannel* ec)
{
  return new ACE_ES_Subscription_Module (ec);
}

void
TAO_Default_Module_Factory::destroy_subscription_module (ACE_ES_Subscription_Module* x)
{
  delete x;
}

ACE_ES_Supplier_Module*
TAO_Default_Module_Factory::create_supplier_module (ACE_EventChannel* ec)
{
  return new ACE_ES_Supplier_Module (ec);
}

void
TAO_Default_Module_Factory::destroy_supplier_module (ACE_ES_Supplier_Module* x)
{
  delete x;
}

TAO_EC_Timer_Module*
TAO_Default_Module_Factory::create_timer_module (ACE_EventChannel* ec)
{
  RtecScheduler::Scheduler_var scheduler = ec->scheduler ();
  return new TAO_EC_RPT_Timer_Module (scheduler.in ());
}

void
TAO_Default_Module_Factory::destroy_timer_module (TAO_EC_Timer_Module* x)
{
  delete x;
}

// ****************************************************************

TAO_Reactive_Module_Factory::TAO_Reactive_Module_Factory (void)
{
}

TAO_Reactive_Module_Factory::~TAO_Reactive_Module_Factory (void)
{
}

ACE_ES_Dispatching_Base*
TAO_Reactive_Module_Factory::create_dispatching_module (ACE_EventChannel* ec)
{
  return new ACE_ES_EFD_Dispatching(ec);
}

void
TAO_Reactive_Module_Factory::destroy_dispatching_module (ACE_ES_Dispatching_Base* x)
{
  delete x;
}

ACE_ES_Consumer_Module*
TAO_Reactive_Module_Factory::create_consumer_module (ACE_EventChannel* ec)
{
  return new ACE_ES_Consumer_Module (ec);
}

void
TAO_Reactive_Module_Factory::destroy_consumer_module (ACE_ES_Consumer_Module* x)
{
  delete x;
}

ACE_ES_Correlation_Module*
TAO_Reactive_Module_Factory::create_correlation_module (ACE_EventChannel* ec)
{
  return new ACE_ES_Correlation_Module (ec);
}

void
TAO_Reactive_Module_Factory::destroy_correlation_module (ACE_ES_Correlation_Module* x)
{
  delete x;
}

ACE_ES_Subscription_Module*
TAO_Reactive_Module_Factory::create_subscription_module (ACE_EventChannel* ec)
{
  return new ACE_ES_Subscription_Module (ec);
}

void
TAO_Reactive_Module_Factory::destroy_subscription_module (ACE_ES_Subscription_Module* x)
{
  delete x;
}

ACE_ES_Supplier_Module*
TAO_Reactive_Module_Factory::create_supplier_module (ACE_EventChannel* ec)
{
  return new ACE_ES_Supplier_Module (ec);
}

void
TAO_Reactive_Module_Factory::destroy_supplier_module (ACE_ES_Supplier_Module* x)
{
  delete x;
}

TAO_EC_Timer_Module*
TAO_Reactive_Module_Factory::create_timer_module (ACE_EventChannel* ec)
{
  ACE_UNUSED_ARG (ec);

  return new TAO_EC_ST_Timer_Module (TAO_ORB_Core_instance ()->reactor ());
}

void
TAO_Reactive_Module_Factory::destroy_timer_module (TAO_EC_Timer_Module* x)
{
  delete x;
}
