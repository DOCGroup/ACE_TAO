// $Id$

#include "controller_i.h"
#include "Daemon_Commands.h"
#include "Softpkg_Commands.h"
#include "Client_init.h"
#include "ace/Get_Opt.h"
#include "CCM_DeploymentC.h"

#if !defined (__ACE_INLINE__)
# include "controller_i.inl"
#endif /* __ACE_INLINE__ */

void
CIAO::Daemon_i::controller::print_usage (void)
{
  ACE_ERROR ((LM_ERROR,
              "Usage: DaemonController [options] command [command-arguments]\n"
              "  where [options] are ORB options\n"
              "  where command is one of the following:\n"
              "    install    Install a component into the ComponentInstallation\n"
              "    get_ior    get the IOR of a service supported by this daemon\n"
              "    uninstall  Ininstall a component from the ComponentInstallation\n"
              "    shutdown   Shuts down the Daemon process\n"
              "    replace    Replace a component with another one\n"
              "    query      Show the location of a component in the ComponentInstallation\n"
              "    start_home Start up a ComponentServer and install a component home\n"
              "    end_home   Terminate a ComponentServer and remove the home and components\n"
              "  where [command-arguments] depend on the command\n"));
}

int
CIAO::Daemon_i::controller::write_IOR(const char* ior,
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
CIAO::Daemon_i::controller::parse_args (int argc, char *argv[] ACE_ENV_ARG_DECL)
{
  // Make sure one command was given
  if (argc < 2)
  {
    this->print_usage ();
    return -1;
  }

  const char *op_name = argv[1];

  if (ACE_OS::strcasecmp (op_name, ACE_TEXT ("install")) == 0)
    this->cmd_ = new CIAO::Daemon_i::CMD_Install (this);
  else if (ACE_OS::strcasecmp (op_name, ACE_TEXT ("get_ior")) == 0)
    this->cmd_ = new CIAO::Daemon_i::CMD_Get_IOR (this);
  else if (ACE_OS::strcasecmp (op_name, ACE_TEXT ("uninstall")) == 0)
    this->cmd_ = new CIAO::Daemon_i::CMD_Uninstall (this);
  else if (ACE_OS::strcasecmp (op_name, ACE_TEXT ("shutdown")) == 0)
    this->cmd_ = new CIAO::Daemon_i::CMD_Shutdown (this);
  else if (ACE_OS::strcasecmp (op_name, ACE_TEXT ("replace")) == 0)
    this->cmd_ = new CIAO::Daemon_i::CMD_Replace (this);
  else if (ACE_OS::strcasecmp (op_name, ACE_TEXT ("query")) == 0)
    this->cmd_ = new CIAO::Daemon_i::CMD_Query (this);
  else if (ACE_OS::strcasecmp (op_name, ACE_TEXT ("start_home")) == 0)
    this->cmd_ = new CIAO::Daemon_i::CMD_Start_Home (this);
  else if (ACE_OS::strcasecmp (op_name, ACE_TEXT ("end_home")) == 0)
    this->cmd_ = new CIAO::Daemon_i::CMD_End_Home (this);
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
CIAO::Daemon_i::controller::init (int argc,
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

  this->daemon_ = CIAO::Daemon::_narrow (obj.in ()
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil (this->daemon_.in ()))
    return -1;

  if (this->parse_args (argc, argv) != 0)
    return -1;

  return 0;
}

int
CIAO::Daemon_i::controller::run (ACE_ENV_SINGLE_ARG_DECL)
{
  this->cmd_->run (ACE_ENV_SINGLE_ARG_PARAMETER);
  return 0;
}

int
CIAO::Daemon_i::controller::fini (void)
{
  this->orb_->destroy ();
  delete this->cmd_;
  this->cmd_ = 0;
  return 0;
}

//=================================================================

CIAO::Daemon_i::Command_Base::~Command_Base ()
{
}
