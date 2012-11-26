// $Id$
#include "dance/Deployment/Deployment_NodeManagerC.h"
#include "dance/Logger/Log_Macros.h"

int ACE_TMAIN (int argc, ACE_TCHAR * argv[])
{
  try
    {
      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      const char * ior = "file://node_manager.ior";
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
      Deployment::NodeApplicationManager_ptr nam_ptr =
        nm->preparePlan (plan, Deployment::ResourceCommitmentManager::_nil());
      ACE_DEBUG ((LM_DEBUG, "Plan prepared\n"));

      nm->destroyManager (nam_ptr);
      ACE_DEBUG ((LM_DEBUG, "NodeApplicationManager object destroyed\n"));
    }
  catch (const CORBA::Exception &)
    {
      ACE_ERROR ((LM_ERROR, "Uncaught CORBA exception\n"));
      return 1;
    }
  catch (...)
    {
      return 1;
    }

  return 0;
}
