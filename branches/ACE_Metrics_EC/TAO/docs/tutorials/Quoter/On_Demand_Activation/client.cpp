//
// $Id$
//

#include "Handler_i.h"

int main (int argc, char* argv[])
{
  try {
    // First initialize the ORB, that will remove some arguments...
    CORBA::ORB_var orb =
      CORBA::ORB_init (argc, argv,
                       "" /* the ORB name, it can be anything! */);

    // There must be at least two arguments, the first is the factory
    // name, the rest are the names of the stock symbols we want to
    // get quotes for.
    if (argc < 3) {
      cerr << "Usage: " << argv[0]
           << " Factory_IOR symbol symbol..." << endl;
      return 1;
    }

    CORBA::Object_var poa_object =
      orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var poa =
      PortableServer::POA::_narrow (poa_object.in ());
    PortableServer::POAManager_var poa_manager =
      poa->the_POAManager ();
    poa_manager->activate ();

    // Use the first argument to create the factory object reference,
    // in real applications we use the naming service, but let's do
    // the easy part first!
    CORBA::Object_var factory_object =
      orb->string_to_object (argv[1]);

    // Now downcast the object reference to the appropriate type
    Quoter::Stock_Factory_var factory =
      Quoter::Stock_Factory::_narrow (factory_object.in ());

    // Create and activate the handler...
    int response_count = 0;
    Single_Query_Stock_Handler_i handler_i (&response_count);
    Quoter::AMI_Single_Query_StockHandler_var handler =
      handler_i._this ();

    // Send all the requests, careful with error handling
    int request_count = 0;
    for (int i = 2; i != argc; ++i) {
      try {
        // Get the stock object
        Quoter::Stock_var tmp =
          factory->get_stock (argv[i]);
        Quoter::Single_Query_Stock_var stock =
          Quoter::Single_Query_Stock::_narrow (tmp.in ());
        if (CORBA::is_nil (stock.in ())) {
          cerr << "Cannot get single query interface for <"
               << argv[i] << ">" << endl;
        }

        stock->sendc_get_price_and_names (handler.in ());
        request_count++;
      }
      catch (Quoter::Invalid_Stock_Symbol &) {
        cerr << "Invalid stock symbol <"
             << argv[i] << ">" << endl;
      }
    }

    while (response_count < request_count
           && orb->work_pending ()) {
      orb->perform_work ();
    }

    // Destroy the POA, waiting until the destruction terminates
    poa->destroy (1, 1);
    orb->destroy ();
  }
  catch (CORBA::Exception &ex) {
    cerr << "CORBA exception raised!" << endl;
  }
  return 0;
}
