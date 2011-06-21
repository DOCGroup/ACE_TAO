// $Id$
#include "test_i.h"
#include "ORB_Task.h"
#include "tao/ImR_Client/ImR_Client.h"
#include <ace/Task.h>
#include <ace/Get_Opt.h>
#include "ace/OS_NS_unistd.h"

const ACE_TCHAR * ior_output_file = ACE_TEXT ("serverB.ior");
// server_notify_delay between resolving RootPOA init ref and create_POA.
int server_notify_delay = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:l:");
  int c;


  while ((c = get_opts ()) != -1)
    switch (c)
      {
        case 'o':
          ior_output_file = get_opts.opt_arg ();
          break;
        case 'l':
          server_notify_delay = ACE_OS::atoi (get_opts.opt_arg ());
          break;
        case '?':
          default:
          ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile> -l <server_notify_delay>"
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

    ORB_Task worker (orb.in ());
    worker.activate (THR_NEW_LWP | THR_JOINABLE,
                      1);

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

    if (server_notify_delay > 0)
    {
      ACE_OS::sleep (server_notify_delay);
      ACE_DEBUG ((LM_DEBUG, "(%P|%t)ServerB Now register with IMR \n"));
    }

    PortableServer::POA_var poa_a = rootPOA->create_POA ("poaB",
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

    Test_Dummy_i* dummy = new Test_Dummy_i();

    PortableServer::ObjectId_var oid =
      PortableServer::string_to_ObjectId ("Server_B");
    poa_a->activate_object_with_id (oid.in (), dummy);
    CORBA::Object_var dummy_obj = poa_a->id_to_reference(oid.in());
    CORBA::String_var ior =
      orb->object_to_string (dummy_obj.in ());

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

    worker.wait ();

    rootPOA->destroy (1, 1);
    orb->destroy ();
  }
  catch (const CORBA::Exception &ex)
  {
    ex._tao_print_exception ("Exception caught by serverB:");
    return 1;
  }

  return 0;
}


