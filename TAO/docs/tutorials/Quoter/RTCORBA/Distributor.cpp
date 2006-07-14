// $Id$

// local header files
#include "Distributor_i.h"

// ACE header files
#include <ace/streams.h>

// TAO headers
#include <tao/RTCORBA/RTCORBA.h>

//
// main
//
int main (int argc, char *argv[])
{
  try {
    // Initalize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

    // Get the RootPOA.
    CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow (obj.in ());
    ACE_ASSERT (!CORBA::is_nil (obj));

    // Activate the POAManager.
    PortableServer::POAManager_var mgr = poa->the_POAManager ();
    mgr->activate ();

    // Create the factory object. Create a <Stock::StockDistributor>
    Stock_StockDistributorHome_i stock_distributor_home (orb);
    Stock::StockDistributor_var stock_distributor = stock_distributor_home.create ();	
    // @@ Shanshan - Yikes!  Please don't use ACE_ASSERT for this!  Use an if
    // statement.
    ACE_ASSERT (!CORBA::is_nil (stock_distributor));

    // Write the object reference for the <stock_distributor> to a file
    // so the <StockBroker> can read it when it's bootstrapping.
    CORBA::String_var str = orb->object_to_string (stock_distributor.in ());
    ofstream outfile("StockDistributor.ior");

    outfile << str << endl;
    outfile.close();

    // Set the rate of the <stock_distributor>.
    // @@ Shanshan - please use ACE_DEBUG instead of cout.
    cout << "Enter notification rate (sec): ";
    CORBA::Long rate; cin >> rate;

    ACE_DEBUG ((LM_DEBUG, "*** message: setting notification rate...\n"));
    stock_distributor->notification_rate (rate);

    // Enter into the event looping.
    ACE_DEBUG ((LM_DEBUG, "*** message: ready for transmission...\n"));
    orb->run ();
    orb->destroy ();
  }
  catch (CORBA::Exception &ex) {
    cerr << "CORBA exception: " << ex << endl;
    return 1;
  }
  return 0;
}
