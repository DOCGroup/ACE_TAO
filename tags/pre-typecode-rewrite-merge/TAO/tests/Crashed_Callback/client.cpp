// $Id$

#include "Crashed_Callback.h"

#include "tao/Messaging/Messaging.h"
#include "ace/Get_Opt.h"

ACE_RCSID(Crashed_Callback, client, "$Id$")

const char *ior = "file://test.ior";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Make all oneways "reliable."
      {
        CORBA::Object_var manager_object =
          orb->resolve_initial_references("ORBPolicyManager"
                                          ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
        CORBA::PolicyManager_var policy_manager =
          CORBA::PolicyManager::_narrow(manager_object.in()
                                        ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        if (CORBA::is_nil (policy_manager.in ()))
          ACE_ERROR_RETURN ((LM_ERROR,
                             " (%P|%t) Panic: nil PolicyManager\n"),
                            1);
        CORBA::Any policy_value;
        policy_value <<= Messaging::SYNC_WITH_SERVER;
        CORBA::PolicyList policies(1); policies.length(1);
        policies[0] =
          orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                              policy_value
                              ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        policy_manager->set_policy_overrides (policies,
                                              CORBA::ADD_OVERRIDE
                                              ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        policies[0]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test::Service_var service =
        Test::Service::_narrow(tmp.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (service.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil service reference <%s>\n",
                             ior),
                            1);
        }

      Crashed_Callback *crashed_callback_impl;
      ACE_NEW_RETURN (crashed_callback_impl,
                      Crashed_Callback,
                      1);
      PortableServer::ServantBase_var owner_transfer(crashed_callback_impl);

      Test::Crashed_Callback_var crashed_callback =
        crashed_callback_impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      service->run_test (crashed_callback.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      root_poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
