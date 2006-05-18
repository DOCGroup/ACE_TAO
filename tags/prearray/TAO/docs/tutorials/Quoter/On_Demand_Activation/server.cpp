//
// $Id$
//

#include "Stock_Factory_Locator_i.h"
#include "ace/streams.h"

int main (int argc, char* argv[])
{
  try {
    // First initialize the ORB, that will remove some arguments...
    CORBA::ORB_var orb =
      CORBA::ORB_init (argc, argv,
                       "" /* the ORB name, it can be anything! */);
    CORBA::Object_var poa_object =
      orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var poa =
      PortableServer::POA::_narrow (poa_object.in ());
    PortableServer::POAManager_var poa_manager =
      poa->the_POAManager ();
    poa_manager->activate ();

    CORBA::PolicyList policies (3);
    policies.length (3);

    // Assign the polices

    policies[0] =
        poa->create_id_assignment_policy (PortableServer::USER_ID);

    policies [1] =
      poa->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER);

    policies [2] =
      poa->create_servant_retention_policy (PortableServer::NON_RETAIN);

    // Create the POA with these policies
    PortableServer::POA_var child_poa =
      poa->create_POA ("childPOA",
                       poa_manager.in (),
                       policies);

    // Destroy the policy objects
    for (CORBA::ULong i = 0; i != policies.length (); ++i) {
      policies[i]->destroy ();
    }

    // Create a Stock_Factory_Locator
    PortableServer::ServantLocator_var servant_locator =
      new Quoter_Stock_Factory_Locator_i (orb.in ());

    // Set the SM with the childPOA
    child_poa->set_servant_manager (servant_locator.in ());

    PortableServer::ObjectId_var child_oid =
      PortableServer::string_to_ObjectId ("childFoo");

    CORBA::Object_var stock_factory =
      child_poa->create_reference_with_id (child_oid.in (),
                                           "IDL:Quoter/Stock_Factory:1.0");


    // Put the object reference as an IOR string
    CORBA::String_var ior = orb->object_to_string (stock_factory.in ());

    // Print it out!
    cout << ior.in () << endl;

    orb->run ();

    // Destroy the POA, waiting until the destruction terminates
    poa->destroy (1, 1);
    orb->destroy ();
  }
  catch (CORBA::Exception &) {
    cerr << "CORBA exception raised!" << endl;
  }
  return 0;
}
