/**
 * @file Plugin_Manager.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu
 *
 * $Id$
 * 
 * Singleton class to manage the lifecycle of deployment 
 * interceptors and plug-ins.
 */

#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include "ace/Singleton.h"

#include "LocalityManager/Scheduler/Deployment_Scheduler_export.h"
#include "Deployment/Deployment_DeploymentPlanC.h"
#include "DAnCE/DAnCE_LocalityManagerC.h"
#include "DAnCE/DAnCE_DeploymentInterceptorsC.h"

#include <map>
#include <list>

namespace DAnCE
{
  /**
   * @class Plugin_Manager
   *
   */
  class Deployment_Scheduler_Export Plugin_Manager
  {
  public:
    Plugin_Manager (void);
    
    ~Plugin_Manager (void);
    
    /// Set the configuration of the plugin_manager
    void set_configuration (const Deployment::Properties &config);

    /// Registers a new installation handler.
    char * register_installation_handler (const ACE_TCHAR *artifact,
                                          const ACE_TCHAR *entrypoint);
    
    /// Registers a new deployment interceptor
    void register_interceptor (const ACE_TCHAR *artifact,
                               const ACE_TCHAR *entrypoint);

    ::DAnCE::InstanceDeploymentHandler_ptr 
        fetch_installation_handler (const char *instance_type);
    
    typedef std::list < ::DAnCE::DeploymentInterceptor_var > 
      INTERCEPTORS;
    
    const INTERCEPTORS & fetch_interceptors (void);

  private:
    ::Deployment::Properties config_;

    typedef std::map < std::string,
                       ::DAnCE::InstanceDeploymentHandler_var > HANDLER_MAP;
    
    HANDLER_MAP handler_map_;

    INTERCEPTORS interceptors_;
  };

  typedef ACE_Singleton <Plugin_Manager,
                         TAO_SYNCH_MUTEX> PLUGIN_MANAGER;
}

DEPLOYMENT_SCHEDULER_SINGLETON_DECLARE (ACE_Singleton,
                                        DAnCE::Plugin_Manager,
                                        TAO_SYNCH_MUTEX)
#endif
 
