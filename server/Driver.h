#ifndef DRIVER_H_
#define DRIVER_H_

#include "DriverS.h"
#include "Input_Adapters/Interactive_Input_Adapter/Interactive_Input_AdapterC.h"
#include "ace/OS.h"
#include "ace/SString.h"
#include "orbsvcs/CosNamingC.h"
#include "common/OpStringC.h"
#include "Plan_Generator_Impl.h"

#include "Planner.h"
#include "LogScreenOut.h"

namespace CIAO
{
  namespace RACE
  {

    namespace SA_POP
    {

      class  Driver_i
        : public virtual POA_CIAO::RACE::SA_POP::Driver
      {
      public:
        // Constructor
        Driver_i (CORBA::ORB_ptr orb);

        // Destructor
        virtual ~Driver_i (void);

        virtual
        int init (const char *RM_name,
                  const char *sanet_str,
                  const char *taskmap_str);

        virtual
        void deploy_plan (const char * plan_uri)
          throw (::CORBA::SystemException);

        virtual
        void deploy_goal (const ::CIAO::RACE::GoalStructure & goal_idl)
          throw (::CORBA::SystemException);

      protected:
        virtual int opstring_to_dplan
        (::CIAO::RACE::OperationalString &op_string,
         ::Deployment::DeploymentPlan &plan);

        /// Convert SA-POP operational string to IDL operational string.
        /**
         * @param opstring  SA-POP operational string to convert.
         *
         * @param opstring_idl  Empty IDL operational string structure
         *                      to be populated.
         */
        virtual void opstring_to_idl (const ::SA_POP::Plan &opstring,
                                      OperationalString &opstring_idl);

        /// Convert IDL goal to SA-POP goal.
        /**
         * @param goal_idl  IDL goal to convert.
         *
         * @param goal  Empty SA-POP goal structure to be populated.
         */
        virtual void goal_from_idl (const GoalStructure &goal_idl,
                                    ::SA_POP::Goal &goal);

        virtual int deploy_plan (::Deployment::DeploymentPlan &plan);


      private:

        // Pointer to the ORB.
        CORBA::ORB_var orb_;

        // Naming context of the interactive input adapter.
        ACE_TString iia_name_;

        // Pointer to the interactive input adapter.
        ::CIAO::RACE::Interactive_Input_Adapter_var iia_;

        // Plan generator object.
        ::CIAO::Plan_Generator::Plan_Generator_i plan_gen_;

        // Pointer to SA-POP planner.
        ::SA_POP::Planner *planner_;

        // Screen output adapter for SA-POP.
        ::SA_POP::LogScreenOut screen_out_;
      };
    }
  }
}

#endif /* DRIVER_H_  */
