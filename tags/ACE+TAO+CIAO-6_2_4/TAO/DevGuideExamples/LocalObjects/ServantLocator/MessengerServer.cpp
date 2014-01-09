// $Id$

#include "MessengerLocator_i.h"
#include "MessengerC.h"

#include "tao/AnyTypeCode/TypeCode.h"
#include <iostream>
#include <fstream>
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT ("Messenger.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
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
  // Indicates successful parsing of the command line
  return 0;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv [])
{
  try {
    // Initialize the ORB
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    if (parse_args (argc, argv) != 0)
        return 1;

    // Get a reference to the POA
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var rootPOA = PortableServer::POA::_narrow (obj.in());

    // Active the POA Manager
    PortableServer::POAManager_var mgr = rootPOA->the_POAManager();
    mgr->activate();

    // Create the policies and assign them for the child POA
    CORBA::PolicyList policies(3);
    policies.length(3);

    policies[0] = rootPOA->create_id_assignment_policy(PortableServer::USER_ID);
    policies[1] = rootPOA->create_request_processing_policy(PortableServer::USE_SERVANT_MANAGER);
    policies[2] = rootPOA->create_servant_retention_policy(PortableServer::NON_RETAIN);

    // Create the POA with these policies
    PortableServer::POA_var childPOA = rootPOA->create_POA("childPOA", mgr.in(), policies);

    // Destroy the policy objects
    for (CORBA::ULong i = 0; i != policies.length(); ++i) {
      policies[i]->destroy();
    }

    // Create our Messenger's ServantLocator.
    PortableServer::ServantLocator_var locator = new Messenger_Locator_i;

    // Set the Servant Manager with the childPOA.
    childPOA->set_servant_manager(locator.in());

    // Get the object id for the user-created ID in the childPOA.
    PortableServer::ObjectId_var child_oid = PortableServer::string_to_ObjectId("Messenger");

    // Create the object without creating a servant.
    CORBA::Object_var messenger_obj =
      childPOA->create_reference_with_id(child_oid.in(), ::_tc_Messenger->id());

    // Put the object reference into an IOR string
    CORBA::String_var str = orb->object_to_string(messenger_obj.in());

    // Write the IOR string to a file
    std::ofstream iorFile(ACE_TEXT_ALWAYS_CHAR(ior_output_file)); // Throws exception if there's a problem.
    iorFile << str.in();
    iorFile.close();

    std::cout << "IOR written to the file " << ACE_TEXT_ALWAYS_CHAR(ior_output_file) << std::endl;

    // Accept requests from clients.
    orb->run();

    // Release resources
    rootPOA->destroy(true,true);
    orb->destroy();
  }
  catch(const CORBA::Exception& ex) {
    std::cerr << "Server Caught a CORBA::Exception: " << ex << std::endl;
    return 1;
  }

  return 0;
}


