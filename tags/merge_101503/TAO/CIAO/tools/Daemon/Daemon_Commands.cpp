// $Id$

#include "Daemon_Commands.h"
#include "ace/Get_Opt.h"
#include "CCM_DeploymentC.h"

#if !defined (__ACE_INLINE__)
# include "Daemon_Commands.inl"
#endif /* __ACE_INLINE__ */

//=================================================================

CIAO::Daemon_i::CMD_Shutdown::~CMD_Shutdown ()
{
}

int
CIAO::Daemon_i::CMD_Shutdown::parse_args (int ,
                                          char *[]
                                          ACE_ENV_ARG_DECL_NOT_USED)
{
  return 0;
}

int
CIAO::Daemon_i::CMD_Shutdown::run (ACE_ENV_SINGLE_ARG_DECL)
{
  this->controller_->daemon ()->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  return 0;
}

//=================================================================

CIAO::Daemon_i::CMD_Install::~CMD_Install ()
{
}

int
CIAO::Daemon_i::CMD_Install::parse_args (int argc,
                                         char *argv[]
                                         ACE_ENV_ARG_DECL_NOT_USED)
{
  ACE_Get_Opt get_opts (argc, argv, "c:l:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'c':
        this->component_name_ = CORBA::string_dup (get_opts.opt_arg ());
        break;

      case 'l':
        this->location_ = CORBA::string_dup (get_opts.opt_arg ());
        break;

      case '?':
      case 'h':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "install:  %s\n"
                           "-c <component UUID>\n"
                           "-l <component location>\n"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
CIAO::Daemon_i::CMD_Install::run (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::Object_var tmp =
    this->controller_->daemon ()->get_service ("ComponentInstallation"
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  Components::Deployment::ComponentInstallation_var inst =
    Components::Deployment::ComponentInstallation::_narrow (tmp.in ()
                                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil (inst.in ()))
    ACE_THROW_RETURN (CORBA::INTERNAL (), -1);

  inst->install (this->component_name_.in (),
                 this->location_.in ()
                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}

//=================================================================

CIAO::Daemon_i::CMD_Uninstall::~CMD_Uninstall ()
{
}

int
CIAO::Daemon_i::CMD_Uninstall::parse_args (int argc,
                                           char *argv[]
                                           ACE_ENV_ARG_DECL_NOT_USED)
{
  ACE_Get_Opt get_opts (argc, argv, "c:l:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'c':
        this->component_name_ = CORBA::string_dup (get_opts.opt_arg ());
        break;

      case '?':
      case 'h':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "uninstall:  %s\n"
                           "-c <component UUID>\n"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
CIAO::Daemon_i::CMD_Uninstall::run (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::Object_var tmp =
    this->controller_->daemon ()->get_service ("ComponentInstallation"
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  Components::Deployment::ComponentInstallation_var inst =
    Components::Deployment::ComponentInstallation::_narrow (tmp.in ()
                                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil (inst.in ()))
    ACE_THROW_RETURN (CORBA::INTERNAL (), -1);

  inst->remove (this->component_name_.in ()
                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}

//=================================================================

CIAO::Daemon_i::CMD_Query::~CMD_Query ()
{
}

int
CIAO::Daemon_i::CMD_Query::parse_args (int argc,
                                       char *argv[]
                                       ACE_ENV_ARG_DECL_NOT_USED)
{
  ACE_Get_Opt get_opts (argc, argv, "c:l:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'c':
        this->component_name_ = CORBA::string_dup (get_opts.opt_arg ());
        break;

      case '?':
      case 'h':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "query:  %s\n"
                           "-c <component UUID>\n"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
CIAO::Daemon_i::CMD_Query::run (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::Object_var tmp =
    this->controller_->daemon ()->get_service ("ComponentInstallation"
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  Components::Deployment::ComponentInstallation_var inst =
    Components::Deployment::ComponentInstallation::_narrow (tmp.in ()
                                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil (inst.in ()))
    ACE_THROW_RETURN (CORBA::INTERNAL (), -1);

  CORBA::String_var str =
    inst->get_implementation (this->component_name_.in ()
                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_DEBUG ((LM_DEBUG,
              "Implmentation for %s is located at:\n\t%s\n",
              this->component_name_.in (),
              str.in ()));

  return 0;
}

//=================================================================

CIAO::Daemon_i::CMD_Replace::~CMD_Replace ()
{
}

int
CIAO::Daemon_i::CMD_Replace::parse_args (int argc,
                                         char *argv[]
                                         ACE_ENV_ARG_DECL_NOT_USED)
{
  ACE_Get_Opt get_opts (argc, argv, "c:l:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'c':
        this->component_name_ = CORBA::string_dup (get_opts.opt_arg ());
        break;

      case 'l':
        this->location_ = CORBA::string_dup (get_opts.opt_arg ());
        break;

      case '?':
      case 'h':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "replace:  %s\n"
                           "-c <component UUID>\n"
                           "-l <new component location>\n"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
CIAO::Daemon_i::CMD_Replace::run (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::Object_var tmp =
    this->controller_->daemon ()->get_service ("ComponentInstallation"
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  Components::Deployment::ComponentInstallation_var inst =
    Components::Deployment::ComponentInstallation::_narrow (tmp.in ()
                                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil (inst.in ()))
    ACE_THROW_RETURN (CORBA::INTERNAL (), -1);

  inst->replace (this->component_name_.in (),
                 this->location_.in ()
                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}

//=================================================================

CIAO::Daemon_i::CMD_Get_IOR::~CMD_Get_IOR ()
{
}

int
CIAO::Daemon_i::CMD_Get_IOR::parse_args (int argc,
                                         char *argv[]
                                         ACE_ENV_ARG_DECL_NOT_USED)
{
  ACE_Get_Opt get_opts (argc, argv, "o:n:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'n':
        this->service_name_ = CORBA::string_dup (get_opts.opt_arg ());
        break;

      case 'o':
        this->filename_ = CORBA::string_dup (get_opts.opt_arg ());
        break;

      case '?':
      case 'h':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "get_ior:  %s\n"
                           "-n service name\n"
                           "-o <filename>\n"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
CIAO::Daemon_i::CMD_Get_IOR::run (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::Object_var tmp =
    this->controller_->daemon ()->get_service (this->service_name_.in ()
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil (tmp.in ()))
    ACE_THROW_RETURN (CIAO::NotFound (), -1);

  CORBA::String_var ior
    = this->controller_->orb ()->object_to_string (tmp.in ()
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_DEBUG ((LM_DEBUG,
              "%s IOR: %s\n",
              this->service_name_.in (),
              ior.in ()));

  if (this->filename_.in () != 0)
    this->controller_->write_IOR (ior.in (),
                                  this->filename_.in ());

  return 0;
}
