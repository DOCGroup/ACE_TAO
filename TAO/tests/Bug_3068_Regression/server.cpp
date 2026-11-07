#include <cstdio>
#include <memory>

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "test_i.h"
#include "ace/Task.h"

#include "tao/Messaging/Messaging.h"
#include "tao/BiDir_GIOP/BiDirGIOP.h"
#include "tao/Utils/PolicyList_Destroyer.h"

int
parse_args (int argc, ACE_TCHAR *argv[], ACE_TCHAR const *&ior_output_file)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("usage: %s -o <iorfile>\n"), argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_TCHAR const *ior_output_file {};
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var const object =
        orb->resolve_initial_references ("ORBPolicyManager");

      CORBA::PolicyManager_var const policy_mgr =
        CORBA::PolicyManager::_narrow (object.in ());
      if (CORBA::is_nil (policy_mgr.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "failed to get policy manager\n"), 1);
      CORBA::String_var const ref_id = policy_mgr->_interface_repository_id ();
      ACE_DEBUG ((LM_DEBUG, "Policy MGR intid = %C\n", ref_id.in ()));

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());
      PortableServer::POAManager_var const poa_manager = root_poa->the_POAManager ();

      PortableServer::POA_var child_poa;
      {
        // Policies for the childPOA to be created.
        TAO::Utils::PolicyList_Destroyer policies (1);
        policies.length (1);

        CORBA::Any pol;
        pol <<= BiDirPolicy::BOTH;
        policies[0] =
          orb->create_policy (BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE, pol);

        // Create the child POA so callbacks use their existing connection.
        child_poa = root_poa->create_POA ("child", poa_manager.in (), policies);
      }

      poa_manager->activate ();

      if (parse_args (argc, argv, ior_output_file) != 0)
        return 1;

      // The POA borrows this stack servant until root_poa->destroy() completes.
      Simple_Server_i server_impl (orb.in ());

      PortableServer::ObjectId_var const id =
        PortableServer::string_to_ObjectId ("simple_server");

      child_poa->activate_object_with_id (id.in (), &server_impl);

      CORBA::Object_var const obj = child_poa->id_to_reference (id.in ());

      CORBA::String_var const ior = orb->object_to_string (obj.in ());

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Server activated as <%C>\n", ior.in ()));

      // If the ior_output_file exists, output the ior to it
      if (ior_output_file != nullptr)
        {
          std::unique_ptr<FILE, int (*) (FILE *)> output_file (
            ACE_OS::fopen (ior_output_file, "w"), ACE_OS::fclose);
          if (output_file == nullptr)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %C",
                               ior_output_file),
                              1);
          ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
        }

      orb->run ();

      ACE_Thread_Manager::instance ()->wait ();

      root_poa->destroy (true, true);
    }
  catch (CORBA::Exception const &excep)
    {
      excep._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}
