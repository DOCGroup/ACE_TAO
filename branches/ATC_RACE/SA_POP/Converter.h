#include "Interactive_Input_AdapterC.h"
#include "ace/OS.h"
#include "ace/SString.h"
#include "orbsvcs/CosNamingC.h"
#include "RACE/common/OpStringC.h"
#include "Plan_Generator_Impl.h"
namespace CIAO
{
  namespace RACE
  {

    class Converter
    {
    public:
      Converter (int argc, char **argv);

      virtual ~Converter ();

      virtual int convert (OperationalString &op_string,
                           ::Deployment::DeploymentPlan &plan);

      virtual int deploy_plan (::Deployment::DeploymentPlan &plan);

    private:
      ::CIAO::Plan_Generator::Plan_Generator_i plan_gen_;

      ACE_CString iia_name_;

      CORBA::ORB_var orb_;

      CIAO::RACE::Interactive_Input_Adapter_var iia_;

      bool history_;

      ::Deployment::DeploymentPlan previous_;
    };

  }
}
