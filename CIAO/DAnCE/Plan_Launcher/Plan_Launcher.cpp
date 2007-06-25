// $Id$

#include "Plan_Launcher_Benchmark_Impl.h"

#include "ace/OS.h"
#include "ace/Get_Opt.h"
#include <iostream>

#include "DAnCE/Interfaces/ExecutionManagerDaemonC.h"

namespace CIAO
{
  namespace Plan_Launcher
  {
    // deployment plan URL
    const char* deployment_plan_url = 0;
    bool use_package_name = true;
    const char* package_names = 0;
    const char* package_types = 0;
    const char* new_deployment_plan_url = 0;
    const char* plan_uuid = 0;
    bool em_use_naming = false;
    const char* em_ior_file = "file://em.ior";
    bool rm_use_naming = false;
    bool use_repoman = false;
    const char* rm_ior_file = "file://rm.ior";
    const char* repoman_name_ = "RepositoryManager";
    const char* dap_ior_filename = 0;
    const char* dap_ior = 0;
    bool do_benchmarking = false;
    size_t niterations = 0;
    CORBA::Short priority = 0;

    enum mode_type {
      pl_mode_start,
      pl_mode_interactive,
      pl_mode_stop_by_dam,
      pl_mode_stop_by_uuid,
      pl_mode_redeployment
    };

    // default mode
    mode_type mode = pl_mode_interactive;

