#ifndef DRIVER_H_
#define DRIVER_H_

#include "DriverS.h"
#include "Input_Adapters/Interactive_Input_Adapter/Interactive_Input_AdapterC.h"
#include "ace/OS.h"
#include "ace/SString.h"
#include "orbsvcs/CosNamingC.h"
#include "RACE/common/OpStringC.h"
#include "Plan_Generator_Impl.h"
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
        int init (const char *RM_name);

        virtual
        void deploy_plan (const char * plan_uri)
          throw (::CORBA::SystemException);

        virtual
        void deploy_goal (const ::CIAO::RACE::GoalStructure & goal)
          throw (::CORBA::SystemException);

      protected:
        virtual int convert (::CIAO::RACE::OperationalString &op_string,
                             ::Deployment::DeploymentPlan &plan);

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
      };
    }
  }
}

#endif /* DRIVER_H_  */
