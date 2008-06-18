// $Id$
#include "orbsvcs/Notify/MonitorControlExt/MC_Notify_Service.h"
#include "orbsvcs/Notify/MonitorControlExt/MC_Default_Factory.h"
#include "orbsvcs/Notify/MonitorControlExt/MonitorEventChannelFactory.h"
#include "orbsvcs/Notify/MonitorControl/MonitorManager.h"
#include "tao/debug.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_MC_Notify_Service::~TAO_MC_Notify_Service (void)
{
}

TAO_Notify_Factory*
TAO_MC_Notify_Service::create_factory (void)
{
  TAO_MC_Default_Factory* factory = 0;
  ACE_NEW_THROW_EX (factory,
                    TAO_MC_Default_Factory (),
                    CORBA::NO_MEMORY ());
  return factory;
}

void
TAO_MC_Notify_Service::init_i (CORBA::ORB_ptr orb)
{
  this->TAO_CosNotify_Service::init_i (orb);

  TAO_MonitorManager* monitor =
    ACE_Dynamic_Service<TAO_MonitorManager>::instance (
      TAO_NOTIFY_MONITOR_CONTROL_MANAGER);
  if (monitor == 0)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "%s must be loaded via "
                              "the Service Configurator\n",
                              TAO_NOTIFY_MONITOR_CONTROL_MANAGER));
    }
  else
    {
      monitor->run ();
    }
}

void
TAO_MC_Notify_Service::init_i2 (CORBA::ORB_ptr orb,
                                CORBA::ORB_ptr dispatching_orb)
{
  this->TAO_CosNotify_Service::init_i2 (orb, dispatching_orb);

  TAO_MonitorManager* monitor =
    ACE_Dynamic_Service<TAO_MonitorManager>::instance (
      TAO_NOTIFY_MONITOR_CONTROL_MANAGER);
  if (monitor == 0)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "%s must be loaded via "
                              "the Service Configurator\n",
                              TAO_NOTIFY_MONITOR_CONTROL_MANAGER));
    }
  else
    {
      monitor->run ();
    }
}

void
TAO_MC_Notify_Service::finalize_service (
           CosNotifyChannelAdmin::EventChannelFactory_ptr factory)
{
  TAO_MonitorManager::shutdown ();
  this->TAO_CosNotify_Service::finalize_service (factory);
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_FACTORY_DEFINE (TAO_Notify_MC_Ext, TAO_MC_Notify_Service)
