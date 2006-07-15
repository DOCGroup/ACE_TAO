// $Id$

// local header files
#include "DistributorC.h"

// ACE header files
#include "ace/streams.h"
#include "ace/Get_Opt.h"

// STL header files
#include <string>

static const char *ior = "file://StockDistributor.ior";

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

void usage ()
{
  ACE_DEBUG ((LM_DEBUG, "This is the administator program for the stock distributor.\n"));
  ACE_DEBUG ((LM_DEBUG, "Use the following commands to control the behavior of the stock distributor:\n"));
  ACE_DEBUG ((LM_DEBUG, "start  -- start the stock distributor\n"));
  ACE_DEBUG ((LM_DEBUG, "stop   -- stop the stock distributor\n"));
  ACE_DEBUG ((LM_DEBUG, "exit   -- exit the administator program\n"));
  ACE_DEBUG ((LM_DEBUG, "rate n -- set the distribution rate to 'n' milliseconds\n"));
}

int main (int argc, char *argv[])
{
  if (parse_args (argc, argv) != 0)
    return 1;

  // Print the usage to the client.
  usage ();

  CORBA::ORB_var orb;

  try
  {
    // Initialiaze the ORB.
    orb = CORBA::ORB_init (argc, argv);

    // Read and destringify the Stock_Distributor object's IOR.
    CORBA::Object_var obj = orb->string_to_object (ior);

    // Narrow the IOR to a Stock_Distributor object reference.
    Stock::StockDistributor_var stock_distributor = Stock::StockDistributor::_narrow(obj);
    if (CORBA::is_nil (stock_distributor.in ()))
    {
      ACE_ERROR_RETURN ((LM_DEBUG,
                         "Nil StockDistributor object reference <%s>\n",
                         ior),
                        1);
    }

    std::string cmd;

    do
    {
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
  catch (CORBA::Exception &ex)
  {
    ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                         "Exception caught:");
    return 1;
  }
  return 0;
}
