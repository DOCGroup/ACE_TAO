// Include the generated names....
#include "QuoterC.h"
#include "ace/streams.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try {
    // First initialize the ORB, that will remove some arguments...
    CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

    // There must be at least two arguments, the first is the factory
    // name, the rest are the names of the stock symbols we want to
    // get quotes for.
    if (argc < 3) {
      cerr << "Usage: " << argv[0] << " Factory_IOR symbol symbol..." << endl;
      return 1;
    }

    // Use the first argument to create the factory object reference,
    // in real applications we use the naming service, but let's do
    // the easy part first!
    CORBA::Object_var const factory_object = orb->string_to_object (argv[1]);

    // Now downcast the object reference to the appropriate type
    Quoter::Stock_Factory_var const factory = Quoter::Stock_Factory::_narrow (factory_object.in ());

    // Now get the full name and price of the other arguments:
    for (int i = 2; i != argc; ++i) {
      try {
        // Get the stock object
        Quoter::Stock_var const stock = factory->get_stock (ACE_TEXT_ALWAYS_CHAR (argv[i]));

        // Get its name, put it on a _var so it is automatically
        // released!
        CORBA::String_var const full_name = stock->full_name ();

        // Now get the price
        CORBA::Double const price = stock->price ();

        Quoter::Stock::StockHistory_var const history = stock->history ();

        cout << "The price of a stock in \"" << full_name.in () << "\" is $" << price << endl;
        cout << "  history: " << history[0] << " " << history[1] << " ... " << history[history->length()-1] << endl;
      } catch (Quoter::Invalid_Stock_Symbol const &) {
        cerr << "Invalid stock symbol <" << argv[i] << ">" << endl;
      }
    }

    // Finally destroy the ORB
    orb->destroy ();
  }
  catch (CORBA::Exception const &e) {
    cerr << "CORBA exception raised: " << e << endl;
  }
  return 0;
}
