//
// $Id$
//

#include "Stock_Factory_i.h"
#include <orbsvcs/CosNamingC.h>
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

    // Create the servant
    Quoter_Stock_Factory_i stock_factory_i;

    // Activate it to obtain the object reference
    Quoter::Stock_Factory_var stock_factory =
      stock_factory_i._this ();

    // Get the Naming Context reference
    CORBA::Object_var naming_context_object =
      orb->resolve_initial_references ("NameService");
    CosNaming::NamingContext_var naming_context =
      CosNaming::NamingContext::_narrow (naming_context_object.in ());

    // Create and initialize the name.
    CosNaming::Name name (1);
    name.length (1);
    name[0].id = CORBA::string_dup ("Stock_Factory");

    // Bind the object
    naming_context->rebind (name, stock_factory.in ());

    // Resolve the Event Service
    name[0].id = CORBA::string_dup ("CosEventService");

    CORBA::Object_var ec_object =
      naming_context->resolve (name);

    // Now downcast the object reference to the appropriate type
    CosEventChannelAdmin::EventChannel_var ec =
      CosEventChannelAdmin::EventChannel::_narrow (ec_object.in ());

    CosEventChannelAdmin::SupplierAdmin_var supplier_admin =
      ec->for_suppliers ();

    stock_factory_i.load_stock_objects (poa.in (),
                                        poa_manager.in (),
                                        supplier_admin.in ());

    // ****************************************************************

    for (int j = 0; j != 1000; ++j) {
      for (int i = 1; i != argc; ++i) {
        try {
          // Get the stock object
          Quoter::Stock_var stock =
            stock_factory->get_stock (argv[i]);

          CORBA::String_var full_name = stock->full_name ();

          // Get the price
          CORBA::Double price = stock->price ();

          Quoter::Modify_Stock_var modify_stock =
            Quoter::Modify_Stock::_narrow (stock.in ());
          modify_stock->set_price (price + 1);

          cout << "Set the price of "
               << full_name.in ()
               << " to " << price + 1 << endl;
        }
        catch (Quoter::Invalid_Stock_Symbol &) {
          cerr << "Invalid stock symbol <"
                    << argv[i] << ">" << endl;
        }
        ACE_Time_Value tv (0, 500000);
        ACE_OS::sleep (tv);
      }
    }

    stock_factory_i.destroy_stock_objects ();

    // Destroy the POA, waiting until the destruction terminates
    poa->destroy (1, 1);
    orb->destroy ();
  }
  catch (CORBA::Exception &) {
    cerr << "CORBA exception raised!" << endl;
  }
  return 0;
}
