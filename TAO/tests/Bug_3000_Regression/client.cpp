#include "Callback.h"

#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/Utils/PolicyList_Destroyer.h"
#include "ace/Get_Opt.h"

int
parse_args (int argc,
            ACE_TCHAR *argv[],
            ACE_TCHAR const *&ior,
            bool &shutdown_server)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:s"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
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
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_TCHAR const *ior = ACE_TEXT ("file://test.ior");
  bool shutdown_server {};
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      // Make all oneways "reliable."
      {
        CORBA::Object_var const manager_object =
          orb->resolve_initial_references ("ORBPolicyManager");
        CORBA::PolicyManager_var const policy_manager =
          CORBA::PolicyManager::_narrow (manager_object.in ());

        if (CORBA::is_nil (policy_manager.in ()))
          ACE_ERROR_RETURN ((LM_ERROR,
                             " (%P|%t) Panic: nil PolicyManager\n"),
                            1);
        CORBA::Any policy_value;
        policy_value <<= Messaging::SYNC_WITH_SERVER;
        TAO::Utils::PolicyList_Destroyer policies (1);
        policies.length (1);
        policies[0] =
          orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                              policy_value);

        policy_manager->set_policy_overrides (policies,
                                              CORBA::ADD_OVERRIDE);

      }

      if (parse_args (argc, argv, ior, shutdown_server) != 0)
        return 1;

      CORBA::Object_var const tmp =
        orb->string_to_object (ior);

      Test::Service_var const service =
        Test::Service::_narrow (tmp.in ());

      if (CORBA::is_nil (service.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil service reference <%s>\n",
                             ior),
                            1);
        }

      // ServantBase_var owns this servant; the raw pointer is borrowed for the callback check.
      Callback *callback_impl {};
      ACE_NEW_RETURN (callback_impl,
                      Callback (),
                      1);
      PortableServer::ServantBase_var owner_transfer (callback_impl);

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (callback_impl);

      CORBA::Object_var const object = root_poa->id_to_reference (id.in ());

      Test::Callback_var const callback =
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
      if (shutdown_server)
        service->shutdown ();

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
