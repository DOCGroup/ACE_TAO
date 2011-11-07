// $Id$
#include "test_i.h"
#include "tao/ImR_Client/ImR_Client.h"
#include <ace/Task.h>
#include <ace/Get_Opt.h>

const ACE_TCHAR * ior_output_file = ACE_TEXT ("serverA.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:");
  int c;


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

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
  {
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
    if (parse_args (argc, argv) != 0)
    {
      return 1;
    }

    CORBA::Object_var object =
      orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var rootPOA =
      PortableServer::POA::_narrow (object.in ());
    PortableServer::POAManager_var poa_manager =
      rootPOA->the_POAManager ();

    CORBA::PolicyList policies (5);
    policies.length (5);

    // Lifespan policy
    policies[0] =
      rootPOA->create_lifespan_policy (PortableServer::PERSISTENT);

    // Servant Retention Policy
    policies[1] =
      rootPOA->create_servant_retention_policy (PortableServer::RETAIN );

    // ID Assignment Policy
    policies[2] =
      rootPOA->create_id_assignment_policy (PortableServer::USER_ID );

    // Request Processing Policy
    policies[3] =
      rootPOA->create_request_processing_policy (PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY );

    // Threading policy
    policies[4] =
      rootPOA->create_thread_policy (PortableServer::ORB_CTRL_MODEL);

    PortableServer::POA_var poa_a = rootPOA->create_POA ("poaA",
        poa_manager.in (),
        policies
        );
    PortableServer::POA_var poa_c = rootPOA->create_POA ("poaC",
        poa_manager.in (),
        policies
        );

    for (CORBA::ULong i = 0;
      i < policies.length ();
      ++i)
    {
      CORBA::Policy_ptr policy = policies[i];
      policy->destroy ();
    }

    Test_Time_i* time = new Test_Time_i();

    PortableServer::ObjectId_var oid =
      PortableServer::string_to_ObjectId ("Server_A");
    poa_a->activate_object_with_id (oid.in (), time);
    CORBA::Object_var time_obj = poa_a->id_to_reference(oid.in());
    CORBA::String_var ior =
      orb->object_to_string (time_obj.in ());

    poa_manager->activate ();

    // Output the IOR to the <ior_output_file>
    FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
    if (output_file == 0)
      ACE_ERROR_RETURN ((LM_ERROR,
      "Cannot open output file for writing IOR: %s",
      ior_output_file),
      1);
    ACE_OS::fprintf (output_file, "%s", ior.in ());
    ACE_OS::fclose (output_file);

    orb->run ();

    rootPOA->destroy (1, 1);
    orb->destroy ();
  }
  catch (const CORBA::Exception &ex)
  {
    ex._tao_print_exception ("Exception caught by serverA:");
    return 1;
  }

  return 0;
}


