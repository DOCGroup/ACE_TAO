// $Id$

#include "controller_i.h"
#include "Client_init.h"
#include "ace/Get_Opt.h"
#include "CCM_DeploymentC.h"

#if !defined (__ACE_INLINE__)
# include "controller_i.inl"
#endif /* __ACE_INLINE__ */

void
CIAO::controller::print_usage (void)
{
  ACE_ERROR ((LM_ERROR,
              "Usage: DaemonController [options] command [command-arguments]\n"
              "  where [options] are ORB options\n"
              "  where command is one of the following:\n"
              "    install   Install a component into the ComponentInstallation\n"
              "    get_ior   get the IOR of a service supported by this daemon\n"
              "    uninstall Ininstall a component from the ComponentInstallation\n"
              "    shutdown  Shuts down the Daemon process\n"
              "    replace   Replace a component with another one\n"
              "    query     Show the location of a component in the ComponentInstallation\n"
              "  where [command-arguments] depend on the command\n"));
}

int
CIAO::controller::write_IOR(const char* ior,
                            const char *filename)
{
  FILE* ior_output_file_ =
    ACE_OS::fopen (filename, "w");

  if (ior_output_file_)
    {
      ACE_OS::fprintf (ior_output_file_,
                       "%s",
                       ior);
      ACE_OS::fclose (ior_output_file_);
    }

  return 0;
}

int
CIAO::controller::parse_args (int argc, char *argv[] ACE_ENV_ARG_DECL)
{
  // Make sure one command was given
  if (argc < 2)
  {
    this->print_usage ();
    return -1;
  }

  const char *op_name = argv[1];

  if (ACE_OS::strcasecmp (op_name, ACE_TEXT ("install")) == 0)
    this->cmd_ = new CIAO::CMD_Install (this);
  else if (ACE_OS::strcasecmp (op_name, ACE_TEXT ("get_ior")) == 0)
    this->cmd_ = new CIAO::CMD_Get_IOR (this);
  else if (ACE_OS::strcasecmp (op_name, ACE_TEXT ("uninstall")) == 0)
    this->cmd_ = new CIAO::CMD_Uninstall (this);
  else if (ACE_OS::strcasecmp (op_name, ACE_TEXT ("shutdown")) == 0)
    this->cmd_ = new CIAO::CMD_Shutdown (this);
  else if (ACE_OS::strcasecmp (op_name, ACE_TEXT ("replace")) == 0)
    this->cmd_ = new CIAO::CMD_Replace (this);
  else if (ACE_OS::strcasecmp (op_name, ACE_TEXT ("query")) == 0)
    this->cmd_ = new CIAO::CMD_Query (this);
  else
    {
      this->print_usage ();
      return -1;
    }

  if (this->cmd_ == 0)
    return -1;

  return this->cmd_->parse_args (argc - 1, argv + 1);
}

int
CIAO::controller::init (int argc,
                        char *argv[]
                        ACE_ENV_ARG_DECL)
{
  this->orb_ =
    CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CIAO::Client_init (this->orb_.in ());

  CORBA::Object_var obj = this->orb_->resolve_initial_references ("CIAODaemon"
                                                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  this->daemon_ = CIAO::Daemon::_narrow (obj
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil (this->daemon_.in ()))
    return -1;

  if (this->parse_args (argc, argv) != 0)
    return -1;

  return 0;
}

int
CIAO::controller::run (ACE_ENV_SINGLE_ARG_DECL)
{
  this->cmd_->run (ACE_ENV_SINGLE_ARG_PARAMETER);
  return 0;
}

int
CIAO::controller::fini (void)
{
  this->orb_->destroy ();
  delete this->cmd_;
  this->cmd_ = 0;
  return 0;
}

//=================================================================

CIAO::Command::~Command ()
{
}

//=================================================================

CIAO::CMD_Shutdown::~CMD_Shutdown ()
{
}

int
CIAO::CMD_Shutdown::parse_args (int ,
                                char *[]
                                ACE_ENV_ARG_DECL_NOT_USED)
{
  return 0;
}

int
CIAO::CMD_Shutdown::run (ACE_ENV_SINGLE_ARG_DECL)
{
  this->controller_->daemon ()->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  return 0;
}

//=================================================================

CIAO::CMD_Install::~CMD_Install ()
{
}

int
CIAO::CMD_Install::parse_args (int argc,
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
CIAO::CMD_Install::run (ACE_ENV_SINGLE_ARG_DECL)
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

CIAO::CMD_Uninstall::~CMD_Uninstall ()
{
}

int
CIAO::CMD_Uninstall::parse_args (int argc,
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
CIAO::CMD_Uninstall::run (ACE_ENV_SINGLE_ARG_DECL)
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

CIAO::CMD_Query::~CMD_Query ()
{
}

int
CIAO::CMD_Query::parse_args (int argc,
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
CIAO::CMD_Query::run (ACE_ENV_SINGLE_ARG_DECL)
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

CIAO::CMD_Replace::~CMD_Replace ()
{
}

int
CIAO::CMD_Replace::parse_args (int argc,
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
CIAO::CMD_Replace::run (ACE_ENV_SINGLE_ARG_DECL)
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

CIAO::CMD_Get_IOR::~CMD_Get_IOR ()
{
}

int
CIAO::CMD_Get_IOR::parse_args (int argc,
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
CIAO::CMD_Get_IOR::run (ACE_ENV_SINGLE_ARG_DECL)
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
