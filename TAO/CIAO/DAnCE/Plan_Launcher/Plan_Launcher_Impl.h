/** 
 * $Id$
 * @file Plan_Launcher_Impl.h
 * @author Will Otte <wotte@dre.vanderbilt.edu>
 * 
 * Contains the Plan_Launcher_i class, which can be used by
 * applications to launch component assemblies.
 */
#ifndef PLAN_LAUNCHER_IMPL_H
#define PLAN_LAUNCHER_IMPL_H
#include "DeploymentS.h"
#include "CIAO_common.h"
#include "ExecutionManager/DAM_Map.h"
#include "ExecutionManager/ExecutionManagerC.h"
namespace CIAO
{
  namespace Plan_Launcher
  {
    
    /**
     * @class Plan_Launcher_i
     * @brief This class launches and manages deployment plans.
     */
    class Plan_Launcher_i
    {
    public:
      Plan_Launcher_i ();
      
      bool init (const char *em_ior, 
                 CORBA::ORB_ptr orb
                 ACE_ENV_ARG_DECL);

      /**
       * @fn launch_plan
       * @brief Launch a plan, given a deployment plan URI
       * @param plan_uri A uri that points ot a valid deployment plan
       * @returns a string containing the UUID of the plan. Null indicates failure.
       */
      const char * launch_plan (const char *plan_uri
                                ACE_ENV_ARG_DECL);
      
      /**
       * @fn launch_plan
       * @brief Launch a plan, given an IDL deployment plan
       * @param plan A valid IDL deployment plan
       * @returns a string containing the UUID of the plan. Null indicates failure.
       */
      const char * launch_plan (const ::Deployment::DeploymentPlan &plan
                                ACE_ENV_ARG_DECL);
      
      /// Returns the DAM associated with a given plan URI
      ::Deployment::DomainApplicationManager_ptr get_dam (const char *uuid
                                                          ACE_ENV_ARG_DECL);
      
      /// Tears down a plan given the UUID
      bool teardown_plan (const char *uuid ACE_ENV_ARG_DECL);
      
      bool teardown_plan (::Deployment::DomainApplicationManager_ptr dam
                          ACE_ENV_ARG_DECL);
      
      void destroy_dam (::Deployment::DomainApplicationManager_ptr dam
                        ACE_ENV_ARG_DECL);
      
    private:
      ::CIAO::ExecutionManagerDaemon_var em_;
      
      /// Local map for DAMs, to save expensive UUID lookups.
      Execution_Manager::DAM_Map map_;
      
    };
    
  }
}


#endif /* PLAN_LAUNCHER_IMPL_H */
