// $Id$
#include "ace/OS.h"
#include "ace/Get_Opt.h"
#include "orbsvcs/CosNamingC.h"
#include "Config_Handlers/XML_File_Intf.h"
#include "ciao/DeploymentC.h"
#include "ciao/CIAO_common.h"
#include <iostream>

#include "ExecutionManager/ExecutionManagerC.h"

namespace CIAO
{
  namespace Plan_Launcher
  {
    // deployment plan URL
    const char* package_url = 0;
    bool use_naming = false;
    const char* ior_file = "file://em.ior";
    const char* dap_ior_filename = 0;
    const char* dap_ior = 0;
    enum mode_type { 
      pl_mode_start,
      pl_mode_stop,
      pl_mode_interactive
    };

    mode_type mode = pl_mode_interactive;

    static void
    usage (const ACE_TCHAR* program)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("[(%P|%t) Executor] Usage: %s\n")
                  ACE_TEXT ("-p <PACKAGE_URI>\n")
                  ACE_TEXT ("-k <EXECUTION_MANAGER_IOR>")
                  ACE_TEXT (" : Default file://em.ior\n")
                  ACE_TEXT ("-n : Use naming service to fetch")
                  ACE_TEXT (" Execution Manager IOR Alternative to -k\n")
                  ACE_TEXT ("-o <DOMAIN_APPLICATION_MANAGER_IOR_OUTPUT_FILE>\n")
                  ACE_TEXT ("-i <DOMAIN_APPLICATION_MANAGER_IOR_FOR_INPUT>\n")
                  ACE_TEXT ("-h : Show this usage information\n"),
                  program));
    }

    static bool
    parse_args (int argc,
                ACE_TCHAR *argv[])
    {
      ACE_Get_Opt get_opt (argc,
                           argv,
                           ACE_TEXT ("p:nk:o:i:h"));
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
            case 'o':
              dap_ior_filename = get_opt.opt_arg ();
              mode = pl_mode_start;
              break;
            case 'i':
              dap_ior = get_opt.opt_arg ();
              mode = pl_mode_stop;
              break;
            case 'h':
            default:
              usage(argv[0]);
              return false;
            }
        }

      if ((mode != pl_mode_stop) && (package_url == 0))
        {
          usage (argv[0]);
          return false;
        }
      
      return true;
    }

    static CORBA::Object_ptr
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

    static CORBA::Object_ptr
    fetch_reference_file (CORBA::ORB_ptr orb
                          ACE_ENV_ARG_DECL)
    {
      return orb->string_to_object (ior_file
                                    ACE_ENV_ARG_PARAMETER);
    }

    static ::Deployment::DomainApplicationManager_ptr
    init_dap_ior (::CIAO::ExecutionManagerDaemon_ptr exec_mgr 
                  ACE_ENV_ARG_DECL)
    {
      CIAO::Config_Handlers::XML_File_Intf intf (package_url);
      
      ::Deployment::DeploymentPlan_var plan =
          intf.get_plan ();

      return exec_mgr->preparePlan (plan, 1);
    }

    static ::Deployment::DomainApplicationManager_ptr
    read_dap_ior (CORBA::ORB_ptr orb
                  ACE_ENV_ARG_DECL)
    {
      CORBA::Object_var obj = orb->string_to_object (dap_ior
                                                     ACE_ENV_ARG_PARAMETER);
      return
        Deployment::DomainApplicationManager::_narrow (obj.in ()
                                                       ACE_ENV_ARG_PARAMETER);
    }

    static int
    write_dap_ior (CORBA::ORB_ptr orb,
                   ::Deployment::DomainApplicationManager_ptr dap
                   ACE_ENV_ARG_DECL)
    {
      CORBA::String_var ior = orb->object_to_string (dap);

      FILE* ior_output_file = ACE_OS::fopen (dap_ior_filename, "w");

      if (ior_output_file)
        {
          ACE_OS::fprintf (ior_output_file, "%s", ior.in ());
          ACE_OS::fclose (ior_output_file);

          return 0;
        }

      return -1;
    }

    static int
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

          if (parse_args (argc, argv) == false)
            return -1;

          CORBA::Object_var obj;

          // Initialize Execution Manager reference
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

          ::CIAO::ExecutionManagerDaemon_var exec_mgr =
              ::CIAO::ExecutionManagerDaemon::_narrow (
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
          
          // Initialize Domain Application Manager reference
          ::Deployment::DomainApplicationManager_var dapp_mgr;
          if (mode != pl_mode_stop)
            {
              dapp_mgr = init_dap_ior (exec_mgr.in ()
                                       ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
          else
            {
              dapp_mgr = read_dap_ior (orb.in ()
                                       ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
          
          if (CORBA::is_nil (dapp_mgr.in ()))
            {
              ACE_DEBUG ((LM_DEBUG,
                          "(%P|%t) CIAO_PlanLauncher:preparePlan call failed: "
                          "nil DomainApplicationManager reference\n"));
              return -1;
            }

          if (CIAO::debug_level () > 10)
            ACE_DEBUG ((LM_DEBUG,
                        "CIAO_PlanLauncher: Obtained DAM ref \n"));

          // == BALA check some bogus things here..
          // Create a dummy set of properties and start the
          // Launching of applications
          if (mode != pl_mode_stop)
            {
              ::Deployment::Properties_var properties;
              ACE_NEW_RETURN (properties,
                              Deployment::Properties,
                              1);
              if (CIAO::debug_level ())
                ACE_DEBUG ((LM_DEBUG,
                            "CIAO_PlanLauncher: start Launch application..."));

              // Don not start the Application immediately since it vialtes
              // the semantics of component activation sequence
              int start = 0;

              dapp_mgr->startLaunch (properties.in (), 0);
              ACE_DEBUG ((LM_DEBUG, "[success]\n"));

              // Call finish Launch to complete the connections
              ACE_DEBUG ((LM_DEBUG,
                          "CIAO_PlanLauncher: finish Launch application..."));
              dapp_mgr->finishLaunch (start);
              ACE_DEBUG ((LM_DEBUG, "[success]\n"));

              // Call start to activate components
              ACE_DEBUG ((LM_DEBUG,
                          "CIAO_PlanLauncher: start activating components..."));
              dapp_mgr->start ();
              ACE_DEBUG ((LM_DEBUG, "[success]\n"));

              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("CIAO_PlanLauncher: ")
                          ACE_TEXT ("Application Deployed successfully\n")));

              if (mode == pl_mode_start)
                {
                  write_dap_ior (orb.in (), dapp_mgr.in ()
                                 ACE_ENV_ARG_PARAMETER);
                }
              
              if (mode == pl_mode_interactive)
                {
                  ACE_DEBUG ((LM_DEBUG,
                             "Press <Enter> to tear down application\n"));
                  char dummy [256];
                  std::cin.getline (dummy, 256);
                }
            }

          if (mode != pl_mode_start)
          {
            // Tear down the assembly
            ACE_DEBUG ((LM_DEBUG,
                        "CIAO_PlanLauncher: destroy the application....."));
            dapp_mgr->destroyApplication ();

            ACE_DEBUG ((LM_DEBUG, "[success]\n"));

            ACE_DEBUG ((LM_DEBUG,
                        "CIAO_PlanLauncher: destroy the manager....."));

            exec_mgr->destroyManager (dapp_mgr.in ());
            ACE_DEBUG ((LM_DEBUG, "[success]\n"));
          }

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
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) Plan_Launcher: Unknown exception.\n"));
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