    static void
    usage (const ACE_TCHAR* program)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("[(%P|%t) Executor] Usage: %s\n")
                  ACE_TEXT ("-a <PACKAGE_NAMES>\n")
                  ACE_TEXT ("-b <BENCHMARKING_ITERATIONS>\n")
                  ACE_TEXT ("-e <PACKAGE_TYPES>\n")
                  ACE_TEXT ("-i <DOMAIN_APPLICATION_MANAGER_IOR_FOR_INPUT>: Tear down the application launched by this Domain Application Manager\n")
                  ACE_TEXT ("-k <EXECUTION_MANAGER_IOR>")
                  ACE_TEXT (" : Default file://em.ior\n")
                  ACE_TEXT ("-l <REPOSITORY_MANAGER_IOR>")
                  ACE_TEXT (" : Default file://rm.ior\n")
                  ACE_TEXT ("-n : Use naming service to fetch EM\n")
                  ACE_TEXT ("-o <DOMAIN_APPLICATION_MANAGER_IOR_OUTPUT_FILE>: Use this option to dump out the IOR of the Domain Application Manager\n")
                  ACE_TEXT ("-p <DEPLOYMENT_PLAN_URL>\n")
                  ACE_TEXT ("-r <NEW_PLAN_DESCRIPTOR_FOR_REDEPLOYMENT>\n")
                  ACE_TEXT ("-t <PLAN_UUID>\n")
                  ACE_TEXT ("-v <REPOSITORY_MANAGER_NAME>: Use naming service to fetch RM with the given name")
                  ACE_TEXT (" : Default RepositoryManager\n")
                  ACE_TEXT ("-z <DESIRED_CORBA_PRIORITY_FOR_EXECUTION_MANAGER>\n")
                  ACE_TEXT ("-h : Show this usage information\n"),
                  program));
    }

    static bool
    parse_args (int argc,
                ACE_TCHAR *argv[])
    {
      ACE_Get_Opt get_opt (argc,
                           argv,
                           ACE_TEXT ("a:b:e:p:nk:l:v:t:o:i:r:z:h"));
      int c;

      while ((c = get_opt ()) != EOF)
        {
          switch (c)
            {
              case 'a':
                package_names = get_opt.opt_arg ();
                use_package_name = true;
                break;
              case 'b':
                do_benchmarking = true;
                niterations = ACE_OS::atoi (get_opt.opt_arg ());
                break;
              case 'e':
                package_types = get_opt.opt_arg ();
                use_package_name = false;
                break;
              case 'p':
                deployment_plan_url = get_opt.opt_arg ();
                break;
              case 'n':
                em_use_naming = true;
                break;
              case 'k':
                em_ior_file = get_opt.opt_arg ();
                break;
              case 'l':
                use_repoman = true;
                rm_ior_file = get_opt.opt_arg ();
                break;
              case 'v':
                use_repoman = true;
                rm_use_naming = true;
                repoman_name_ = get_opt.opt_arg ();
                break;
              case 'o':
                dap_ior_filename = get_opt.opt_arg ();
                mode = pl_mode_start;
                break;
              case 'i':
                dap_ior = get_opt.opt_arg ();
                mode = pl_mode_stop_by_dam;
                break;
              case 't':
                plan_uuid = get_opt.opt_arg ();
                mode = pl_mode_stop_by_uuid;
                break;
              case 'r':
                new_deployment_plan_url = get_opt.opt_arg ();
                mode = pl_mode_redeployment;
                break;
              case 'z':
                priority = ACE_OS::atoi (get_opt.opt_arg ());
                break;
              case 'h':
              default:
                usage(argv[0]);
                return false;
            }
        }

      if ((mode != pl_mode_stop_by_dam) &&
          (mode != pl_mode_stop_by_uuid) &&
          (package_names == 0) &&
          (package_types == 0) &&
          (deployment_plan_url == 0) &&
          (new_deployment_plan_url == 0))
        {
          usage (argv[0]);
          return false;
        }

      return true;
    }

    static ::Deployment::DomainApplicationManager_ptr
    read_dap_ior (CORBA::ORB_ptr orb)
    {
      CORBA::Object_var obj = orb->string_to_object (dap_ior);
      return
        Deployment::DomainApplicationManager::_narrow (obj.in ());
    }

    static int
    write_dap_ior (CORBA::ORB_ptr orb,
                   ::Deployment::DomainApplicationManager_ptr dap)
    {
      CORBA::String_var ior = orb->object_to_string (dap);

      FILE* ior_output_file = ACE_OS::fopen (dap_ior_filename, "w");

      if (ior_output_file)
        {
          ACE_OS::fprintf (ior_output_file, "%s", ior.in ());
          ACE_OS::fclose (ior_output_file);

          return 0;
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Error in opening file %s to write DAM IOR: %m",
                      dap_ior_filename));
          return -1;
        }
    }

    static int
    run_main_implementation (int argc, char *argv[])
    {

      try
        {
          CORBA::ORB_var orb =
            CORBA::ORB_init (argc,
                             argv,
                             "");

          if (parse_args (argc, argv) == false)
            return -1;

          Plan_Launcher_i * launcher = 0;

          if (do_benchmarking)
            {
              ACE_NEW_RETURN (launcher,
                              Plan_Launcher_Benchmark_i,
                              -1);
            }
          else
            {
              ACE_NEW_RETURN (launcher,
                              Plan_Launcher_i,
                              -1);
            }

          if (!launcher->init (em_use_naming ? 0 : em_ior_file,
                               orb.in (),
                               use_repoman,
                               rm_use_naming,
                               rm_use_naming ? repoman_name_ : rm_ior_file,
                               priority,
                               niterations))
            {
              ACE_ERROR ((LM_ERROR, "(%P|%t) Plan_Launcher: Error initializing the EM.\n"));
              return -1;
            }

          ::Deployment::DomainApplicationManager_var dapp_mgr;

          if (mode == pl_mode_start || mode == pl_mode_interactive)  // initial deployment
            {
              CORBA::String_var uuid;

              if (package_names != 0)
                uuid = launcher->launch_plan (deployment_plan_url,
                                              package_names,
                                              use_package_name,
                                              use_repoman);
              else
                uuid = launcher->launch_plan (deployment_plan_url,
                                              package_types,
                                              use_package_name,
                                              use_repoman);

              if (uuid.in () == 0)
                {
                  ACE_ERROR ((LM_ERROR, "(%P|%t) Plan_Launcher: Error launching plan\n"));
                  return -1;
                }

              ACE_DEBUG ((LM_DEBUG, "Plan_Launcher returned UUID is %s\n",
                          uuid.in ()));
              dapp_mgr = launcher->get_dam (uuid.in ());

              // Write out DAM ior if requested
              if (mode == pl_mode_start)
                {
                  if (write_dap_ior (orb.in (), dapp_mgr.in ()) != 0)
                    return -1;
                }
              else // if (pl_mode_interactive)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "Press <Enter> to tear down application\n"));
                  char dummy [256];
                  std::cin.getline (dummy, 256);

                  // Tear down the assembly
                  ACE_DEBUG ((LM_DEBUG,
                              "Plan_Launcher: destroy the application.....\n"));
                  if (! launcher->teardown_plan (uuid))
                    ACE_DEBUG ((LM_DEBUG,
                                "(%P|%t) CIAO_PlanLauncher:tear down assembly failed: "
                                "unknown plan uuid.\n"));
                }
            }
          else if (mode == pl_mode_redeployment && new_deployment_plan_url != 0) // do redeployment
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Plan_Launcher: reconfigure application assembly.....\n"));

              CORBA::String_var uuid;

              if (package_names != 0)
                uuid = launcher->re_launch_plan (new_deployment_plan_url,
                                                 package_names,
                                                 use_package_name,
                                                 use_repoman);
              else
                uuid = launcher->re_launch_plan (new_deployment_plan_url,
                                                 package_types,
                                                 use_package_name,
                                                 use_repoman);

              if (uuid.in () == 0)
                {
                  ACE_ERROR ((LM_ERROR, "(%P|%t) Plan_Launcher: Error re-launching plan\n"));
                  return -1;
                }
            }
          else if (mode == pl_mode_stop_by_dam) // tear down by DAM
            {
              dapp_mgr = read_dap_ior (orb.in ());

              if (CORBA::is_nil (dapp_mgr.in ()))
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "(%P|%t) CIAO_PlanLauncher:tear down assembly failed: "
                              "nil DomainApplicationManager reference\n"));
                  return -1;
                }

              // Tear down the assembly
              ACE_DEBUG ((LM_DEBUG,
                          "Plan_Launcher: destroy the application.....\n"));
              launcher->teardown_plan (dapp_mgr.in ());
            }
          else if (mode == pl_mode_stop_by_uuid) // tear down by plan_uuid
            {
              // Tear down the assembly
              ACE_DEBUG ((LM_DEBUG,
                          "Plan_Launcher: destroy the application.....\n"));
              if (! launcher->teardown_plan (plan_uuid))
                {
                  ACE_ERROR ((LM_ERROR,
                              "(%P|%t) CIAO_PlanLauncher:tear down assembly failed: "
                              "unkown plan uuid.\n"));
                }
            }

          orb->destroy ();
        }
      catch (const Plan_Launcher_i::Deployment_Failure&)
        {
          ACE_ERROR ((LM_ERROR,
                      "Deployment failed. Plan_Launcher exiting.\n"));
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception ("(%P|%t) Plan_Launcher: ");

          return -1;
        }
      catch (...)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) Plan_Launcher: Unknown exception.\n"));
          return -1;
        }

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
