// $Id$

// local header files
#include "Broker_i.h"
#include "Distributor_i.h"
#include "Stock_PriorityMapping.h"

// ACE header files
#include "ace/streams.h"
#include "ace/Get_Opt.h"

// STL headers
#include <cstdlib>

static const char *ior = "file://StockDistributor.ior";
static const char *priority_level = "2";
static const char *stock_name = "IBM";

int parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:p:n:");
  int c;

  while ((c = get_opts ()) != -1)
  {
    switch (c)
    {
      case 'o':
        ior = get_opts.opt_arg ();
        break;
      case 'p':
        priority_level = get_opts.opt_arg ();
        break;
      case 'n':
        stock_name = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                          "usage: %s\n"
                          "-o <Distributor IOR> (default is file://StockDistributor.ior) \n"
                          "-p <Priority> (default is 2) \n"
                          "-n <Stock name> (default is IBM) \n"
                          "\n",
                          argv [0]),
                          -1);
    }
  }

  return 0;
}

int main (int argc, char *argv[])
{
  if (parse_args (argc, argv) != 0)
    return 1;

  try
  {
    // Initialiaze the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

    // Get a reference to the RootPOA.
    CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow (obj);

    // Activate the POAManager.
    PortableServer::POAManager_var mgr = poa->the_POAManager ();
    mgr->activate ();

    // Create an instance of the <StockBroker>.
    static const RTCORBA::Priority priority = atoi (priority_level);
    Stock_StockBrokerHome_i stock_broker_home (orb, stock_name, priority);
    Stock::StockBroker_var stock_broker = stock_broker_home.create ();
    if (CORBA::is_nil (stock_broker.in ()))
    {
      ACE_ERROR_RETURN ((LM_DEBUG,
                         "Nil StockBroker object reference <%s>\n",
                         ior),
                        1);
    }

    // Read and destringify the Stock_Distributor object's IOR.
    obj = orb->string_to_object (ior);

    // Narrow the IOR to a Stock_Distributor object reference.
    Stock::StockDistributor_var stock_distributor = Stock::StockDistributor::_narrow(obj);
    if (CORBA::is_nil (stock_distributor.in ()))
    {
      ACE_ERROR_RETURN ((LM_DEBUG,
                         "Nil StockDistributor object reference <%s>\n",
                         ior),
                        1);
    }

    // Connect the <stock_quoter> to the <stock_broker>.
    Stock::StockQuoter_var stock_quoter = stock_distributor->provide_quoter_info ();
    stock_broker->connect_quoter_info (stock_quoter);

    // Create a new consumer; initialize it; subscribe to the notification service.
    Stock::StockNameConsumer_var consumer = stock_broker->get_consumer_notifier ();
    Stock::Cookie *cookie = stock_distributor->subscribe_notifier (consumer, priority);

    // Run the event loop.
    ACE_DEBUG ((LM_DEBUG, "Ready to receieve stock information!\n"));
    orb->run ();
    orb->destroy ();
  }
  catch (CORBA::Exception &ex)
  {
    ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                         "Exception caught:");
    return 1;
  }
  return 0;
}
