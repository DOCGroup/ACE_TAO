//--*C++*--
// $Id$

#include "Execution_Manager_Impl.h"

// Include Name Service header
#include "orbsvcs/CosNamingC.h"
#include "tao/Utils/Implicit_Deactivator.h"

#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTCORBA/RT_Policy_i.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "tao/Strategies/advanced_resource.h"

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
    static bool rt_corba_enabled = false;
    static bool is_using_ami = false;
    static bool is_using_active_object = false;

    bool
    parse_args (int argc, char *argv[])
    {
      ACE_Get_Opt get_opts (argc, argv, "o:i:abnrp:");
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
            case 'r':
              rt_corba_enabled = true;
              break;
            case 'p':
              pid_file_name_ = get_opts.opt_arg ();
              break;
            case 'a':
              is_using_ami = true;
              break;
            case 'b':
              is_using_active_object = true;
              break;
            case '?':  // display help for use of the server.
            default:
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "usage:  %s\n"
                                 "-a : Using Asynchronous Deployment\n"
                                 "-b : Using Active Object based deployment \n"
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
    write_ior_file (CORBA::ORB_ptr orb, CIAO::ExecutionManagerDaemon_ptr obj)
    {
      CORBA::String_var ior = orb->object_to_string (obj);

      FILE* ior_output_file_ = ACE_OS::fopen (ior_file_name_, "w");

      if (ior_output_file_)
        {
          ACE_OS::fprintf (ior_output_file_,
                           "%s",
                           ior.in ());
          ACE_OS::fclose (ior_output_file_);
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      "Unable to open ExecutionManager IOR output file %s : %m\n",
                      ior_file_name_));
          return false;
        }
      return true;
    }

    bool
    write_pid (void)
    {

      FILE* pid_file = ACE_OS::fopen (pid_file_name_, "w");

      if (!pid_file)
        {
          ACE_ERROR ( (LM_ERROR, "Unable to open file %s to write the PID : %m",
                       pid_file_name_));
          return false;
        }
      ACE_OS::fprintf (pid_file, "%i", ACE_OS::getpid ());
      ACE_OS::fclose (pid_file);
      return true;
    }


    bool
    register_with_ns (CORBA::ORB_ptr orb,
                      CIAO::ExecutionManagerDaemon_ptr obj)
    {
      // Naming Service related operations
      CORBA::Object_var naming_context_object =
        orb->resolve_initial_references ("NameService");

      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_context_object.in ());

      // Initialize the Naming Sequence
      CosNaming::Name name (1);
      name.length (1);

      name[0].id = CORBA::string_dup ("ExecutionManager");

      // Register the servant with the Naming Service
      try
        {
          // Register the servant with the Naming Service
          naming_context->bind (name, obj);
        }
      catch (const CosNaming::NamingContext::AlreadyBound &)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Execution_Manager.cpp: Name already bound, rebinding\n"));
          naming_context->rebind (name, obj);
        }

      return true;
    }

    int
    run_main (int argc, char *argv[])
    {

      try
        {
          CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
          if (!parse_args (argc, argv))
            return -1;

          // RTORB.
          CORBA::Object_var object =
            orb->resolve_initial_references ("RTORB");
          RTCORBA::RTORB_var rt_orb = RTCORBA::RTORB::_narrow (object.in ());

          if (CORBA::is_nil (rt_orb.in ()))
            ACE_ERROR_RETURN ((LM_ERROR,
                               "(%P|%t) CIAO_ExecutionManager: "
                               "Nil RT_ORB panic error, returning \n"),
                              -1);

          // Get reference to Root POA.
          CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");

          PortableServer::POA_var root_poa =
            PortableServer::POA::_narrow (obj.in ());

          if (CORBA::is_nil (root_poa.in ()))
            ACE_ERROR_RETURN ((LM_ERROR,
                               "(%P|%t) CIAO_ExecutionManager: "
                               "Nil Root POA panic error, returning \n"),
                              -1);

          // POAManager.
          PortableServer::POAManager_var poa_manager =
            root_poa->the_POAManager ();

          if (poa_manager.in () == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "(%P|%t) CIAO_ExecutionManager: "
                               "Nil POA Manager panic error, returning \n"),
                              -1);

          PortableServer::POA_var child_poa;
          if (rt_corba_enabled)
            {
              // Create child POA with RTCORBA::ClientProtocolPolicy set.
              CORBA::PolicyList poa_policy_list;
              poa_policy_list.length (1);
              poa_policy_list[0] =
                rt_orb->create_priority_model_policy (RTCORBA::CLIENT_PROPAGATED,
                                                      0);

              child_poa =
                root_poa->create_POA ("Child_POA",
                                      poa_manager.in (),
                                      poa_policy_list);


            }
          else
            {
              child_poa =
                root_poa->create_POA ("Child_POA",
                                      poa_manager.in (),
                                      0);
            }

          if (CORBA::is_nil (child_poa.in ()))
            ACE_ERROR_RETURN ((LM_ERROR,
                               "(%P|%t) CIAO_ExecutionManager: "
                               "Nil Child POA panic error, returning \n"),
                              -1);

          // Create and install the CIAO Daemon servant on child POA
          Execution_Manager_Impl *daemon_servant = 0;
          ACE_NEW_RETURN (daemon_servant,
                          Execution_Manager_Impl(orb.in (),
                                                 child_poa.in (),
                                                 init_file_name,
                                                 is_using_ami,
                                                 is_using_active_object),
                          -1);

          // Explicit activation through the child POA
          PortableServer::ServantBase_var safe_daemon (daemon_servant);
          TAO::Utils::Implicit_Deactivator de (daemon_servant);

          PortableServer::ObjectId_var id =
            child_poa->activate_object (daemon_servant);

          CORBA::Object_var daemon_obj =
            child_poa->id_to_reference (id.in ());

          CIAO::ExecutionManagerDaemon_var daemon =
            CIAO::ExecutionManagerDaemon::_narrow (daemon_obj.in ());

          // Register to naming service
          bool retval = false;

          if (register_with_ns_)
            {
              retval = register_with_ns (orb.in (), daemon.in ());
              if (!retval)
                return -1;
            }


          if (write_to_ior_)
            {
              retval = write_ior_file (orb.in (), daemon.in ());
              if (!retval)
                return -1;
            }

          // Activate POA manager
          PortableServer::POAManager_var mgr = root_poa->the_POAManager ();

          if (CORBA::is_nil (mgr.in ()))
            ACE_ERROR_RETURN ((LM_ERROR,
                               "(%P|%t) CIAO_ExecutionManager: "
                               "Nil POA Manager error, returning \n"),
                              -1);

          mgr->activate ();

          // End Deployment part
          ACE_DEBUG ((LM_DEBUG,
                      "CIAO_ExecutionManager is running...\n"));

          if (pid_file_name_)
            {
              retval = write_pid ();
              if (!retval)
                return -1;
            }

          // Run the main event loop for the ORB.
          orb->run ();

          // Forget the pointer. The POA will take care of it during
          // destroy.
          (void) de.release ();

          root_poa->destroy (1, 1);

          orb->destroy ();
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception ("CIAO_ExecutionManager::main\n");
          return -1;
        }

      ACE_DEBUG ((LM_DEBUG,
                  "CIAO_ExecutionManager has closed\n"));
      return 0;
    }

  }
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  return CIAO::Execution_Manager::run_main (argc, argv);
}
