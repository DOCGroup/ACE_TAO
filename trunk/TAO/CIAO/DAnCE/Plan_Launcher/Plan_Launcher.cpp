// $Id$
#include "ace/Get_Opt.h"
#include "orbsvcs/CosNamingC.h"
#include "Config_Handlers/XML_File_Intf.h"
#include "ciao/DeploymentC.h"
#include "ciao/CIAO_common.h"
#include <iostream>

namespace CIAO
{
  namespace Plan_Launcher
  {
    // deployment plan URL
    const char* package_url = 0;
    bool use_naming = false;
    const char* ior_file = "file://em.ior";

    void
    usage (const ACE_TCHAR* program)
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) Executor: Usage: %s -p <URI> -k <EX_IOR> "
                  " (or -n <Use NameService>) \n",
                  program));
    }

    bool
    parse_args (int argc,
                ACE_TCHAR *argv[])
    {
      ACE_Get_Opt get_opt (argc,
                           argv,
                           ACE_TEXT ("p:nk:"));
      int c;

      while ((c = get_opt ()) != EOF)
        {
          switch (c)
            {
            case 'p':
              package_url = get_opt.opt_arg ();
              break;
            case 'n':
              use_naming = true;
              break;
            case 'k':
              ior_file = get_opt.opt_arg ();
              break;
            default:
              usage(argv[0]);
              return false;
            }
        }

      return true;
    }

    CORBA::Object_ptr
    fetch_reference_naming (CORBA::ORB_ptr orb
                            ACE_ENV_ARG_DECL)
    {
      CORBA::Object_var tmp =
        orb->resolve_initial_references ("NameService"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CosNaming::NamingContext_var pns =
        CosNaming::NamingContext::_narrow (tmp.in ()
                                           ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CosNaming::Name name (2);
      name.length (2);
      name[0].id =
        CORBA::string_dup ("CIAO");
      name[1].id =
        CORBA::string_dup ("ExecutionManager");

      return pns->resolve (name
                           ACE_ENV_ARG_PARAMETER);
    }

    CORBA::Object_ptr
    fetch_reference_file (CORBA::ORB_ptr orb
                          ACE_ENV_ARG_DECL)
    {
      return orb->string_to_object (ior_file
                                    ACE_ENV_ARG_PARAMETER);
    }

    int
    run_main_implementation (int argc, char *argv[])
    {
      ACE_DECLARE_NEW_CORBA_ENV;

      ACE_TRY
        {
          CORBA::ORB_var orb =
            CORBA::ORB_init (argc,
                             argv,
                             ""
                             ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (!parse_args (argc, argv) || package_url == 0)
            {
              usage (argv[0]);
              return -1;
            }

          CORBA::Object_var obj;

          if (use_naming)
            {
              obj =
                fetch_reference_naming (orb.in ()
                                        ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
          else
            {
              obj =
                fetch_reference_file (orb.in ()
                                      ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }

          ::Deployment::ExecutionManager_var exec_mgr =
              ::Deployment::ExecutionManager::_narrow (
                obj.in ()
                ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (CORBA::is_nil (exec_mgr.in ()))
            {
              ACE_ERROR ((LM_ERROR,
                          "(%P|%t) CIAO_PlanLauncher: nil Execution"
                          " Manager reference, narrow failed\n"));
              return -1;
            }

          if (CIAO::debug_level () > 10)
            ACE_DEBUG ((LM_DEBUG,
                        "(%P|%t) CIAO_PlanLauncher: Obtained Execution"
                        " Manager ref \n"));

          CIAO::Config_Handlers::XML_File_Intf intf (package_url);

          ::Deployment::DeploymentPlan_var plan =
              intf.get_plan ();

          ::Deployment::DomainApplicationManager_var dapp_mgr =
              exec_mgr->preparePlan (plan,
                                     1);

          if (CORBA::is_nil (dapp_mgr.in ()))
            {
              ACE_DEBUG ((LM_DEBUG,
                          "(%P|%t) CIAO_PlanLauncher:preparePlan call failed:nil "
                          "DomainApplicationManager reference\n"));
              return -1;
            }

          if (CIAO::debug_level () > 10)
            ACE_DEBUG ((LM_DEBUG,
                        "CIAO_PlanLauncher: Obtained DAM ref \n"));

          // == BALA check some bogus things here..
          // Create a dummy set of properties and start the
          // Launching of applications
          ::Deployment::Properties_var properties;
          ACE_NEW_RETURN (properties,
                          Deployment::Properties,
                          1);
          if (CIAO::debug_level ())
            ACE_DEBUG ((LM_DEBUG,
                        "CIAO_PlanLauncher: start Launch application....."));

          // Start the Application immediately
          int start = 1;

          dapp_mgr->startLaunch (properties.in (), start);
          ACE_DEBUG ((LM_DEBUG, "[success]\n"));

          ACE_DEBUG ((LM_DEBUG, "CIAO_PlanLauncher: finish Launch application....."));
          // Call finish Launch to complete the connections
          dapp_mgr->finishLaunch (start);
          ACE_DEBUG ((LM_DEBUG, "[success]\n"));

          ACE_DEBUG ((LM_DEBUG, "CIAO_PlanLauncher: Application Deployed successfully \n"));
          ACE_DEBUG ((LM_DEBUG, "Press <Enter> to tear down application \n"));

          char dummy [256];
          std::cin.getline (dummy, 256);
          // Tear down the assembly

          ACE_DEBUG ((LM_DEBUG,
                      "CIAO_PlanLauncher: destroy the application....."));
          dapp_mgr->destroyApplication ();

          ACE_DEBUG ((LM_DEBUG, "[success]\n"));

          ACE_DEBUG ((LM_DEBUG,
                      "CIAO_PlanLauncher: destroy the manager....."));

          exec_mgr->destroyManager (dapp_mgr.in ());
          ACE_DEBUG ((LM_DEBUG, "[success]\n"));

          orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               "(%P|%t) Plan_Launcher: ");

          return -1;
        }
      ACE_CATCHALL
        {
          return -1;
        }
      ACE_ENDTRY;
      ACE_CHECK_RETURN (-1);

      return 0;
    }
  }
}

using namespace CIAO::Plan_Launcher;

int
ACE_TMAIN (int argc,
           ACE_TCHAR *argv[])
{
  return run_main_implementation (argc, argv);
}
