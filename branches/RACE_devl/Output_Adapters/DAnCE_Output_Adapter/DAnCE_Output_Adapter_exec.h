#ifndef CIAO_DANCE_OUTPUT_ADAPTER_EXEC_H
#define CIAO_DANCE_OUTPUT_ADAPTER_EXEC_H

#include "DAnCE_Output_Adapter_svnt.h"
#include "DAnCE_Output_Adapter_exec_export.h"
#include "tao/LocalObject.h"

#include "Plan_Launcher/Plan_Launcher_Impl.h"

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
          Admin_exec_i (DAnCE_OA_Component_Context *context);

          virtual ~Admin_exec_i (void);

          virtual ::CORBA::Boolean deploy_plan
            (const ::Deployment::DeploymentPlan & plan);

          virtual ::CORBA::Boolean deploy_string
            (const ::CIAO::RACE::OperationalString & opstring);

          virtual ::CORBA::Boolean tear_down_string
            (const char * ID);

          private:

          /// Pointer to the component's context object.
          DAnCE_OA_Component_Context *context_;

          /// PlanLauncher object.
          Plan_Launcher::Plan_Launcher_i launcher_;
        };


        class DANCE_OUTPUT_ADAPTER_EXEC_Export DAnCE_OA_Component_exec_i
          : public virtual DAnCE_OA_Component_Exec,
            public virtual TAO_Local_RefCounted_Object
        {
          public:

          DAnCE_OA_Component_exec_i (void);

          virtual ~DAnCE_OA_Component_exec_i (void);

          virtual ::CIAO::RACE::DAnCE_OA::CCM_Admin_ptr get_admin ();

          virtual void set_session_context
            (::Components::SessionContext_ptr ctx);

          virtual void ciao_preactivate ();

          virtual void ciao_postactivate ();

          virtual void ccm_activate ();

          virtual void ccm_passivate ();

          virtual void ccm_remove ();

          protected:
          DAnCE_OA_Component_Context *context_;
        };

        class DANCE_OUTPUT_ADAPTER_EXEC_Export DAnCE_OA_Component_Home_exec_i
          : public virtual DAnCE_OA_Component_Home_Exec,
            public virtual TAO_Local_RefCounted_Object
        {
          public:
          DAnCE_OA_Component_Home_exec_i (void);

          virtual ~DAnCE_OA_Component_Home_exec_i (void);

          virtual ::Components::EnterpriseComponent_ptr create ();
        };

        extern "C" DANCE_OUTPUT_ADAPTER_EXEC_Export ::Components::HomeExecutorBase_ptr
        create_CIAO_RACE_DAnCE_OA_DAnCE_OA_Component_Home_Impl (void);
      }
    }
  }
}

#endif /* CIAO_DANCE_OUTPUT_ADAPTER_EXEC_H */
