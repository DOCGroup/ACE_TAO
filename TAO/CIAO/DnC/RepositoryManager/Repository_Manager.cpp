// $Id$

#include "RepositoryManager_Impl.h"
#include "DeploymentC.h"
#include "ace/OS_NS_stdio.h"
#include "ace/streams.h"

const char * exec_ior = "file://exec_mgr.ior";

static void
usage (const ACE_TCHAR* program)
{
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("Usage: %s -u <URI>\n")
              ACE_TEXT (" <URI>: URI identifying the package\n"),
              program));
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_TCHAR* package_url = 0;
  ACE_TCHAR* plan_url = 0;
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("p:d:k:"));
  int c;

  while ((c = get_opt ()) != EOF)
    {
      switch (c)
        {
          case 'p':
            package_url = get_opt.opt_arg ();
            break;
          case 'd':
            plan_url = get_opt.opt_arg ();
            break;
          case 'k':
            exec_ior = get_opt.opt_arg ();
            break;
          default:
            usage(argv[0]);
            return -1;
        }
    }

  if (package_url == 0) {
    usage(argv[0]);
    return -1;
  }

  if (plan_url == 0) {
    usage(argv[0]);
    return -1;
  }

  // Initialize the ORB so that CORBA::Any will work
  //
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "");

  try
    {
      DOMDocument* doc = CIAO::Config_Handler::Utils::
                         create_document (plan_url);
      CIAO::Config_Handler::Plan_Handler plan_handler (doc,
                                                DOMNodeFilter::SHOW_ELEMENT |
                                                DOMNodeFilter::SHOW_TEXT);
      Deployment::DeploymentPlan plan;
      plan_handler.process_plan (plan);
      Deployment::DnC_Dump::dump (plan);
      Deployment::PackageConfiguration* pc;
      CIAO::RepositoryManager_Impl rep_impl;
      rep_impl.installPackage ("PC", package_url);
      pc = rep_impl.findPackageByName ("PC");
      Deployment::DnC_Dump::dump (*pc);

      /**********************************************************
       Pass the parsed plan to the Execution Manager to start the
       Deployment Process.
      ************************************************************/
      CORBA::Object_var obj = orb->string_to_object (exec_ior
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Deployment::ExecutionManager_var exec_mgr =
        Deployment::ExecutionManager::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Deployment::DomainApplicationManager_var dapp_mgr =
        exec_mgr->preparePlan (plan, 1);

      if (CORBA::is_nil (dapp_mgr.in ()))
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Executor:preparePlan call failed:nil DomainApplicationManager reference\n"));
          return 1;
        }

      // Create a dummy set of properties and start the
      // Launching of applications
      Deployment::Properties_var properties;
      ACE_NEW_RETURN (properties,
                      Deployment::Properties,
                      1);
      // Start the Application immediately
      int start = 1;
      dapp_mgr->startLaunch (properties.in (), start);

      // Call finish Launch to complete the connections
      dapp_mgr->finishLaunch (start);

      ACE_DEBUG ((LM_DEBUG, "Executor: Application Deployed successfully \n"));
      ACE_DEBUG ((LM_DEBUG, "Press <Enter> to tear down application \n"));

      char dummy [256];
      std::cin.getline (dummy, 256);

      // Tear down the assembly
      exec_mgr->destroyManager (dapp_mgr.in ());

    }
  catch (CORBA::Exception& ex)
    {
      ACE_PRINT_EXCEPTION (ex, "Caught CORBA Exception: ");
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Caught unknown exception\n"));
      return 1;
    }

  return 0;
}
