//
// $Id$
//

#include "Stock_Factory_i.h"

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

    // Create the servant
    Quoter_Stock_Factory_i stock_factory_i;

    // Activate it to obtain the object reference
    Quoter::Stock_Factory_var stock_factory =
      stock_factory_i._this ();

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
