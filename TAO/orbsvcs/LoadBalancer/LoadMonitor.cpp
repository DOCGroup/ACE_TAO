#include "orbsvcs/LoadBalancing/LB_CPU_Monitor.h"

#include "ace/Get_Opt.h"


ACE_RCSID (LoadBalancer,
           LoadMonitor,
           "$Id$")


static const char * monitor_ior_file = "monitor.ior";

void
usage (const ACE_TCHAR * cmd)
{
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("Usage:  %s ")
              ACE_TEXT ("-l <location_id> ")
              ACE_TEXT ("-k <location_kind> ")
              ACE_TEXT ("-m <CPU|Disk|Memory|Network> ")
              ACE_TEXT ("-o <ior_output_file> ")
              ACE_TEXT ("-h ")
              ACE_TEXT ("\n\n"),
              cmd));
}

void
parse_args (int argc,
            ACE_TCHAR *argv[]
            ACE_ENV_ARG_DECL)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("l:k:m:o:h"));

  int c = 0;

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'o':
          ::monitor_ior_file = get_opts.opt_arg ();
          break;

        case 'l':
          break;

        case 'k':
          break;

        case 'm':
          break;

        case 'h':
          ::usage (argv[0]);
          break;

        default:
          ::usage (argv[0]);
          ACE_THROW (CORBA::BAD_PARAM ());
        }
    }
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // The usual server side boilerplate code.

      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            ""
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Check the non-ORB arguments.
      ::parse_args (argc,
                    argv
                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // The "LoadManager" reference should have already been
      // registered with the ORB by its ORBInitializer.
      obj = orb->resolve_initial_references ("LoadManager"
                                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CosLoadBalancing::LoadManager_var load_manager =
        CosLoadBalancing::LoadManager::_narrow (obj.in ()
                                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var str =
        orb->object_to_string (load_manager.in ()
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      FILE * monitor_ior = ACE_OS::fopen (monitor_ior_file, "w");
      ACE_OS::fprintf (monitor_ior, "%s", str.in ());
      ACE_OS::fclose (monitor_ior);

      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "TAO Load Monitor");

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
