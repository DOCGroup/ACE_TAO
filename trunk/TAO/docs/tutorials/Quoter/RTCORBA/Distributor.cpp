// $Id$

// local header files
#include "Distributor_i.h"

// ACE header files
#include "ace/streams.h"
#include "ace/Get_Opt.h"

// TAO headers
#include <tao/RTCORBA/RTCORBA.h>

static const char *ior = "StockDistributor.ior";

int parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:");
  int c;

  while ((c = get_opts ()) != -1)
  {
    switch (c)
    {
      case 'o':
        ior = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                          "usage: %s\n"
                          "-o <Distributor IOR> (default is file://StockDistributor.ior) \n"
                          "\n",
                          argv [0]),
                          -1);
    }
  }

  return 0;
}

int main (int argc, char *argv[])
{
  try
  {
    // Initalize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

    // Get the RootPOA.
    CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow (obj.in ());
    ACE_ASSERT (!CORBA::is_nil (obj));

    // Activate the POAManager.
    PortableServer::POAManager_var mgr = poa->the_POAManager ();
    mgr->activate ();

    // Create the factory object. Create a <Stock::StockDistributor>.
    Stock_StockDistributorHome_i stock_distributor_home (orb);
    Stock::StockDistributor_var stock_distributor = stock_distributor_home.create ();	
    if (CORBA::is_nil (stock_distributor.in ()))
    {
      ACE_ERROR_RETURN ((LM_DEBUG,
                         "Nil StockDistributor object reference <%s>\n",
                         ior),
                        1);
    }

    // Write the object reference for the <stock_distributor> to a file
    // so the <StockBroker> can read it when it's bootstrapping.
    CORBA::String_var str = orb->object_to_string (stock_distributor.in ());
    ofstream outfile(ior);
    outfile << str << endl;
    outfile.close();

    // Set the rate of the <stock_distributor>.
    ACE_DEBUG ((LM_DEBUG, "Enter notification rate (sec): "));
    CORBA::Long rate; cin >> rate;

    ACE_DEBUG ((LM_DEBUG, "*** message: setting notification rate...\n"));
    stock_distributor->notification_rate (rate);

    // Enter into the event looping.
    ACE_DEBUG ((LM_DEBUG, "*** message: ready for transmission...\n"));
    orb->run ();
    orb->destroy ();
  }
  catch (CORBA::Exception &ex)
  {
    ACE_PRINT_EXCEPTION (ex, "Admin: ");

    return 1;
  }
  return 0;
}
