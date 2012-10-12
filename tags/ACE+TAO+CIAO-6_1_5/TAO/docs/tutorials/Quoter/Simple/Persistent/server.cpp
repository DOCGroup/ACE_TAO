
//=============================================================================
/**
 *  @file     server.cpp
 *
 *  $Id$
 *
 *
 *   In this example, a new POA (childPOA) is created and its
 *   policy is set so that it object references are persistent.
 *
 *
 *  @author  Priyanka Gontla <pgontla@ece.uci.edu>
 */
//=============================================================================


#include "Stock_Factory_i.h"
#include "ace/streams.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try {

    // Initialze the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

    // Get a reference to the RootPOA.
    CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");

    // Get the POA_var object from Object_var.
    PortableServer::POA_var root_poa =
      PortableServer::POA::_narrow (obj.in ());

    // Get the POAManager of the RootPOA.
    PortableServer::POAManager_var poa_manager =
      root_poa->the_POAManager ();

    poa_manager->activate ();

    // Create a USER_ID IdAssignmentpolicy object.
    PortableServer::IdAssignmentPolicy_var idassignment =
      root_poa->create_id_assignment_policy (PortableServer::USER_ID);

    // Create a PERSISTENT LifespanPolicy object.
    PortableServer::LifespanPolicy_var lifespan =
      root_poa->create_lifespan_policy (PortableServer::PERSISTENT);

    // Policies for the childPOA to be created.
    CORBA::PolicyList policies;
    policies.length (2);

    policies[0] =
      PortableServer::IdAssignmentPolicy::_duplicate (idassignment.in ());

    policies[1] =
      PortableServer::LifespanPolicy::_duplicate (lifespan.in ());

    // Create the childPOA under the RootPOA.
    PortableServer::POA_var child_poa =
      root_poa->create_POA ("childPOA",
                            poa_manager.in (),
                            policies);

    // Destroy policy objects.
    idassignment->destroy ();
    lifespan->destroy ();

    // Create an instance of class Quoter_Stock_Factory_i.
    Quoter_Stock_Factory_i stock_factory_i;

    // Get the Object ID.
    PortableServer::ObjectId_var oid =
      PortableServer::string_to_ObjectId ("Stock_Factory");

    // Activate the Stock_Factory object.
    child_poa->activate_object_with_id (oid.in (),
                                        &stock_factory_i);

    // Get the object reference.
    CORBA::Object_var stock_factory =
      child_poa->id_to_reference (oid.in ());

    // Stringify all the object referencs.
    CORBA::String_var ior = orb->object_to_string (stock_factory.in ());
    // Print them out !
    cout << ior.in () << endl;

    orb-> run ();

    // Destroy POA, waiting until the destruction terminates.
    root_poa->destroy (1, 1);
    orb->destroy ();
  }
  catch (CORBA::Exception &) {
    cerr << "CORBA exception raised !" << endl;
  }
  return 0;
}
