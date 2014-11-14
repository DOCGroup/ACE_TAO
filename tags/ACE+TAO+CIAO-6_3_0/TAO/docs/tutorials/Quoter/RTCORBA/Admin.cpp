// $Id$

// ACE header files
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdlib.h"
#include "orbsvcs/CosNamingC.h"
// STL strings
#include <string>

// local header files
#include "DistributorC.h"

enum Option_Types
{
  START,
  STOP,
  RATE
};

// Name of the file that stores the StockDistributor IOR.
static std::string ior = "file://StockDistributor.ior";

// Name of the StockDistributor registered with the Naming Service.
static std::string distributor_name = "StockDistributor";

// The default command, which starts the StockDistributor.
static Option_Types option = START;

// The default rate, which sends data out once per second.
static std::string rate = "1";

// A flag that indicates use of the Naming Service.
static bool use_naming = false;

// Parse the command-line arguments and set the global options.

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:r:bsc"));
  int c;

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'o':
          ior = get_opts.opt_arg ();
          break;
        case 'r':
          option = RATE;
          rate = get_opts.opt_arg ();
          break;
        case 'b':
          option = START;
          break;
        case 's':
          option = STOP;
          break;
        case 'c':
          use_naming = get_opts.opt_arg ();
          break;

        case '?':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "usage: %s\n"
                             "-o <Distributor IOR> (the ior file of stock distributor (default is file://StockDistributor.ior))\n"
                             "-r <rate> (set the distribution rate to 'n' seconds (default is 1))\n"
                             "-b (start the stock distributor)\n"
                             "-s (stop the stock distributor)\n"
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

      CORBA::Object_var obj = get_distributor_reference (orb.in ());

      // Narrow the IOR to a Stock_Distributor object reference.
      Stock::StockDistributor_var stock_distributor =
        Stock::StockDistributor::_narrow(obj.in ());

      if (CORBA::is_nil (stock_distributor.in ()))
        ACE_ERROR_RETURN ((LM_DEBUG,
                           "Nil StockDistributor object reference <%s>\n",
                           ior.c_str ()),
                          1);

      if (option == STOP)
        {
          ACE_DEBUG ((LM_DEBUG, "Stop the stock distributor.\n"));
          stock_distributor->stop ();
        }
      else if (option == RATE)
        {
          CORBA::Long notify_rate = ACE_OS::atoi (rate.c_str ());
          ACE_DEBUG ((LM_DEBUG,
                      "Set the distribution notification rate to %d seconds.\n",
                      notify_rate));
          stock_distributor->notification_rate (notify_rate);
        }
      else // option == START
        {
          ACE_DEBUG ((LM_DEBUG, "Start the stock distributor.\n"));
          stock_distributor->start ();
        }

      // Since we are a "pure client" we don't need to run the ORB's event loop!
      // Cleanup the ORB.
      orb->destroy ();
    }
  catch (CORBA::Exception &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
