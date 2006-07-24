// $Id$

// ACE header files
#include "ace/Get_Opt.h"

// local header files
#include "Distributor_i.h"

static const char *ior = "StockDistributor.ior";
static const char *rate = "1";

int parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:r:");
  int c;

  while ((c = get_opts ()) != -1)
  {
    switch (c)
    {
      case 'o':
        ior = get_opts.opt_arg ();
        break;
      case 'r':
        rate = get_opts.opt_arg ();
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
  if (parse_args (argc, argv) != 0)
    return 1;

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
    FILE *output_file= ACE_OS::fopen (ior, "w");
    if (output_file == 0)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Cannot open output file for writing IOR: %s\n",
                         ior),
                         1);
    ACE_OS::fprintf (output_file, "%s", str.in ());
    ACE_OS::fclose (output_file);

    stock_distributor->notification_rate (atoi (rate));

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
