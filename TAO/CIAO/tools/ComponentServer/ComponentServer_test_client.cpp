// $Id$

#include "CCM_DeploymentC.h"
#include "Client_init.h"
#include "ace/Get_Opt.h"

const char *ior = "file://test.ior";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s \n"
                           "-k <ComponentServer ior> \n"
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

      ::Components::Deployment::ComponentServer_var comserv =
          ::Components::Deployment::ComponentServer::_narrow(tmp.in ()
                                                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (comserv.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil ServerActivator reference <%s>\n",
                             ior),
                            1);
        }


      ACE_DEBUG ((LM_DEBUG, "Try creating a Container\n"));
      ::Components::ConfigValues config;
      Components::Deployment::Container_var container
        = comserv->create_container (config
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // now install a fake home:
      Components::ConfigValues com_config (2);
      com_config.length (2);

      Components::ConfigValue *item = new OBV_Components::ConfigValue ();

      item->name (CORBA::string_dup ("CIAO-servant-UUID"));
      item->value () <<= CORBA::string_dup ("DCE:93D254CF-9538-44e8-BB98-AABCD134ADD3");
      com_config[0] = item;

      item = new OBV_Components::ConfigValue ();
      item->name (CORBA::string_dup ("CIAO-servant-entrypt"));
      item->value () <<= CORBA::string_dup ("createRateGenHome_Servant");
      com_config[1] = item;

      //      ACE_OS::sleep (2);

      container->install_home ("DCE:82C2B032-37F0-4315-A59F-7020D3264E4D",
                               "createRateGenHome_Impl",
                               com_config
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Try removing a ComponentServer\n"));
      comserv->remove_container (container.in ()
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
