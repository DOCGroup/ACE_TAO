// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Notify/MonitorControlExt/MC_Notify_Service.h"

#include "tao/debug.h"

#include "orbsvcs/Notify/MonitorControl/MonitorManager.h"

#include "orbsvcs/Notify/MonitorControlExt/MC_Default_Factory.h"
#include "orbsvcs/Notify/MonitorControlExt/MonitorEventChannelFactory.h"

#if defined (TAO_HAS_MONITOR_FRAMEWORK) && (TAO_HAS_MONITOR_FRAMEWORK == 1)

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
        ORBSVCS_DEBUG ((LM_DEBUG, "%s must be loaded via "
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
        ORBSVCS_DEBUG ((LM_DEBUG, "%s must be loaded via "
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

ACE_STATIC_SVC_DEFINE (TAO_MC_Notify_Service,
                       ACE_TEXT (TAO_NOTIFY_MONITOR_CONTROL_EXTENSION),
                       ACE_Service_Type::SERVICE_OBJECT,
                       &ACE_SVC_NAME (TAO_MC_Notify_Service),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_Notify_MC_Ext, TAO_MC_Notify_Service)

#endif /* TAO_HAS_MONITOR_FRAMEWORK==1 */

