#include "Callback.h"

#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"
#include "ace/Get_Opt.h"

#include <string>

static int
parse_args (int argc, ACE_TCHAR *argv[], std::basic_string<ACE_TCHAR> &ior)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s "
                                     "-k <ior>"
                                     "\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      std::basic_string<ACE_TCHAR> ior {ACE_TEXT ("file://test.ior")};
      CORBA::ORB_var const orb {CORBA::ORB_init (argc, argv)};

      CORBA::Object_var const poa_object {orb->resolve_initial_references ("RootPOA")};

      PortableServer::POA_var const root_poa {PortableServer::POA::_narrow (poa_object.in ())};

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var const poa_manager {root_poa->the_POAManager ()};

      // Make all oneways "reliable."
      {
        CORBA::Object_var const manager_object {orb->resolve_initial_references ("ORBPolicyManager")};
        CORBA::PolicyManager_var const policy_manager {
          CORBA::PolicyManager::_narrow (manager_object.in ())};

        if (CORBA::is_nil (policy_manager.in ()))
          ACE_ERROR_RETURN ((LM_ERROR,
                             " (%P|%t) Panic: nil PolicyManager\n"),
                            1);
        CORBA::Any policy_value {};
        policy_value <<= Messaging::SYNC_WITH_SERVER;
        CORBA::PolicyList policies (1);
        policies.length (1);
        policies[0] =
          orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                              policy_value);

        policy_manager->set_policy_overrides (policies,
                                              CORBA::ADD_OVERRIDE);

        policies[0]->destroy ();
      }

      if (parse_args (argc, argv, ior) != 0)
        return 1;

      CORBA::Object_var const tmp {
        orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (ior.c_str ()))};

      Test::Service_var const service {Test::Service::_narrow (tmp.in ())};

      if (CORBA::is_nil (service.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             ACE_TEXT ("Nil service reference <%s>\n"),
                             ior.c_str ()),
                            1);
        }

      Callback *callback_impl {};
      ACE_NEW_RETURN (callback_impl,
                      Callback (orb.in ()),
                      1);
      // The servant var adopts this allocation and keeps it alive through POA activation.
      PortableServer::ServantBase_var owner_transfer (callback_impl);

      PortableServer::ObjectId_var const id {root_poa->activate_object (callback_impl)};

      CORBA::Object_var const object {root_poa->id_to_reference (id.in ())};

      Test::Callback_var const callback {Test::Callback::_narrow (object.in ())};

      poa_manager->activate ();

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) client - starting test\n"));

      service->run_test (callback.in ());

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) client - running ORB\n"));

      orb->run ();

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
