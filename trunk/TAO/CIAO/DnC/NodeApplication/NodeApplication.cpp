// $Id$

/**
 * @file NodeApplication.cpp
 *
 * @brief CIAO's NodeApplication implementation
 *
 */

#include "NodeApplication_Impl.h"
#include "NOdeApplicatin_CallBackC.h"
#include "ciao/Server_init.h"
#include "ciao/CIAO_common.h"
#include "ace/SString.h"
#include "ace/Get_Opt.h"
#include "ace/Env_Value_T.h"
#include "ace/Sched_Params.h"
#include "ace/OS_NS_errno.h"

char *ior_file_name_ = 0;
char *callback_ior_ = 0;
int use_callback = 1;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "nk:o:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'n':
        use_callback = 0;
        break;

      case 'o':  // get the file name to write to
       ior_file_name_ = get_opts.opt_arg ();
       break;

      case 'k':  // get the activator callback IOR
       callback_ior_ = get_opts.opt_arg ();
       break;

      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s\n"
                           "-n Do not use Callback (for testing)\n"
                           "-o <ior_output_file>\n"
                           "-k <NodeApplicationManager_callback_ior>\n"
                           "\n",
                           argv [0]),
                          -1);
      }

  if (use_callback && callback_ior_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Callback IOR to NodeApplicationManager is required.\n"),
                      -1);

  return 0;
}

int
main (int argc, char *argv[])
{
  //@@ I will ignor this config done at WashU for now. --Tao
  // Define CIAO_FIFO_SCHED=1 to run component server in FIFO_SCHED class
  /* ACE_Env_Value<int> envar ("CIAO_FIFO_SCHED", 0);
  if (envar != 0)
    {
      int priority =
        (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO)
         + ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)) / 2;
      priority = ACE_Sched_Params::next_priority (ACE_SCHED_FIFO,
                                                  priority);
      // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.

      if (ACE_OS::sched_params (ACE_Sched_Params (ACE_SCHED_FIFO,
                                                  priority,
                                                  ACE_SCOPE_PROCESS)) != 0)
        {
          if (ACE_OS::last_error () == EPERM)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "ComponentServer (%P|%t): user is not superuser, "
                          "test runs in time-shared class\n"));
            }
          else
            ACE_ERROR ((LM_ERROR,
                        "ComponentServer (%P|%t): sched_params failed\n"));
        }
      }*/

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

      // Activate POA manager
      PortableServer::POAManager_var mgr
        = poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      mgr->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CIAO::NodeApplication_Impl *nodeapp_servant;

      ACE_NEW_RETURN (nodeapp_servant,
                      CIAO::NodeApplication_Impl (orb.in (),
                                                  poa.in ()),
                      -1);

      PortableServer::ServantBase_var safe_servant (nodeapp_servant);

      // CONFIGURING NodeApplication
      Portableserver::ObjectId_var nodeapp_oid
        = poa->activate_object (nodeapp_servant
                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      obj = poa->id_to_reference (nodeapp_oid.in ()
                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Deployment::NodeApplication_var nodeapp_obj =
        Deployment::NodeApplication::_narrow (obj.in ()
					      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (nodeapp_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "Unable to activate NodeApplication object\n"), -1);


      Deployment::NodeApplicationManager_var nodeapp_man;
      Deployment::Properties_var prop = new Deployment::Properties;

      if (use_callback)
        {
          obj = orb->string_to_object (callback_ior_
                                       ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CIAO::NodeApplication_Callback_var nam_callback
            = CIAO::NodeApplication_Callback::_narrow (obj.in ()
							 ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          Deployment::Properties_out properties_out (prop.out ());

          nodeapp_man
            = nam_callback->register_node_application (nodeapp_obj.in (),
                                                       properties_out
                                                       ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      /** @@ We need to call NodeApplication servant's init method.
       * But it's not sure to me where exactly we can get the
       * Properties needed by the init method at this moment. There are two
       * ways to set the property of the NodeApplication:
       * 1. call init remotely from NodeApplicationManager
       * 2. call init locally on the servant of NodeApplication.
       */

      nodeapp_servant->init ();
      ACE_TRY_CHECK;

      CORBA::String_var str = orb->object_to_string (nodeapp_obj.in ()
                                                     ACE_ENV_ARG_PARAMETER);

      CIAO::Utility::write_IOR (ior_file_name_, str.in ());

      // End Deployment part
      if (CIAO::debug_level () > 10)
        {
          ACE_DEBUG ((LM_INFO, "NodeApplication IOR: %s\n", str.in ()));

          ACE_DEBUG ((LM_DEBUG,
                      "Running NodeApplication...\n"));
        }

      // Run the main event loop for the ORB.
      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "NodeApp::main\t\n");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
