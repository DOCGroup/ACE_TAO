/**
 * $Id$
 * @file Plan_Generator_Impl.h
 * @author Shanshan Jiang <shanshan.jiang@vanderbilt.edu>
 *
 * Contains the Plan_Generator_i class, which can be used to generate/modify
 * DeploymentPlans.
 */

#ifndef PLAN_GENERATOR_IMPL_H
#define PLAN_GENERATOR_IMPL_H

#include "DAnCE/Deployment/Deployment_RepositoryManagerC.h"
#include "Plan_Generator_Impl_Export.h"

namespace DAnCE
{
  namespace Plan_Generator
  {
    const size_t PACKAGE_NAME_LEN = 1024;

    /**
     * @class Plan_Generator_i
     * @brief This class generates or modifies deployment plans.
     */
    class Plan_Generator_Impl_Export Plan_Generator_i
    {
    public:
      Plan_Generator_i ();

      /**
       * @fn init
       * @brief Get the proper Repoman, using naming service or ior file.
       * @param orb
       * @param rm_use_naming Whether use naming service or not.
       * @param rm_name The name of the Repoman.
       * @returns a boolean denoting if this operation is successful or not.
       */
      bool init (CORBA::ORB_ptr orb,
                 bool rm_use_naming = 0,
                 const char *rm_name = 0);

      /**
       * @fn generate_plan
       * @brief Use the package name(s) or type(s) to get the PackageConfiguration
       *        provided by Repoman; and then use the PackageConfiguration to
       *        modify the location of all the artifacts in DeploymentPlan.
       * @param plan A DeploymentPlan that need to be updated.
       * @param package_uri The package name(s) or type(s).
       * @param use_package_name Use package name(s) or type(s).
       * @returns a boolean denoting if this operation is successful or not.
       */
      bool generate_plan (Deployment::DeploymentPlan &plan,
                          const char *package_uri,
                          bool use_package_name);

      /**
       * @fn generate_plan
       * @brief Use the specific type of a package to get the PackageConfiguration
       *        provided by Repoman; and then use the PackageConfiguration to
       *        generate the implementation and artifact fields in DeploymentPlan.
       * @param plan A DeploymentPlan that need to be updated.
       * @param package_uri The package specific type.
       * @param index The index of the newly generated implementation and artifact fields.
       * @returns a boolean denoting if this operation is successful or not.
       */
      bool generate_plan (Deployment::DeploymentPlan &plan,
                          const char *specific_type,
                          int &index);

    private:
      Deployment::RepositoryManager_var rm_;
    };

  }
}

#endif /* PLAN_GENERATOR_IMPL_H */
