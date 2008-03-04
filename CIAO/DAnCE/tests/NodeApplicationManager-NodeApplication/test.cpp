#include "DAnCE/Deployment/Deployment_NodeManagerC.h"
#include "DAnCE/Deployment/Deployment_NodeApplicationC.h"

int main (int argc, char * argv[])
{
  try
    {
      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      char * ior = "file://node_manager.ior";
      CORBA::Object_var obj = orb->string_to_object (ior);
      if (CORBA::is_nil (obj))
        {
          throw 0;
        }

      Deployment::NodeManager_var nm = Deployment::NodeManager::_narrow (obj);
      if (CORBA::is_nil (nm))
        {
          throw 0;
        }

      //get NodeApplicationManager
      Deployment::DeploymentPlan plan;
      Deployment::NodeApplicationManager_var nam;
      nam = nm->preparePlan (plan, Deployment::ResourceCommitmentManager::_nil());

      //call startLaunch
      Deployment::Properties dp;
      Deployment::Connections_var dc;
      Deployment::Application_ptr app = nam->startLaunch (dp, dc.out());
      Deployment::NodeApplication_var na = Deployment::NodeApplication::_narrow (app);
    }
  catch (const CORBA::Exception &e)
    {
      e._tao_print_exception ("DAnCE_NodeManager::main\t\n");
      ACE_ERROR((LM_ERROR, "[%M] Uncaught CORBA exception\n"));
      return 1;
    }
  catch (...)
    {
      return 1;
    }

  return 0;
}
