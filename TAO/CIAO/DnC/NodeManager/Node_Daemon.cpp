// $Id$

/**
 * @file Node_Daemon.cpp
 *
 * @brief CIAO's Node Daemon implementation
 *
 * NodeDaemon is the bootstraping mechanism to create new
 * NodeApplication on the localhost.  We currently depend on
 * starting the NodeApplicationManager process on some specific port of all
 * the hosts that have NodeApplication install to function properly.
 */

#include "NodeDaemon_Impl.h"
#include "ciao/Server_init.h"
#include "tao/IORTable/IORTable.h"

#include "ace/SString.h"
#include "ace/Read_Buffer.h"
#include "ace/Get_Opt.h"

const char *ior_file_name_ = "daemon.ior";
char *default_svcconf_ = 0;
char *svcconf_config_ = 0;


int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:c:m:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':  // get the file name to write to
       ior_file_name_ = get_opts.opt_arg ();
      break;

      case 'c':  // get the default svc.conf filename for ComponentServer
        default_svcconf_ = get_opts.opt_arg ();
      break;

      case 'm':  // get the svc.conf map configuration filename
        svcconf_config_ = get_opts.opt_arg ();
      break;

      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s\n"
                           "-o <ior_output_file>\n"
                           "-c <svc.conf file>\n"
                           "-i <installation data filename>\n"
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
                                            argv,
                                            ""
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CIAO::Server_init (orb.in ());

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
      CIAO::NodeDaemon_Impl *daemon_servant = 0;
      ACE_NEW_RETURN (daemon_servant,
                      CIAO::NodeDaemon_Impl("NodeDaemon",
					    orb.in (),
					    poa.in ()),
                      -1);
      PortableServer::ServantBase_var safe_daemon (daemon_servant);
      // Implicit activation
      CIAO::NodeDaemon_var daemon = daemon_servant->_this ();

      CORBA::String_var str = orb->object_to_string (daemon.in ()
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      adapter->bind ("NodeManager", str.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Now register daemon with IOR table and write its IOR.
      str = orb->object_to_string (daemon.in ()
                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      adapter->bind ("CIAO_NodeDaemon", str.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      write_IOR (str.in ());
      ACE_DEBUG ((LM_INFO, "CIAO_NodeDaemon IOR: %s\n", str.in ()));

      // Activate POA manager
      PortableServer::POAManager_var mgr
        = poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      mgr->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // End Deployment part
      ACE_DEBUG ((LM_DEBUG,
                  "CIAO_NodeDaemon is running...\n"));

      // Run the main event loop for the ORB.
      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "CIAO_NodeDaemon::main\t\n");
      return 1;
    }
  ACE_ENDTRY;

  ACE_DEBUG ((LM_DEBUG,
              "CIAO_NodeDaemon has closed\n"));
  return 0;
}
