// $Id$

/**
 * @file CIAO_Daemon.cpp
 *
 * @brief CIAO's server activator implementation
 *
 * ServerActivator is the bootstraping mechanism to create new
 * component servers on the localhost.  We currently depend on
 * starting the ServerActivator process on some specific port of all
 * the hosts that have ServerActivator install to function properly.
 */

#include "ServerActivator_Impl.h"
#include "ComponentInstallation_Impl.h"
#include "Daemon_Impl.h"
#include "tao/IORTable/IORTable.h"
#include "ace/SString.h"
#include "ace/Read_Buffer.h"
#include "ace/Get_Opt.h"

char *ior_file_name_ = "daemon.ior";
char *comserv_path_ = "../ComponentServer/ComponentServer";
CORBA::ULong spawn_wait_ = 5;
char *installation_datafile_ = "CIAO_Installation_Data.ini";
char *section_name_ = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "i:n:o:d:s:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':  // get the file name to write to
       ior_file_name_ = get_opts.opt_arg ();
      break;

      case 'n':  // get the path name to the component server
        comserv_path_ = get_opts.opt_arg ();
      break;

      case 'd':  // time to wait before we consider a ComponentServer
                 // fails to start.
        spawn_wait_ = ACE_OS::atoi (get_opts.opt_arg ());
      break;

      case 'i':  // get the name for the ComponentInstallation data file
        installation_datafile_ = get_opts.opt_arg ();
        break;

      case 's':  // get the section name to use in ComponentInstallation data file
        section_name_ = get_opts.opt_arg ();
        break;

      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s\n"
                           "-n <component server pathname>\n"
                           "-o <ior_output_file>\n"
                           "-d <time (in second) to wait for component server>\n"
                           "-i <installation data filename>\n"
                           "-s <section name to use in installation data file>\n"
                           "\n",
                           argv [0]),
                          -1);
      }

  return 0;
}

int
write_IOR(const char* ior)
{
  FILE* ior_output_file_ =
    ACE_OS::fopen (ior_file_name_, "w");

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
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return -1;

      // Get reference to Root POA.
      CORBA::Object_var obj
        = orb->resolve_initial_references ("RootPOA"
                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var poa
        = PortableServer::POA::_narrow (obj.in ()
                                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var table_object =
        orb->resolve_initial_references ("IORTable"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      IORTable::Table_var adapter =
        IORTable::Table::_narrow (table_object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (adapter.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR, "Nil IORTable\n"), -1);
        }

      // Create and install the CIAO Daemon servant

      CIAO::Daemon_Impl *daemon_servant = 0;
      ACE_NEW_RETURN (daemon_servant,
                      CIAO::Daemon_Impl ("CIAODaemon",
                                         orb.in (),
                                         poa.in ()),
                      -1);
      PortableServer::ServantBase_var safe_daemon (daemon_servant);
      // Implicit activation
      CIAO::Daemon_var daemon = daemon_servant->_this ();

      // Create and install the ComponentInstallation servant

      CIAO::ComponentInstallation_Impl *installation_servant = 0;

      ACE_NEW_RETURN (installation_servant,
                      CIAO::ComponentInstallation_Impl (orb.in (),
                                                        poa.in ()),
                      -1);

      installation_servant->init (installation_datafile_,
                                  section_name_
                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::ServantBase_var safe_installation (installation_servant);

      Components::Deployment::ComponentInstallation_var installation
        = installation_servant->_this ();

      CORBA::String_var str = orb->object_to_string (installation.in ()
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      adapter->bind ("ComponentInstallation", str.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (daemon_servant->bind ("ComponentInstallation", installation.in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Failed to register ComponentInstallation with CIAO Daemon\n"),
                          -1);

      // Create and install the ServerActivator servant

      CIAO::ServerActivator_Impl *activator_servant = 0;

      ACE_NEW_RETURN (activator_servant,
                      CIAO::ServerActivator_Impl (orb.in (),
                                                  poa.in ()),
                      -1);

      activator_servant->init (comserv_path_,
                               spawn_wait_,
                               str.in ()
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::ServantBase_var safe_activator (activator_servant);

      Components::Deployment::ServerActivator_ptr activator
        = activator_servant->_ciao_get_objref ();

      str = orb->object_to_string (activator
                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      adapter->bind ("ServerActivator", str.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (daemon_servant->bind ("ServerActivator", activator) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Failed to register ServerActivator with CIAO Daemon\n"),
                          -1);

      // Now register daemon with IOR table and write its IOR.
      str = orb->object_to_string (daemon.in ()
                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      adapter->bind ("CIAO-Daemon", str.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      write_IOR (str.in ());
      ACE_DEBUG ((LM_INFO, "CIAO-Daemon IOR: %s\n", str.in ()));

      // Activate POA manager
      PortableServer::POAManager_var mgr
        = poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      mgr->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // End Deployment part

      ACE_DEBUG ((LM_DEBUG,
                  "CIAO_Daemon is running...\n"));

      // Run the main event loop for the ORB.
      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "server::main\t\n");
      return 1;
    }
  ACE_ENDTRY;

  ACE_DEBUG ((LM_DEBUG,
              "CIAO_Daemon has closed\n"));

  return 0;
}
