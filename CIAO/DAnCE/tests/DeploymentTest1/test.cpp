#include "ace/Auto_Ptr.h"
#include "DAnCE/Deployment/Deployment_DeploymentPlanC.h"
#include "DAnCE/Deployment/Deployment_ExecutionManagerC.h"
#include "DAnCE/Deployment/Deployment_DomainApplicationManagerC.h"
#include "DAnCE/Deployment/Deployment_DomainApplicationC.h"
#include "DAnCE/Deployment/Deployment_NodeManagerC.h"
#include "DAnCE/Deployment/Deployment_NodeApplicationManagerC.h"
#include "tools/Config_Handlers/DnC_Dump.h"
#include "tools/Config_Handlers/XML_File_Intf.h"


int main (int argc, char * argv[])
{
  try
    {
      CIAO::Config_Handlers::XML_File_Intf file (argv[1]);
      file.add_search_path ("CIAO_ROOT", "/docs/schema/");
      file.add_search_path ("TAO_ROOT", "/docs/schema/");
      auto_ptr<Deployment::DeploymentPlan> pplan (file.release_plan());

      ACE_DEBUG((LM_DEBUG, "[%M] Test starts.\n"));
      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      char * ior = "file://execution_manager.ior";

      CORBA::Object_var obj = orb->string_to_object (ior);
      if (CORBA::is_nil (obj))
        {
          throw 0;
        }

      Deployment::ExecutionManager_var em = Deployment::ExecutionManager::_narrow (obj);
      if (CORBA::is_nil (em))
        {
          throw 0;
        }
      ACE_DEBUG((LM_DEBUG, "[%M] preparePlan\n"));
      Deployment::DomainApplicationManager_var dam = em->preparePlan (*pplan, Deployment::ResourceCommitmentManager::_nil());
      ACE_DEBUG((LM_DEBUG, "[%M] startLaunch\n"));
      //call startLaunch
      Deployment::Properties dp;
      Deployment::Connections_var dc;
      Deployment::Application_var na = dam->startLaunch (dp, dc.out());
      Deployment::DomainApplication_var da = Deployment::DomainApplication::_narrow (na);
      if (CORBA::is_nil (da))
        {
          ACE_DEBUG((LM_DEBUG, "[%M] narrow fails\n"));
        }
      ACE_DEBUG((LM_DEBUG, "[%M] finishLaunch\n"));
      da->finishLaunch (dc.in(), true);
      ACE_DEBUG((LM_DEBUG, "[%M] Test succeded!\n"));

    }
  catch (const CORBA::Exception &e)
    {
      e._tao_print_exception ("DeploymentTest1::main\t\n");
      ACE_ERROR((LM_ERROR, "[%M] Uncaught CORBA exception\n"));
      return 1;
    }
  catch (...)
    {
      return 1;
    }

  return 0;
}
