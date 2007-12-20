#include "TM_Daemon.h"
#include "TM_DaemonC.h"
#include "utils/RT.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "DomainDataManager.h"
#include "Config_Handlers/XML_File_Intf.h"
#include "Config_Handlers/DnC_Dump.h"
#include "Client_Task.h"
#include "orbsvcs/CosNamingC.h"

#include <vector>
#include <string>

using namespace std;

namespace CIAO
{
  namespace TM_Daemon
  {
    const char *ior_output_file = "daemon.ior";
//    const char *deploymentplan_file = 0;
    char *name = 0;
    vector<string> plans;
    string aplan;
    const char *domain_file = 0;
    const char *mapping_file = 0;
    size_t threads = 2;


    int
    parse_args (int argc, char *argv[])
    {
      ACE_Get_Opt get_opts (argc, argv, "o:n:k:d:m:N:");
      int c;

      while ((c = get_opts ()) != -1)
        switch (c)
          {
            case 'o':
              ior_output_file = get_opts.opt_arg ();
              break;

            case 'N':
              name = get_opts.opt_arg ();
              ACE_DEBUG ((LM_DEBUG, "Naming service enabled!\nName is %s\n", name));

              break;

            case 'n':
              threads = (size_t) strtod (get_opts.opt_arg (), 0);
              break;

            case 'k':
//              deploymentplan_file = get_opts.opt_arg ();
//              ACE_DEBUG ((LM_DEBUG, "deploymentplan_file is %s.\n", deploymentplan_file));
	      aplan = get_opts.opt_arg ();
	      plans.push_back (aplan);
              break;

            case 'd':
              domain_file = get_opts.opt_arg ();
              break;

            case 'm':
              mapping_file = get_opts.opt_arg ();
              break;

            case '?':
            default:
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "usage:  %s "
                                 "-o <output ior filename>\n"
                                 "-k <DeploymentPlan filename>\n"
                                 "-d <Domain filename>\n"
                                 "-m <CORBALOC filename>\n"
                                 "-n <no of threads default = 2>"
                                 "-N <name ro register with the naming service>"
                                 "\n",
                                 argv [0]),
                                -1);
          }

      if (plans.size () != 0 && domain_file != 0 && mapping_file != 0)
        {
          // Indicates sucessful parsing of the command line
          return 0;
        }
      else
        {
          return -1;
        }

    }

    int
    run_main (int argc, char *argv[])
    {
      try
        {
          ACE_DEBUG ((LM_DEBUG, "Now running the TM daemon!\n"));

          CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
          if (parse_args (argc, argv) != 0)
            {
              return -1;
            }

          utils::set_priority ();

          CORBA::Object_var poa_object =
            orb->resolve_initial_references ("RootPOA");

          PortableServer::POA_var root_poa =
            PortableServer::POA::_narrow (poa_object.in ());
          root_poa->the_POAManager ()->activate ();

          // create the TM Daemon.
          Daemon_impl *daemon_impl = 0;
          ACE_NEW_RETURN (daemon_impl,
                          Daemon_impl (orb,
                   //                    deploymentplan_file,
				       plans,
                                       mapping_file,
                                       domain_file,
                                       root_poa,
                                       threads), 1);
          ACE_DEBUG ((LM_DEBUG, "done!\n"));

          PortableServer::ServantBase_var owner_transfer(daemon_impl);

          PortableServer::ObjectId_var id =
            root_poa->activate_object (daemon_impl);

          CORBA::Object_var object = root_poa->id_to_reference (id.in ());

          Daemon_var daemon = Daemon::_narrow (object.in ());

          CORBA::String_var ior = orb->object_to_string (daemon.in ());

          FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s\n",
                               ior_output_file),
                              1);
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);

          if (name)
            {
              ACE_DEBUG ((LM_DEBUG, "Now trying to bind to the naming service...."));
              CORBA::Object_var naming_context_object =
                orb->resolve_initial_references ("NameService");
              CosNaming::NamingContext_var naming_context =
                CosNaming::NamingContext::_narrow (naming_context_object.in ());
              CosNaming::Name cos_name;
              cos_name.length (1);
              cos_name[0].id = ::CORBA::string_dup (name);
              naming_context->rebind (cos_name, daemon.in ());
              ACE_DEBUG ((LM_DEBUG, "done! Bound witht the naming service under %s.\n", name));

            }
          daemon_impl->run ();

        }
      catch (const CORBA::Exception& ex)
        {
          ACE_PRINT_EXCEPTION (ex, "CIAO_TM_Daemon::main\n");
          return -1;
        }

      ACE_DEBUG ((LM_DEBUG, "CIAO_TM_Daemon has ended\n"));
      return 0;
    }
  }
}

int
main (int argc, char *argv[])
{
  return CIAO::TM_Daemon::run_main (argc, argv);
}
