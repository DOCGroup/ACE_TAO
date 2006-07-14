// $Id$

// $Id$

// local header files
#include "DistributorC.h"

// ACE header files
#include <ace/streams.h>

// STL header files
#include <string>

void usage ()
{
  cou
    << "This is the administator program for the stock distributor. Use the following" << endl
    << "commands to control the behavior of the stock distributor: " << endl
    << endl
    << "    start     - start the stock distributor" << endl
    << "    stop      - stop the stock distributor" << endl
    << "    exit      - exit the administator program" << endl
    << "    rate n    - set the distribution rate to 'n' milliseconds" << endl;
}

//
// main
//
int main (int argc, char *argv[])
{
  // Print the usage to the client.
  usage ();
  CORBA::ORB_var orb;

  try {
    // Initialiaze the ORB.
    orb = CORBA::ORB_init (argc, argv);

    // Read and destringify the Stock_Distributor object's IOR.
    // @@ Shanshan - Please make the ior file name a command line argument.
    CORBA::Object_var obj = orb->string_to_object ("file://StockDistributor.ior");
    if (CORBA::is_nil (obj)) {
      cerr << "Could not get the Stock_Distributor IOR." << endl;
      return 1;
    }

    // Narrow the IOR to a Stock_Distributor object reference.
    Stock::StockDistributor_var stock_distributor = Stock::StockDistributor::_narrow(obj);
    if (CORBA::is_nil (stock_distributor)) {
      cerr << "IOR was not a Stock_Distributor object reference." << endl;
      return 1;
    }

    std::string cmd;

    do {
      // Prompt user for the next command.
      cout << "% > ";
      cin >> cmd;

      if (cmd == "start")
        // Start the distributor.
        stock_distributor->start ();
      else if (cmd == "stop")
        // Stop the distributor.
        stock_distributor->stop ();
      else if (cmd == "rate") {
        // Change the notification rate.
        CORBA::Long rate;	
        cin >> rate;
        stock_distributor->notification_rate (rate);
      }
      else if (cmd == "exit")
        // Exit the application.	
        ;
      else
        cerr << "*** error: unrecognized command" << endl;
    } while (cmd != "exit");

    orb->destroy ();
  }
  catch (CORBA::Exception &ex) {
    cerr << "CORBA exception: " << ex << endl;
    orb->destroy ();
  }
  return 1;
}
