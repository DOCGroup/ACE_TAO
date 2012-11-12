// $Id$

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "test_i.h"
#include "ace/Task.h"

#include "tao/Messaging/Messaging.h"
#include "tao/BiDir_GIOP/BiDirGIOP.h"

const ACE_TCHAR *ior_output_file = 0;
int iterations = 10;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:i:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case 'i':
        iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("usage: %s -o <iorfile>")
                           ACE_TEXT ("-i <iterations>\n"), argv [0]),
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
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var object =
        orb->resolve_initial_references ("ORBPolicyManager");

      CORBA::PolicyManager_var policy_mgr =
        CORBA::PolicyManager::_narrow (object.in ());
      if (CORBA::is_nil (policy_mgr.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "failed to get policy manager\n"), 1);
      CORBA::String_var ref_id = policy_mgr->_interface_repository_id ();
      ACE_DEBUG ((LM_DEBUG, "Policy MGR intid = %C\n", ref_id.in ()));

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());
      PortableServer::POAManager_var poa_manager = root_poa->the_POAManager ();

      // Policies for the childPOA to be created.
      CORBA::PolicyList policies (1);
      policies.length (1);

      CORBA::Any pol;
      pol <<= BiDirPolicy::BOTH;
      policies[0] =
        orb->create_policy (BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE, pol);

      // Create POA as child of RootPOA with the above policies.  This POA
      // will receive request in the same connection in which it sent
      // the request
      PortableServer::POA_var child_poa =
        root_poa->create_POA ("child", poa_manager.in (), policies);

      // Creation of childPOA is over. Destroy the Policy objects.
      for (CORBA::ULong i = 0; i < policies.length (); ++i)
        {
          policies[i]->destroy ();
        }

      poa_manager->activate ();

      if (parse_args (argc, argv) != 0)
        return 1;

      Simple_Server_i server_impl (orb.in (), iterations);

      PortableServer::ObjectId_var id =
        PortableServer::string_to_ObjectId ("simple_server");

      child_poa->activate_object_with_id (id.in (), &server_impl);

      CORBA::Object_var obj = child_poa->id_to_reference (id.in ());

      CORBA::String_var ior = orb->object_to_string (obj.in ());

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Server activated as <%C>\n", ior.in ()));

      // If the ior_output_file exists, output the ior to it
      if (ior_output_file != 0)
        {
          FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %C",
                               ior_output_file),
                              1);
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }

      orb->run();

      ACE_Thread_Manager::instance ()->wait ();

      root_poa->destroy (1, 1);
    }
  catch (CORBA::Exception &excep)
    {
      excep._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}
