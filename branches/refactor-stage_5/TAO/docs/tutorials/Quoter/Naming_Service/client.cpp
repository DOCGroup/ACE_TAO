//
// $Id$
//

#include "QuoterC.h"
#include <orbsvcs/CosNamingC.h>
#include "ace/streams.h"

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
    if (argc < 2) {
      cerr << "Usage: " << argv[0]
                << "symbol symbol..." << endl;
      return 1;
    }

    CORBA::Object_var naming_context_object =
      orb->resolve_initial_references ("NameService");
    CosNaming::NamingContext_var naming_context =
      CosNaming::NamingContext::_narrow (naming_context_object.in ());

    CosNaming::Name name (1);
    name.length (1);
    name[0].id = CORBA::string_dup ("Stock_Factory");

    CORBA::Object_var factory_object =
      naming_context->resolve (name);

    // Now downcast the object reference to the appropriate type
    Quoter::Stock_Factory_var factory =
      Quoter::Stock_Factory::_narrow (factory_object.in ());

    // Now get the full name and price of the other arguments:
    for (int i = 1; i != argc; ++i) {
      try {
        // Get the stock object
        Quoter::Stock_var stock =
          factory->get_stock (argv[i]);

        // Get its name, put it on a _var so it is automatically
        // released!
        CORBA::String_var full_name = stock->full_name ();

        // Now get the price
        CORBA::Double price = stock->price ();

        cout << "The price of a stock in \""
             << full_name.in () << "\" is $"
             << price << endl;
      }
      catch (Quoter::Invalid_Stock_Symbol &) {
        cerr << "Invalid stock symbol <"
             << argv[i] << ">" << endl;
      }
    }

    // Finally destroy the ORB
    orb->destroy ();
  }
  catch (CORBA::Exception &) {
    cerr << "CORBA exception raised!" << endl;
  }
  return 0;
}
