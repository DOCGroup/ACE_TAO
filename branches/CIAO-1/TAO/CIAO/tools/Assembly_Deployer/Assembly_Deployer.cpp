// $Id$

#include "CCM_DeploymentC.h"
#include "Assembly_ServiceC.h"
#include "Client_init.h"
#include "ace/Get_Opt.h"

const char *ior = "file://test.ior";
const char *cad = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "a:k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'a':
        cad = get_opts.opt_arg ();
        break;

      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s \n"
                           "-a <assembly descriptor (.cad)>\n"
                           "-k <Assembly_Manager ior> \n"
                           "\n",
                           argv [0]),
                          -1);
      }

  if (cad == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Must specify an assembly descriptor using -a flag.\n"),
                      -1);

  // Indicates sucessful parsing of the command line
  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CIAO::Client_init (orb.in ());

      CORBA::Object_var tmp =
        orb->string_to_object(ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CIAO::Assembly_Service_var as_svc
        = CIAO::Assembly_Service::_narrow (tmp.in ()
                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ::Components::Deployment::AssemblyFactory_var factory =
          as_svc->get_assemblyfactory (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (factory.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil ServerActivator reference <%s>\n",
                             ior),
                            1);
        }


      ACE_DEBUG ((LM_DEBUG, "Try creating an Assembly with %s\n", cad));

      Components::Cookie_var ck =
        factory->create_assembly (cad
                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Components::Deployment::Assembly_var assembly =
        factory->lookup (ck.in ()
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      assembly->build (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Press <enter> to tear down the assembly\n"));
      char dummy [256];
      cin.getline (dummy, 256);

      assembly->tear_down (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      factory->destroy (ck.in ()
                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      as_svc->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
