// $Id$

/**
 * @file ComponentServer.cpp
 *
 * @brief CIAO's ComponentServer implementation
 *
 */

#include "ComponentServer_Impl.h"
#include "CIAO_ServersC.h"
#include "Server_init.h"
#include "ace/SString.h"
#include "ace/Get_Opt.h"

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
                           "-n Don't not try to callback ServerActivator (testing)\n"
                           "-o <ior_output_file>\n"
                           "-k <activator_callback_ior>\n"
                           "\n",
                           argv [0]),
                          -1);
      }

  if (use_callback && callback_ior_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Callback IOR to ServerActivator is required.\n"),
                      -1);

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

      CIAO::ComponentServer_Impl *comserv_servant;

      ACE_NEW_RETURN (comserv_servant,
                      CIAO::ComponentServer_Impl (orb.in (),
                                                  poa.in ()),
                      -1);

      PortableServer::ServantBase_var safe_servant (comserv_servant);

      // @@ We need to call ComponentServer servant's init method.
      // But it's not sure to me where exactly we can get the
      // ConfigValues needed by the init method at this moment.

      // comserv_servant->init (config ACE_ENV_ARG_PARAMETER);

      // Configuring ComponentServer.
      PortableServer::ObjectId_var cs_oid
        = poa->activate_object (comserv_servant
                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      obj = poa->id_to_reference (cs_oid.in ()
                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Components::Deployment::ComponentServer_var comserv_obj =
        Components::Deployment::ComponentServer::_narrow (obj.in ()
                                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (comserv_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "Unable to activate ComponentServer object\n"), -1);


      Components::Deployment::ServerActivator_var activator;
      Components::ConfigValues_var config;

      if (use_callback)
        {
          obj = orb->string_to_object (callback_ior_
                                       ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CIAO::Activator_Callback_var act_callback
            = ::CIAO::Activator_Callback::_narrow (obj.in ()
                                                   ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          Components::ConfigValues_out config_out (config.out ());

          activator
            = act_callback->register_component_server (comserv_obj.in (),
                                                       config_out
                                                       ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      comserv_servant->set_objref (activator.in (),
                                   config.in (),
                                   comserv_obj.in ()
                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var str = orb->object_to_string (comserv_obj.in ()
                                                     ACE_ENV_ARG_PARAMETER);

      CIAO::Utility::write_IOR (ior_file_name_, str.in ());
      ACE_DEBUG ((LM_INFO, "ComponentServer IOR: %s\n", str.in ()));

      // End Deployment part

      ACE_DEBUG ((LM_DEBUG,
                  "Running ComponentServer...\n"));

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
