/**
 * $Id$
 * @file Plan_Launcher_Base_Impl_T.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 * Contains the generic implementation of plan launcher logic for both EM and NM deployments.
 * 
 */

#ifndef PLAN_LAUNCHER_BASE_IMPL_H
#define PLAN_LAUNCHER_BASE_IMPL_H

#include "DAnCE/Deployment/Deployment_ConnectionC.h"
#include "DAnCE/Deployment/Deployment_DeploymentPlanC.h"

#include "Plan_Launcher_Base.h"

namespace DAnCE
{
  /**
   * @class Plan_Launcher_Base_Impl
   * @brief This class launches and manages deployment plans.
   */
  template<typename Manager, typename AppManager, typename Application>
  class Plan_Launcher_Base_Impl : public virtual Plan_Launcher_Base
  {
  protected:
    
    typedef typename Manager::_ptr_type Manager_ptr;
    typedef typename Manager::_var_type Manager_var;

    typedef typename AppManager::_ptr_type AppManager_ptr;
    typedef typename AppManager::_var_type AppManager_var;

    typedef typename Application::_ptr_type Application_ptr;
    typedef typename Application::_var_type Application_var;
  
  public:
    Plan_Launcher_Base_Impl (CORBA::ORB_ptr orb,
                             Manager_ptr manager);

    virtual ~Plan_Launcher_Base_Impl ();

    /**
     * @brief Launch a plan, given an IDL deployment plan
     * @param plan A valid IDL deployment plan
     * @returns a string containing the UUID of the plan. Null indicates failure.
     */
    virtual const char * launch_plan (const ::Deployment::DeploymentPlan &plan,
                                      CORBA::Object_out am,
                                      CORBA::Object_out app);
  
    virtual CORBA::Object_ptr prepare_plan (const ::Deployment::DeploymentPlan &plan);
  
    virtual CORBA::Object_ptr start_launch (CORBA::Object_ptr app_mgr,
                                            const ::Deployment::Properties &properties,
                                            ::Deployment::Connections_out connections);
  
    virtual void finish_launch (CORBA::Object_ptr app,
                                const ::Deployment::Connections &provided_connections,
                                bool start);
  
    virtual void start (CORBA::Object_ptr application);
  
    /// Tears down a plan given the UUID
    virtual void teardown_application (CORBA::Object_ptr app_mgr,
                                       CORBA::Object_ptr app);

    virtual void destroy_app_manager (CORBA::Object_ptr);

    virtual void create_external_connections(const ::Deployment::DeploymentPlan &plan,
                                             Deployment::Connections &conn);

    //  static ACE_CString expand_env_vars(const ACE_TCHAR * src);
  protected:
    /// Cached ORB pointer
    CORBA::ORB_var orb_;
  
    /// Object reference of the Manager
    Manager_var manager_;
  }; // class Plan_Launcher_Impl

}
#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Plan_Launcher/Plan_Launcher_Base_Impl_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Plan_Launcher_Base_Impl_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* PLAN_LAUNCHER_BASE_IMPL_H */
