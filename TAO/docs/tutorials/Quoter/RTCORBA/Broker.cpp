// $Id$

// ACE header files
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdlib.h"
#include "orbsvcs/CosNamingC.h"

// STL strings
#include <iostream>
#include <string>

// local header files
#include "Broker_i.h"
#include "Distributor_i.h"
#include "Stock_PriorityMapping.h"

using Stock::Priority_Mapping;

// Name of the file that stores the StockDistributor IOR.
static std::string ior = "file://StockDistributor.ior";

// Name of the StockDistributor registered with the Naming Service.
static std::string distributor_name = "StockDistributor";

// Default priority level for running the broker.
static Priority_Mapping::Priority priority_level
    = Priority_Mapping::MEDIUM;

// Default stock name.
static std::string stock_name = "IBM";

// A flag that indicates use of the Naming Service.
bool use_naming = false;

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:p:n:c"));
  int c;

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'o':
          ior = get_opts.opt_arg ();
          break;
        case 'p':
          switch (ACE_OS::atoi (get_opts.opt_arg ()))
            {
            case 0:
              priority_level = Priority_Mapping::VERY_LOW;
              break;

            case 1:
              priority_level = Priority_Mapping::LOW;
              break;

            case 2:
              priority_level = Priority_Mapping::MEDIUM;
              break;

            case 3:
              priority_level = Priority_Mapping::HIGH;
              break;

            case 4:
              priority_level = Priority_Mapping::VERY_HIGH;
              break;

            default:
              std::cerr << "Warning: Invalid priority detected, defaulting to very low.\n";
              priority_level = Priority_Mapping::VERY_LOW;
              break;

            }
          break;

        case 'n':
          stock_name = get_opts.opt_arg ();
          break;
        case 'c':
          use_naming = true;
          break;

        case '?':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "usage: %s\n"
                             "-o <Distributor IOR> (default is file://StockDistributor.ior)\n"
                             "-p <Priority> (default is 2)\n"
                             "-n <Stock name> (default is IBM)\n"
                             "-c Use the naming service"
                             "\n",
                             argv [0]),
                            -1);
        }
    }

  return 0;
}

static CORBA::Object_ptr
get_distributor_reference (CORBA::ORB_ptr orb)
{
  if (use_naming)
    {
      CORBA::Object_var tmp =
        orb->resolve_initial_references ("NameService");

      CosNaming::NamingContext_var pns =
        CosNaming::NamingContext::_narrow (tmp.in ());

      CosNaming::Name name (1);
      name.length (1);
      name[0].id = distributor_name.c_str ();

      return pns->resolve (name);
    }
  else
    // Read and destringify the Stock_Distributor object's IOR.
    return orb->string_to_object (ior.c_str ());
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialiaze the ORB.
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      // This call MUST come after ORB_init(), which may need to
      // extract -ORB options first.
      if (parse_args (argc, argv) != 0)
        return 1;

      // Get a reference to the RootPOA.
      CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var poa = PortableServer::POA::_narrow (obj.in ());

      // Activate the POAManager.
      PortableServer::POAManager_var mgr = poa->the_POAManager ();
      mgr->activate ();

      // Read and destringify the Stock_Distributor object's IOR.
      obj = get_distributor_reference (orb.in ());

      // Narrow the IOR to a Stock_Distributor object reference.
      Stock::StockDistributor_var stock_distributor =
        Stock::StockDistributor::_narrow (obj.in ());

      if (CORBA::is_nil (stock_distributor.in ()))
        ACE_ERROR_RETURN ((LM_DEBUG,
                           "Nil StockDistributor object reference <%s>\n",
                           ior.c_str ()),
                          1);

      // Create an instance of the <StockBroker>.

      // Create the factory object. Create a <Stock::StockBroker>.
      Stock_StockBrokerHome_i stock_broker_home (orb.in ());
      Stock::StockBroker_var stock_broker =
        stock_broker_home.create (stock_distributor.in (),
                                  stock_name.c_str ());

      if (CORBA::is_nil (stock_broker.in ()))
        ACE_ERROR_RETURN ((LM_DEBUG,
                           "Nil StockBroker object reference <%s>\n",
                           ior.c_str ()),
                          1);

      // Create a new consumer and initialize it.
      Stock::StockNameConsumer_var consumer =
        stock_broker->get_consumer_notifier ();

      // Subscribe the consumer with the distributor.
      ::Stock::Cookie_var cookie =
        stock_distributor->subscribe_notifier (consumer.in (), priority_level);

      consumer->cookie_ (cookie.in ());

      // Get the object reference to a StockQuoter that's been
      // activated at the appropriate priority.
      Stock::StockQuoter_var stock_quoter =
        stock_distributor->provide_quoter_info (cookie.in ());

      if (CORBA::is_nil (stock_quoter.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ERROR: Quoter reference was nil!\n"),
                          -1);

      // Stash the stock_quoter object reference away for later use.
      stock_broker->connect_quoter_info (stock_quoter.in ());

      // Run the event loop.
      ACE_DEBUG ((LM_DEBUG,
                  "running the event loop:\n"
                  "*** message: ready to receive stock information...\n\n"));
      orb->run ();

      // Cleanup the POA and ORB.
      poa->destroy (1, 1);
      orb->destroy ();
    }
  catch (CORBA::Exception &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Broker: Caught unknown C++ exception\n"));
    }

  return 0;
}
