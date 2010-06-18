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
    const char * register_installation_handler (const ACE_TCHAR *artifact,
                                                const ACE_TCHAR *entrypoint);
    
    /// Registers a new deployment interceptor
    void register_interceptor (const ACE_TCHAR *artifact,
                               const ACE_TCHAR *entrypoint);

    ::DAnCE::InstanceDeploymentHandler_ptr 
        fetch_installation_handler (const char *instance_type);
    
    typedef std::list < ::DAnCE::InstanceInstallation_var > 
      INST_INTERCEPTORS;
    
    const INST_INTERCEPTORS & fetch_installation_interceptors (void);
      
    typedef std::list < ::DAnCE::InstanceConnection_var > 
      CONN_INTERCEPTORS;
    
    const CONN_INTERCEPTORS & fetch_connection_interceptors (void);

    typedef std::list < ::DAnCE::PlanPreprocessing_var > 
      PP_INTERCEPTORS;
    
    const PP_INTERCEPTORS & fetch_pp_interceptors (void);

  private:
    ::Deployment::Properties config_;

    typedef std::map < std::string,
                       ::DAnCE::InstanceDeploymentHandler_var > HANDLER_MAP;
    
    HANDLER_MAP handler_map_;

    INST_INTERCEPTORS inst_interceptors_;

    CONN_INTERCEPTORS conn_interceptors_;

    PP_INTERCEPTORS pp_interceptors_;
  };

  typedef ACE_Singleton <Plugin_Manager,
                         TAO_SYNCH_MUTEX> PLUGIN_MANAGER;
}

DEPLOYMENT_SCHEDULER_SINGLETON_DECLARE (ACE_Singleton,
                                        DAnCE::Plugin_Manager,
                                        TAO_SYNCH_MUTEX)
#endif
 
