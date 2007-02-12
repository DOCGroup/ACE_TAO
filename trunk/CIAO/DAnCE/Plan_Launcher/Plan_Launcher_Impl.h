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

#include "Plan_Launcher_Impl_Export.h"
#include "ciao/CIAO_common.h"
#include "DAnCE/ExecutionManager/DAM_Map.h"
#include "DAnCE/Interfaces/ExecutionManagerDaemonC.h"
#include "DAnCE/Plan_Generator/Plan_Generator_Impl.h"

namespace CIAO
{
  namespace Plan_Launcher
  {
    /**
     * @class Plan_Launcher_i
     * @brief This class launches and manages deployment plans.
     */
    class Plan_Launcher_Impl_Export Plan_Launcher_i
    {
    public:
      class Deployment_Failure {
      public:
        Deployment_Failure (const ACE_CString  &error)
          : error_ (error)
        {
        }

        ACE_CString error_;
      };

      Plan_Launcher_i ();


      bool init (const char *em_ior,
                 CORBA::ORB_ptr orb,
                 bool use_repoman = false,
                 bool rm_use_naming = false,
                 const char *rm_name = 0);

      /**
       * @brief Launch a plan, given a deployment plan URI
       * @param deployment_plan_uri A uri that points ot a valid deployment plan
       * @returns a string containing the UUID of the plan. Null indicates failure.
       */
      const char * launch_plan (const char *deployment_plan_uri,
                                const char *package_uri = 0,
                                bool use_package_name = true,
                                bool use_repoman = false);

      const char * re_launch_plan (const char *deployment_plan_uri,
                                   const char *package_uri = 0,
                                   bool use_package_name = true,
                                   bool use_repoman = false);

      /**
       * @brief Launch a plan, given an IDL deployment plan
       * @param plan A valid IDL deployment plan
       * @returns a string containing the UUID of the plan. Null indicates failure.
       */
      const char * launch_plan (const ::Deployment::DeploymentPlan &plan);

      const char * re_launch_plan (const ::Deployment::DeploymentPlan &plan);

      /// Returns the DAM associated with a given plan URI
      ::Deployment::DomainApplicationManager_ptr get_dam (const char *uuid);

      /// Tears down a plan given the UUID
      bool teardown_plan (const char *uuid);

      bool teardown_plan (::Deployment::DomainApplicationManager_ptr dam);

      void destroy_dam (::Deployment::DomainApplicationManager_ptr dam);

      void destroy_dam_by_plan (const char * plan_uuid);

    private:
      ::CIAO::ExecutionManagerDaemon_var em_;

      /// Local map for DAMs, to save expensive UUID lookups.
      Execution_Manager::DAM_Map map_;

      CIAO::Plan_Generator::Plan_Generator_i pg_;
    };

  }
}


#endif /* PLAN_LAUNCHER_IMPL_H */
