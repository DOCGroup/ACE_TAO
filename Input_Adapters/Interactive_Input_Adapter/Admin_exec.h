// $Id$
#ifndef CIAO_INTERACTIVE_INPUT_ADAPTER_ADMIN_EXEC_H
#define CIAO_INTERACTIVE_INPUT_ADAPTER_ADMIN_EXEC_H

#include "Interactive_Input_AdapterEC.h"
#include "Interactive_Input_Adapter_exec_export.h"
#include "tao/LocalObject.h"
#include "Logger.h"
#include "RACE/common/OpStringC.h"

namespace CIAO
{
  namespace RACE
  {
    namespace Input_Adapter
    {
      namespace CIDL_Interactive_IA_Component_Impl
      {
        class INTERACTIVE_INPUT_ADAPTER_EXEC_Export Admin_exec_i
          : public virtual ::CIAO::RACE::Input_Adapter::CCM_Admin,
            public virtual TAO_Local_RefCounted_Object
        {
          public:
          Admin_exec_i (CCM_Interactive_IA_Component_Context_ptr context,
                        Logger &logger);

          virtual ~Admin_exec_i (void);

          // Operations from ::CIAO::RACE::Input_Adapter::Admin

          virtual ::CORBA::Boolean
            add_plan (const char * filename,
                      ::CORBA::String_out ID);

          virtual ::CORBA::Boolean
            delete_plan (const char * ID);

          virtual ::CORBA::Boolean
            deploy_plan (const char * filename,
                         ::CORBA::String_out ID);

          virtual ::CORBA::Boolean
            deploy_plan_with_id (const char * ID);

          virtual ::CORBA::Boolean
            modify_plan (const char * filename,
                         const char * ID);

          virtual ::CORBA::Boolean
            tear_down_plan (const char * ID);

          virtual ::CORBA::Boolean
            start_controller ();

          virtual ::CORBA::Boolean
            stop_controller ();


          protected:

          // Helper methords.

          virtual void plan_to_opstring
            (const char *filename,
             ::CIAO::RACE::OperationalString & op_string);

          virtual void update_basic_info
            (const ::Deployment::DeploymentPlan * plan,
             ::CIAO::RACE::OperationalString & op_string);

          virtual void update_rate_info
            (const ::Deployment::DeploymentPlan * plan,
             ::CIAO::RACE::OperationalString & op_string);

          virtual void update_instance_info
            (const ::Deployment::DeploymentPlan * plan,
             ::CIAO::RACE::OperationalString & op_string);

          virtual int parse_properties
            (const char *name,
             const ::Deployment::Properties &prop);

          virtual const char * get_effector_ID
            (const ::Deployment::Properties &prop);

          virtual double parse_rate_info
            (const char* name,
             const ::Deployment::Properties &props);

          private:
          /// Object reference of the conductor object.
          Conductor::Admin_var conductor_;
          Logger &logger_;
        };
      }
    }
  }
}

#endif /* CIAO_INTERACTIVE_INPUT_ADAPTER_EXEC_H */
