//
// $Id$
//

#include "orbsvcs/Event/Dispatching_Modules.h"
#include "orbsvcs/Event/Task_Manager.h"
#include "orbsvcs/Event/Event_Channel.h"
#include "orbsvcs/Event/Module_Factory.h"

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
  return new ACE_ES_Priority_Dispatching(ec);
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

ACE_Task_Manager*
TAO_Default_Module_Factory::create_task_manager (ACE_EventChannel*)
{
  return new ACE_Task_Manager;
}

void
TAO_Default_Module_Factory::destroy_task_manager (ACE_Task_Manager* x)
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

ACE_ES_Priority_Timer*
TAO_Default_Module_Factory::create_timer_module (ACE_EventChannel* ec)
{
  return new ACE_ES_Priority_Timer (ec->task_manager ());
}

void
TAO_Default_Module_Factory::destroy_timer_module (ACE_ES_Priority_Timer* x)
{
  delete x;
}
