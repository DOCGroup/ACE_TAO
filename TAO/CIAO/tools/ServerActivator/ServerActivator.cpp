// $Id$

/**
 * @file Server_Activator.cpp
 *
 * @brief CIAO's server activator implementation
 *
 * ServerActivator is the bootstraping mechanism to create new
 * component servers on the localhost.  We currently depend on
 * starting the ServerActivator process on some specific port of all
 * the hosts that have ServerActivator install to function properly.
 */

#include "ServerActivator_Impl.h"
#include "tao/IORTable/IORTable.h"
#include "ace/SString.h"
#include "ace/Read_Buffer.h"
#include "ace/Get_Opt.h"

char *ior_file_name_ = 0;
char *comserv_path_ = "../ComponentServer/ComponentServer";
char *installation_ior_ = 0;
CORBA::ULong spawn_wait_ = 5;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "n:o:d:i:");
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

      case 'd':  // get the path name to the component server
        spawn_wait_ = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'i':  // get the ior to a ComponentInstallation interface
        installation_ior_ = get_opts.opt_arg ();
        break;

      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           "-n <component server pathname>"
                           "-o <ior_output_file>"
                           "-d <time (in second) to wait for component server>"
                           "\n",
                           argv [0]),
                          -1);
      }

  if (installation_ior_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "An IOR to a ComponentInstallation interface is needed."
                       "  Specified with \"-i <ior>\"\n"),
                      -1);

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

      // Activate POA manager
      PortableServer::POAManager_var mgr
        = poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      mgr->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CIAO::ServerActivator_Impl *activator_servant;

      ACE_NEW_RETURN (activator_servant,
                      CIAO::ServerActivator_Impl (orb.in (),
                                                  poa.in ()),
                      -1);

      activator_servant->init (comserv_path_,
                               spawn_wait_,
                               installation_ior_,
                               0,
                               0
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::ServantBase_var safe_servant (activator_servant);

      CORBA::String_var str = orb->object_to_string (activator_servant->_ciao_get_objref ()
                                                     ACE_ENV_ARG_PARAMETER);

      CORBA::Object_var table_object =
        orb->resolve_initial_references ("IORTable"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      IORTable::Table_var adapter =
        IORTable::Table::_narrow (table_object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (adapter.in ()))
        {
          ACE_ERROR ((LM_ERROR, "Nil IORTable\n"));
        }
      else
        {
          adapter->bind ("ServerActivator", str.in () ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      write_IOR (str.in ());
      ACE_DEBUG ((LM_INFO, "ServerActivator IOR: %s\n", str.in ()));

      // End Deployment part

      ACE_DEBUG ((LM_DEBUG,
                  "Running ServerActivator...\n"));

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

  return 0;
}
