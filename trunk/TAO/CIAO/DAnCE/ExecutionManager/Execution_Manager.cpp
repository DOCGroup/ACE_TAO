//--*C++*--

//==============================================================
/**
 * @file Execution_Manager.cpp
 *
 * @brief CIAO's Execution Manager implementation
 *
 * $Id$
 *
 * ExecutionManager is the DnC entity which is the starting point for
 * the deployment process. This entity collaborates with other entities
 * including Logging Facility, TargetManager, and
 * DomainApplicationManager entities to facilitate the deployment and
 * launching of components. In our current design, the ExecutionManager
 * is started as an executable on a give host and registered with the
 * name service. The Executor/client obtains this reference and starts
 * the deployment process.
 *
 *
 * @author Arvind S. Krishna <arvindk@dre.vanderbilt.edu>
 * @author Tao Lu <lu@dre.vanderbilt.edu>
 */
//===============================================================

#include "ExecutionManager_Impl.h"
#include "Server_init.h"
#include "tao/IORTable/IORTable.h"

// Include Name Service header
#include "orbsvcs/CosNamingC.h"

#include "ace/SString.h"
#include "ace/Read_Buffer.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdio.h"

const char *ior_file_name_ = "executionManager.ior";
char * default_svcconf_ = 0;
char * svcconf_config_ = 0;
const char * name = "ExecutionManager";
const char * init_file_name = "deployment.dat";

int write_to_ior_ = 0;
int register_with_ns_ = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:c:m:i:n");
  int c;
  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':  // get the file name to write to
       ior_file_name_ = get_opts.opt_arg ();
       write_to_ior_ = 1;
      break;

      case 'c':  // get the default svc.conf filename
        default_svcconf_ = get_opts.opt_arg ();
      break;

      case 'i':  // get the svc.conf map configuration filename
        init_file_name = get_opts.opt_arg ();
      break;

      case 'n':
        register_with_ns_ = 1;
      break;

      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s\n"
                           "-o <ior_output_file>\n"
                           "-c <svc.conf file>\n"
                           "-i <installation data filename>\n"
                           "-n <use naming service>\n"
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

bool
register_with_ns (const char * name_context,
                  CORBA::ORB_ptr orb,
                  CIAO::ExecutionManagerDaemon_ptr obj
                  ACE_ENV_ARG_DECL)
{
  // Naming Service related operations
  CORBA::Object_var naming_context_object =
    orb->resolve_initial_references ("NameService"
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (false);

  CosNaming::NamingContext_var naming_context =
    CosNaming::NamingContext::_narrow (naming_context_object.in ());

  // Initialize the Naming Sequence
  CosNaming::Name name (1);
  name.length (1);
  // Register the name with the NS
  name[0].id = name_context;

  // Register the servant with the Naming Service
  naming_context->bind (name,
                        obj
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (false);

  return true;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      // @@ Error checking is very bad! Need to be fixed!
      // -- bala
      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            ""
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
        IORTable::Table::_narrow (table_object.in ()
                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (adapter.in ()))
          ACE_ERROR_RETURN ((LM_ERROR, "Nil IORTable\n"), -1);

      // Create and install the CIAO Daemon servant
      CIAO::ExecutionManager_Impl *daemon_servant = 0;
      ACE_NEW_RETURN (daemon_servant,
                      CIAO::ExecutionManager_Impl(orb.in (),
                                                  poa.in (),
                                                  init_file_name),
                      -1);

      // Implicit activation
      PortableServer::ServantBase_var safe_daemon (daemon_servant);
      CIAO::ExecutionManagerDaemon_var daemon =
        daemon_servant->_this ();

      // Now register daemon with IOR table and write its IOR.
      CORBA::String_var str =
        orb->object_to_string (daemon.in ()
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      adapter->bind ("ExecutionManager",
                     str.in ()
                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (write_to_ior_)
        write_IOR (str.in ());
      else if (register_with_ns_)
        {
          // Register this name with the Naming Service
          (void) register_with_ns (name,
                                   orb.in (),
                                   daemon.in ()
                                   ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

#if 0
      ACE_DEBUG ((LM_INFO,
                  "CIAO_ExecutionMananger IOR: %s\n", str.in ()));
#endif /*if 0*/

      // Activate POA manager
      PortableServer::POAManager_var mgr
        = poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      mgr->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // End Deployment part
      ACE_DEBUG ((LM_DEBUG,
                  "CIAO_ExecutionManager is running...\n"));

      // Run the main event loop for the ORB.
      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK

      poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "CIAO_ExecutionManager::main\t\n");
      return 1;
    }
  ACE_ENDTRY;

  ACE_DEBUG ((LM_DEBUG,
              "CIAO_ExecutionManager has closed\n"));
  return 0;
}
