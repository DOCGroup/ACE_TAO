#include "Service.h"

#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include <memory>

int
parse_args (int argc, ACE_TCHAR *argv[], ACE_TString &ior_output_file)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
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
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      ACE_TString ior_output_file (ACE_TEXT("test.ior"));
      if (parse_args (argc, argv, ior_output_file) != 0)
        return 1;

      CORBA::Object_var const poa_object =
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
        CORBA::Object_var const manager_object =
          orb->resolve_initial_references("ORBPolicyManager");
        CORBA::PolicyManager_var const policy_manager =
          CORBA::PolicyManager::_narrow(manager_object.in());

        if (CORBA::is_nil (policy_manager.in ()))
          ACE_ERROR_RETURN ((LM_ERROR,
                             " (%P|%t) Panic: nil PolicyManager\n"),
                            1);
        CORBA::Any policy_value;
        policy_value <<= Messaging::SYNC_WITH_SERVER;
        CORBA::PolicyList policies(1);
        policies.length(1);
        policies[0] =
          orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                              policy_value);

        policy_manager->set_policy_overrides (policies,
                                              CORBA::ADD_OVERRIDE);

        policies[0]->destroy ();
      }

      std::unique_ptr<Service> service_impl = std::make_unique<Service> ();

      service_impl->orb_ = CORBA::ORB::_duplicate (orb.in());
      // This non-owning alias remains valid while owner_transfer owns the servant.
      Service * const service_impl_alias = service_impl.get ();
      PortableServer::ServantBase_var owner_transfer (service_impl.release ());

      PortableServer::ObjectId_var id =
        root_poa->activate_object (owner_transfer.in ());

      CORBA::Object_var const object = root_poa->id_to_reference (id.in ());

      Test::Service_var const service =
        Test::Service::_narrow (object.in ());

      CORBA::String_var const ior =
        orb->object_to_string (service.in ());

      // If the ior_output_file exists, output the ior to it
      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (ior_output_file.c_str (), "w"), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open output file for writing IOR: %s",
                             ior_output_file.c_str ()),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      poa_manager->activate ();

      ACE_Time_Value tv (50, 0);
      orb->run (tv);

      ACE_DEBUG ((LM_DEBUG, "Event loop finished\n"));

      service_impl_alias->dump_results ();

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
