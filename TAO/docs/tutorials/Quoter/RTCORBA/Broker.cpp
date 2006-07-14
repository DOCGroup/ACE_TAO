// $Id$

// local header files
#include "Broker_i.h"
#include "Distributor_i.h"
#include "Stock_PriorityMapping.h"

// ACE header files
#include <ace/streams.h>

// STL headers
#include <cstdlib>

//
// usage
//
void usage()
{
  cerr << "syntax:" << endl;
  cerr << "         ./broker stock_symbol [priority]" << endl;
  cerr << endl;
  cerr << "arguments:" << endl;
  cerr << "         stock_symbol      - symbol of the stock of interest" << endl;
  cerr << "         priority          - priority which to listen; [0,4]" << endl;
  cerr << endl;
  cerr << "NOTE: If you do not specify a priority, the default priority" << endl;
  cerr << "of 2 [MEDIUM] will be assigned to the broker." << endl;
}

//
// main
//
int main (int argc, char *argv[])
{
  if (argc < 2) {
    usage ();
    return 0;
  }

  try {
    // Initialiaze the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

    // Get a reference to the RootPOA.
    CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow (obj);

    // Activate the POAManager.
    PortableServer::POAManager_var mgr = poa->the_POAManager ();
    mgr->activate ();

    // @@ Shanshan - Be careful here, what if I don't pass any arguments to the program?
    // It might be better to use ACE_Get_Opt (please see me if you need an example) to parse your arguments.

    // Extract the command-line arguments.
    std::string stock_name (argv[1]);
    RTCORBA::Priority priority = argc > 2 ? priority = atoi(argv[2]) : Stock_PriorityMapping::MEDIUM;

    // Create an instance of the <StockBroker>.
    Stock_StockBrokerHome_i stock_broker_home (orb, argv[1], priority);
    Stock::StockBroker_var stock_broker = stock_broker_home.create ();
    ACE_ASSERT (!CORBA::is_nil (stock_broker));

    // Read and destringify the Stock_Distributor object's IOR.
    obj = orb->string_to_object ("file://StockDistributor.ior");
    ACE_ASSERT (!CORBA::is_nil (obj));

    // Narrow the IOR to a Stock_Distributor object reference.
    Stock::StockDistributor_var stock_distributor = Stock::StockDistributor::_narrow(obj);
    ACE_ASSERT (!CORBA::is_nil (stock_distributor));

    // Connect the <stock_quoter> to the <stock_broker>.
    Stock::StockQuoter_var stock_quoter = stock_distributor->provide_quoter_info ();
    stock_broker->connect_quoter_info (stock_quoter);

    // Create a new consumer; initialize it; subscribe to the notification service.
    Stock::StockNameConsumer_var consumer = stock_broker->get_consumer_notifier ();
    Stock::Cookie *cookie = stock_distributor->subscribe_notifier (consumer, priority);

    // @@ Shanshan - Please use ACE_DEBUG
    // Run the event loop.
    cerr << "*** message: Ready to receieve stock information..." << endl;
    orb->run ();
    orb->destroy ();
  }
  catch (CORBA::Exception &ex) {
    cerr << "CORBA exception: " << ex << endl;
  }
  return 1;
}
