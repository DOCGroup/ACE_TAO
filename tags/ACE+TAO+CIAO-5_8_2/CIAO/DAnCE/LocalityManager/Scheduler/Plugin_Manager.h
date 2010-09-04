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
#include "LocalityManager/Scheduler/Dependency_Sorter.h"
#include "Deployment/Deployment_DeploymentPlanC.h"
#include "DAnCE/DAnCE_LocalityManagerC.h"
#include "DAnCE/DAnCE_DeploymentInterceptorsC.h"

#include <map>
#include <set>
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
    
    void set_orb (CORBA::ORB_ptr orb);
    
    CORBA::ORB_ptr get_orb (void);

    /// Set the configuration of the plugin_manager
    void set_configuration (const Deployment::Properties &config);
    
    typedef Dependency_Sorter::IH_DEPS IH_DEPS;
    
    /// Registers a new installation handler.
    char * register_installation_handler (const ACE_TCHAR *artifact,
                                          const ACE_TCHAR *entrypoint,
                                          const IH_DEPS &dependencies);

    /// Registers a new deployment interceptor
    void register_interceptor (const ACE_TCHAR *artifact,
                               const ACE_TCHAR *entrypoint);
    
    
    typedef Dependency_Sorter::Invalid_Install_Order Invalid_Install_Order;
    typedef Dependency_Sorter::INSTALL_ORDER INSTALL_ORDER;

    void get_installation_order (INSTALL_ORDER &);
    
    ::DAnCE::InstanceDeploymentHandler_ptr 
        fetch_installation_handler (const char *instance_type);
    
    typedef std::list < ::DAnCE::DeploymentInterceptor_var > 
      INTERCEPTORS;
    
    const INTERCEPTORS & fetch_interceptors (void);
    
    void register_configuration_plugin (const ACE_TCHAR *artifact,
                                        const ACE_TCHAR *entrypoint);
    
    ::DAnCE::LocalityConfiguration_ptr
        get_configuration_handler (const char *id);

  private:
    ::Deployment::Properties config_;

    typedef std::map < std::string,
                       ::DAnCE::InstanceDeploymentHandler_var > HANDLER_MAP;
    
    typedef std::map < std::string,
                       LocalityConfiguration_var > CONFIG_MAP;
    
    HANDLER_MAP handler_map_;
    
    INTERCEPTORS interceptors_;
    
    CONFIG_MAP config_plugins_;
    
    Dependency_Sorter ih_dep_;
    
    CORBA::ORB_var orb_;
  };
  
  typedef ACE_Singleton <Plugin_Manager,
                         TAO_SYNCH_MUTEX> PLUGIN_MANAGER;
}

DEPLOYMENT_SCHEDULER_SINGLETON_DECLARE (ACE_Singleton,
                                        DAnCE::Plugin_Manager,
                                        TAO_SYNCH_MUTEX)
#endif
 
