// $Id$
// $Id$

#include "CCM_DeploymentC.h"
#include "Client_init.h"
#include "ace/Get_Opt.h"

const char *ior = "file://test.ior";
int test_container = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "ck:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'c':
        test_container = 1;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s\n"
                           "-c Test Creating containers on ComponentServer\n"
                           "-k <ior>\n"
                           "\n",
                           argv [0]),
                          -1);
      }
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

      ::Components::Deployment::ServerActivator_var activator =
          ::Components::Deployment::ServerActivator::_narrow(tmp.in ()
                                                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (activator.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil ServerActivator reference <%s>\n",
                             ior),
                            1);
        }

      ACE_DEBUG ((LM_DEBUG, "Try creating a ComponentServer\n"));
      ::Components::ConfigValues config;

      ::Components::Deployment::ComponentServer_var comserv
          = activator->create_component_server (config
                                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Components::Deployment::Container_var container;

      if (test_container)
        {
          ACE_DEBUG ((LM_DEBUG, "Try creating a Container\n"));

          container
            = comserv->create_container (config
                                         ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      ACE_OS::sleep (2);

      if (test_container)
        {
          ACE_DEBUG ((LM_DEBUG, "Try removing a ComponentServer\n"));
          comserv->remove_container (container
                                     ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      ACE_DEBUG ((LM_DEBUG, "Try removing a ComponentServer\n"));
      activator->remove_component_server (comserv.in ()
                                          ACE_ENV_ARG_PARAMETER);
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
