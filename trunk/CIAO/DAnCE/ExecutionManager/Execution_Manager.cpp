//--*C++*--
// $Id$

#include "Execution_Manager_Impl.h"

// Include Name Service header
#include "orbsvcs/CosNamingC.h"
#include "tao/Utils/Implicit_Deactivator.h"

#include "ace/SString.h"
#include "ace/Read_Buffer.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdio.h"

namespace CIAO
{
  namespace Execution_Manager
  {
    const char *ior_file_name_ = "executionManager.ior";
    const char *init_file_name = "deployment.dat";
    const char *pid_file_name_ = 0;
    static bool register_with_ns_ = false;
    static bool write_to_ior_ = false;

    bool
    parse_args (int argc, char *argv[])
    {
      ACE_Get_Opt get_opts (argc, argv, "o:i:np:");
      int c;
      while ((c = get_opts ()) != -1)
        switch (c)
          {
          case 'o':
            write_to_ior_ = true;
            ior_file_name_ = get_opts.opt_arg ();
            break;
          case 'i':
            init_file_name = get_opts.opt_arg ();
            break;
          case 'n':
            register_with_ns_ = true;
            break;
          case 'p':
            pid_file_name_ = get_opts.opt_arg ();
            break;
          case '?':  // display help for use of the server.
          default:
            ACE_ERROR_RETURN ((LM_ERROR,
                               "usage:  %s\n"
                               "-o <ior_output_file>\n"
                               "-i <installation data filename>\n"
                               "-n <use naming service>\n"
                               "-p <filename to output the process id>\n"
                               "\n",
                               argv [0]),
                              false);
          }

      return true;
    }

    bool
    write_ior_file (CORBA::ORB_ptr orb,
                    CIAO::ExecutionManagerDaemon_ptr obj
                    ACE_ENV_ARG_DECL)
    {
      CORBA::String_var ior =
        orb->object_to_string (obj
                               ACE_ENV_ARG_PARAMETER);

      FILE* ior_output_file_ =
        ACE_OS::fopen (ior_file_name_, "w");

      if (ior_output_file_)
        {
          ACE_OS::fprintf (ior_output_file_,
                           "%s",
                           ior.in ());
          ACE_OS::fclose (ior_output_file_);
        }
      else
        return false;

      return true;
    }

    void
    write_pid (void)
    {
      if (pid_file_name_ == 0)
        return;

      FILE* pid_file = ACE_OS::fopen (pid_file_name_, "w");

      if (pid_file)
        {
          ACE_OS::fprintf (pid_file,
                           "%i",
                           ACE_OS::getpid ());
          ACE_OS::fclose (pid_file);
        }
    }

    bool
    register_with_ns (CORBA::ORB_ptr orb,
                      CIAO::ExecutionManagerDaemon_ptr obj
                      ACE_ENV_ARG_DECL)
    {
      // Naming Service related operations
      CORBA::Object_var naming_context_object =
        orb->resolve_initial_references ("NameService"
                                         ACE_ENV_ARG_PARAMETER);

      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_context_object.in ());

      // Initialize the Naming Sequence
      CosNaming::Name name (1);
      name.length (1);

      // String dup required for MSVC6
      name[0].id = CORBA::string_dup ("ExecutionManager");

      // Register the servant with the Naming Service
      try
        {
          // Register the servant with the Naming Service
          naming_context->bind (name, obj);
        }
      catch (CosNaming::NamingContext::AlreadyBound &)
        {
          ACE_DEBUG ((LM_DEBUG, "Execution_Manager.cpp: Name already bound, rebinding....\n"));
          naming_context->rebind (name, obj);
        }

      return true;
    }

    int
    run_main (int argc, char *argv[])
    {
      ACE_DECLARE_NEW_CORBA_ENV;

      ACE_TRY
        {
          CORBA::ORB_var orb =
            CORBA::ORB_init (argc,
                             argv,
                             ""
                             ACE_ENV_ARG_PARAMETER);

          if (!parse_args (argc, argv))
            return -1;

          // Get reference to Root POA.
          CORBA::Object_var obj
            = orb->resolve_initial_references ("RootPOA"
                                               ACE_ENV_ARG_PARAMETER);

          PortableServer::POA_var poa =
            PortableServer::POA::_narrow (obj.in ()
                                          ACE_ENV_ARG_PARAMETER);


          if (CORBA::is_nil (poa.in ()))
            ACE_ERROR_RETURN ((LM_ERROR,
                               "(%P|%t) CIAO_ExecutionManager: "
                               "Nil POA panic error, returning \n"),
                              -1);

          // Create and install the CIAO Daemon servant
          Execution_Manager_Impl *daemon_servant = 0;
          ACE_NEW_RETURN (daemon_servant,
                          Execution_Manager_Impl(orb.in (),
                                                 poa.in (),
                                                 init_file_name),
                          -1);

          // Implicit activation
          PortableServer::ServantBase_var safe_daemon (daemon_servant);

          CIAO::ExecutionManagerDaemon_var daemon =
            daemon_servant->_this ();

          TAO::Utils::Implicit_Deactivator de (daemon_servant);
          bool retval = false;

          if (register_with_ns_)
            {
              retval =
                register_with_ns (orb.in (),
                                  daemon.in ()
                                  ACE_ENV_ARG_PARAMETER);
            }

          if (write_to_ior_)
            {
              retval =
                write_ior_file (orb.in (),
                                daemon.in ()
                                ACE_ENV_ARG_PARAMETER);
            }

          if (!retval)
            return -1;

          // Activate POA manager
          PortableServer::POAManager_var mgr =
            poa->the_POAManager ();

          if (mgr.in () == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "(%P|%t) CIAO_ExecutionManager: "
                               "Nil POA Manager error, returning \n"),
                              -1);

          mgr->activate ();

          // End Deployment part
          ACE_DEBUG ((LM_DEBUG,
                      "CIAO_ExecutionManager is running...\n"));

          write_pid ();

          // Run the main event loop for the ORB.
          orb->run ();

          // Forget the pointer. The POA will take care of it during
          // destroy.
          (void) de.release ();

          poa->destroy (1,
                        1
                        ACE_ENV_ARG_PARAMETER);

          orb->destroy ();
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               "CIAO_ExecutionManager::main\t\n");
          return -1;
        }
      ACE_ENDTRY;

      ACE_DEBUG ((LM_DEBUG,
                  "CIAO_ExecutionManager has closed\n"));
      return 0;
    }

  }
}

int
main (int argc, char *argv[])
{
  return CIAO::Execution_Manager::run_main (argc,
                                            argv);
}
