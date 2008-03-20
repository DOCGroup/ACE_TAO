#include "DAnCE/Deployment/Deployment_NodeManagerC.h"

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

      Deployment::DeploymentPlan plan;
      Deployment::NodeApplicationManager_ptr nam_ptr = nm->preparePlan (plan, Deployment::ResourceCommitmentManager::_nil());
      DANCE_DEBUG((LM_DEBUG, "[%M] Plan prepared\n"));

      nm->destroyManager (nam_ptr);
      DANCE_DEBUG((LM_DEBUG, "[%M] NodeApplicationManager object destroyed\n"));
    }
  catch (const CORBA::Exception &)
    {
      DANCE_ERROR((LM_ERROR, "[%M] Uncaught CORBA exception\n"));
      return 1;
    }
  catch (...)
    {
      return 1;
    }

  return 0;
}
