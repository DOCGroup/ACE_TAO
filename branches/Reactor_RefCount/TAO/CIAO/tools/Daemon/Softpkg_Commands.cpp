// $Id$

#include "Softpkg_Commands.h"
#include "ace/Get_Opt.h"
#include "CCM_DeploymentC.h"
#include "../XML_Helpers/XML_Utils.h"

#if !defined (__ACE_INLINE__)
# include "Softpkg_Commands.inl"
#endif /* __ACE_INLINE__ */

//=================================================================

CIAO::Daemon_i::CMD_Start_Home::~CMD_Start_Home ()
{
}

int
CIAO::Daemon_i::CMD_Start_Home::parse_args (int argc,
                                            char *argv[]
                                            ACE_ENV_ARG_DECL_NOT_USED)
{
  ACE_Get_Opt get_opts (argc, argv, "s:m:c:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 's':
        this->softpkg_filename_ = CORBA::string_dup (get_opts.opt_arg ());
        break;

      case 'm':
        this->home_ior_filename_ = CORBA::string_dup (get_opts.opt_arg ());
        break;

      case 'c':
        this->com_server_ior_filename_ = CORBA::string_dup (get_opts.opt_arg ());
        break;

      case '?':
      case 'h':
      default:
        this->print_usage (argv[0]);
        return -1;
      }
  // Indicates sucessful parsing of the command line
  if (this->home_ior_filename_.in () == 0 ||
      this->com_server_ior_filename_.in () == 0)
    {
      this->print_usage (argv[0]);
      return -1;
    }

  return 0;
}

void
CIAO::Daemon_i::CMD_Start_Home::print_usage (const char *name) const
{
  ACE_ERROR ((LM_ERROR,
              "start_home:  %s\n"
              "-s <softpkg filename to install>\n"
              "-m <CCMHome IOR output filename>\n"
              "-c <ComponentServer IOR output filename>\n"
              "-? this help\n"
              "\n",
              name));
}

int
CIAO::Daemon_i::CMD_Start_Home::run (ACE_ENV_SINGLE_ARG_DECL)
{
  CIAO::Softpkg_Handler::Softpkg_Info info;
  info.csd_path_ = this->softpkg_filename_.in ();

  if (CIAO::XML_Utils::parse_softpkg (&info) == 0)
    {
      //      info.dump ();             // For debug purpose.
      CORBA::Object_var tmp =
        this->controller_->daemon ()->get_service ("ServerActivator"
                                                   ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      Components::Deployment::ServerActivator_var activator =
        Components::Deployment::ServerActivator::_narrow (tmp.in ()
                                                          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      if (CORBA::is_nil (activator.in ()))
        ACE_THROW_RETURN (CORBA::INTERNAL (), -1);

      Components::ConfigValues server_config;
      // @@ Nothing to config yet.

      Components::Deployment::ComponentServer_var com_server =
        activator->create_component_server (server_config
                                            ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      CORBA::String_var ior =
        this->controller_->orb ()->object_to_string (com_server.in ()
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      this->controller_->write_IOR (ior.in (),
                                    this->com_server_ior_filename_.in ());

      // Write IOR of newly created ComponentServer immediately.

      Components::ConfigValues container_config;
      // @@ Should we get the config value from Softpkg_Info?
      Components::Deployment::Container_var container =
        com_server->create_container (container_config
                                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      Components::ConfigValues home_config;
      // Setting home config value here:
      home_config.length (2);
      Components::ConfigValue_ptr item = new OBV_Components::ConfigValue ();

      item->name (CORBA::string_dup ("CIAO-servant-UUID"));
      item->value () <<= CORBA::string_dup (info.servant_UUID_.c_str ());
      home_config[0] = item;

      item = new OBV_Components::ConfigValue ();
      item->name (CORBA::string_dup ("CIAO-servant-entrypt"));
      item->value () <<= CORBA::string_dup (info.servant_entrypt_.c_str ());
      home_config[1] = item;

      Components::CCMHome_var home =
        container->install_home (info.executor_UUID_.c_str (),
                                 info.executor_entrypt_.c_str (),
                                 home_config
                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      ior =
        this->controller_->orb ()->object_to_string (home.in ()
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      this->controller_->write_IOR (ior.in (),
                                    this->home_ior_filename_.in ());
    }

  return 0;
}

//=================================================================

CIAO::Daemon_i::CMD_End_Home::~CMD_End_Home ()
{
}

int
CIAO::Daemon_i::CMD_End_Home::parse_args (int argc,
                                          char *argv[]
                                          ACE_ENV_ARG_DECL_NOT_USED)
{
  ACE_Get_Opt get_opts (argc, argv, "s:m:c:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'c':
        this->com_server_ior_ = CORBA::string_dup (get_opts.opt_arg ());
        break;

      case '?':
      case 'h':
      default:
        this->print_usage (argv[0]);
        return -1;
      }
  // Indicates sucessful parsing of the command line
  if (this->com_server_ior_.in () == 0)
    {
      this->print_usage (argv[0]);
      return -1;
    }

  return 0;
}

void
CIAO::Daemon_i::CMD_End_Home::print_usage (const char *name) const
{
  ACE_ERROR ((LM_ERROR,
              "end_home:  %s\n"
              "-c <ComponentServer IOR>\n"
              "-? this help\n"
              "\n",
              name));
}

int
CIAO::Daemon_i::CMD_End_Home::run (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::Object_var tmp =
    this->controller_->orb ()->string_to_object (this->com_server_ior_.in ()
                                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  Components::Deployment::ComponentServer_var server =
    Components::Deployment::ComponentServer::_narrow (tmp.in ()
                                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  Components::Deployment::ServerActivator_var activator =
    server->get_server_activator (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  activator->remove_component_server (server.in ()
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}

//=================================================================
