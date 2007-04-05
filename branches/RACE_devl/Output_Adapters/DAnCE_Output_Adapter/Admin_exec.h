#ifndef ADMIN_EXEC_H
#define ADMIN_EXEC_H

#include "DAnCE_Output_Adapter_svnt.h"
#include "DAnCE_Output_Adapter_exec_export.h"
#include "tao/LocalObject.h"

#include "Plan_Launcher/Plan_Launcher_Impl.h"
#include "Plan_Generator_Impl.h"

namespace CIAO
{
  namespace RACE
  {
    namespace DAnCE_OA
    {
      namespace CIDL_DAnCE_OA_Impl
      {
        class DANCE_OUTPUT_ADAPTER_EXEC_Export Admin_exec_i
          : public virtual ::CIAO::RACE::DAnCE_OA::CCM_Admin,
            public virtual TAO_Local_RefCounted_Object
        {
          public:
          Admin_exec_i (DAnCE_OA_Component_Context *context,
                        const char * repoman_id);

          virtual ~Admin_exec_i (void);

          virtual ::CORBA::Boolean deploy_plan
            (const ::Deployment::DeploymentPlan & plan);

          virtual ::CORBA::Boolean redeploy_plan
            (const ::Deployment::DeploymentPlan & plan);

          virtual ::CORBA::Boolean deploy_string
            (const ::CIAO::RACE::OperationalString & opstring);

          virtual ::CORBA::Boolean redeploy_string
            (const ::CIAO::RACE::OperationalString & opstring);

          virtual ::CORBA::Boolean tear_down_string
            (const char * ID);

          protected:

          /// Convert the operational string to a deployment_plan.
          virtual bool opstring_to_dplan (const ::CIAO::RACE::OperationalString &opstring,
                                          ::Deployment::DeploymentPlan &plan);

          private:

          /// Pointer to the component's context object.
          DAnCE_OA_Component_Context *context_;

          /// PlanLauncher object.
          Plan_Launcher::Plan_Launcher_i launcher_;

          /// ID of the repoman.
          CORBA::String_var repoman_id_;

          // CIAO Plan generator object.
          ::CIAO::Plan_Generator::Plan_Generator_i plan_gen_;

          /// Can the repoman be used?
          bool repoman_;


        };
      }
    }
  }
}

#endif /* CIAO_DANCE_OUTPUT_ADAPTER_EXEC_H */
