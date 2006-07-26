// $Id$

// ACE header files
#include "ace/Get_Opt.h"

// local header files
#include "DistributorC.h"

static const char *ior = "file://StockDistributor.ior";
static const char *option = "start";
static const char *rate = "1";

int parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:r:bs");
  int c;

  while ((c = get_opts ()) != -1)
  {
    switch (c)
    {
      case 'o':
        ior = get_opts.opt_arg ();
        break;
      case 'r':
        option = "rate";
        rate = get_opts.opt_arg ();
        break;
      case 'b':
        option = "start";
        break;
      case 's':
        option = "stop";
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                          "usage: %s\n"
                          "-o <Distributor IOR> (the ior file of stock distributor (default is file://StockDistributor.ior)) \n"
                          "-r <rate> (set the distribution rate to 'n' milliseconds (default is 1)) \n"
                          "-b (start the stock distributor) \n"
                          "-s (stop the stock distributor) \n"
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

    if (ACE_OS::strcmp (option, "stop") == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Stop the stock distributor.\n"));
      stock_distributor->stop ();
    }
    else if (ACE_OS::strcmp (option, "rate") == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Set the distribution notification rate to %d seconds.\n", atoi (rate)));
      stock_distributor->notification_rate (atoi (rate));
    }
    else
    {
      ACE_DEBUG ((LM_DEBUG, "Start the stock distributor.\n"));
      stock_distributor->start ();
    }

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
