// $Id$

#include "Callback.h"

#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
bool shutdown_server = false;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:s"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 's':
        shutdown_server = true;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior>"
                           "-s "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      // Make all oneways "reliable."
      {
        CORBA::Object_var manager_object =
          orb->resolve_initial_references("ORBPolicyManager");
        CORBA::PolicyManager_var policy_manager =
          CORBA::PolicyManager::_narrow(manager_object.in());

        if (CORBA::is_nil (policy_manager.in ()))
          ACE_ERROR_RETURN ((LM_ERROR,
                             " (%P|%t) Panic: nil PolicyManager\n"),
                            1);
        CORBA::Any policy_value;
        policy_value <<= Messaging::SYNC_WITH_SERVER;
        CORBA::PolicyList policies(1); policies.length(1);
        policies[0] =
          orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                              policy_value);

        policy_manager->set_policy_overrides (policies,
                                              CORBA::ADD_OVERRIDE);

        policies[0]->destroy ();
      }

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior);

      Test::Service_var service =
        Test::Service::_narrow(tmp.in ());

      if (CORBA::is_nil (service.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil service reference <%s>\n",
                             ior),
                            1);
        }

      Callback *callback_impl;
      ACE_NEW_RETURN (callback_impl,
                      Callback (),
                      1);
      PortableServer::ServantBase_var owner_transfer(callback_impl);

      PortableServer::ObjectId_var id =
        root_poa->activate_object (callback_impl);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      Test::Callback_var callback =
        Test::Callback::_narrow (object.in ());

      poa_manager->activate ();

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) client - starting test\n"));

      service->run_test (callback.in ());

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) client - running ORB\n"));

      while (!callback_impl->received_callback ())
        {
          orb->perform_work ();
        }
      if (shutdown_server) {
        service->shutdown ();
      }

      root_poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
