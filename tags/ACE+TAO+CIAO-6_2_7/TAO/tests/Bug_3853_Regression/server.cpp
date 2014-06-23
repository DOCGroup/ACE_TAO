// $Id$
#include "Hello_i.h"
#include "HelloC.h"

#include "ace/Task.h"
#include "ace/Get_Opt.h"
#include <fstream>
#include <iostream>

const ACE_TCHAR *ior_output_file = ACE_TEXT("");
const ACE_TCHAR *shutdown_ior_output_file = ACE_TEXT("");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:s:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case 's':
        shutdown_ior_output_file = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "-s <shutdown_iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }

  // Indicates successful parsing of the command line
  return 0;
}


int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
  {
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

    if (parse_args (argc, argv) != 0)
      return 1;

    CORBA::Object_var poa_object =  orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var rootPOA =  PortableServer::POA::_narrow (poa_object.in ());
    PortableServer::POAManager_var poa_manager =  rootPOA->the_POAManager ();

    // Policies for the firstPOA to be created.
    CORBA::PolicyList policies (5);
    policies.length (5);
    // Lifespan policy
    policies[0] =
      rootPOA->create_lifespan_policy (PortableServer::PERSISTENT );

    // Servant Retention Policy
    policies[1] =
      rootPOA->create_servant_retention_policy (PortableServer::RETAIN );

    // ID Assignment Policy
    policies[2] =
      rootPOA->create_id_assignment_policy (PortableServer::USER_ID );

    // Request Processing Policy
    policies[3] =
      rootPOA->create_request_processing_policy (PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY);

    // Threading policy
    policies[4] =
      rootPOA->create_thread_policy (PortableServer::ORB_CTRL_MODEL);

    PortableServer::POA_var demoPOA
      = rootPOA->create_POA ("HelloWorldServer",
                              poa_manager.in (),
                              policies);

    for (CORBA::ULong i = 0;
        i < policies.length ();
        ++i)
    {
      CORBA::Policy_ptr policy = policies[i];
      policy->destroy ();
    }

    // Create object for shutdown commanded by client.
    {
        // create the object
        Demo_HelloWorld_i * hello = new Demo_HelloWorld_i(orb.in());

        // Get the Object ID.
        PortableServer::ObjectId_var oid =  PortableServer::string_to_ObjectId ("shutdown");
        demoPOA->activate_object_with_id (oid.in (),  hello);


        // Create an object reference.
        CORBA::Object_var myhello = demoPOA->id_to_reference(oid.in());

        // Put the object reference as an IOR string
        ofstream out(ACE_TEXT_ALWAYS_CHAR (shutdown_ior_output_file));
        CORBA::String_var ior = orb->object_to_string (myhello.in ());
        out  << ior.in();
        // save the reference into a file
        out.close();
    }

    // Create object to handle client sayHello requests.
    {
        // create the object
        Demo_HelloWorld_i * hello = new Demo_HelloWorld_i(orb.in());

        // Get the Object ID.
        PortableServer::ObjectId_var oid =  PortableServer::string_to_ObjectId ("hello");
        demoPOA->activate_object_with_id (oid.in (),  hello);

        // Create an object reference.
        CORBA::Object_var myhello = demoPOA->id_to_reference(oid.in());

        // Put the object reference as an IOR string
        ofstream out(ACE_TEXT_ALWAYS_CHAR (ior_output_file));
        CORBA::String_var ior = orb->object_to_string (myhello.in ());
        out  << ior.in();
        // save the reference into a file
        out.close();
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////
    poa_manager->activate ();

    std::cout << ior_output_file << " is ready " << std::endl;
    orb->run ();

    // Destroy the POA, waiting until the destruction terminates
    rootPOA->destroy (1, 1);
    orb->destroy ();
  }
  catch (const CORBA::Exception &e)
  {
    std::cerr << "Unexpected exception: " << e << std::endl;
    return 1;
  }

  return 0;
}
