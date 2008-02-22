// $Id$

// ACE header files
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdlib.h"
#include "orbsvcs/CosNamingC.h"

// STL strings
#include <string>

// local header files
#include "Distributor_i.h"
#include "Stock_Database.h"

// Name of the file that stores the StockDistributor IOR.
static std::string ior = "StockDistributor.ior";

// Name of the StockDistributor registered with the Naming Service.
static std::string distributor_name = "StockDistributor";

// The default rate, which sends data out once per second.
CORBA::Long rate = 1;

u_int update_freq = 1;

// A flag that indicates use of the Naming Service.
bool use_naming = false;

static int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:r:u:c");
  int c;

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'o':
          ior = get_opts.opt_arg ();
          break;
        case 'r':
          rate = ACE_OS::atoi (get_opts.opt_arg ());
          break;
        case 'u':
          update_freq = ACE_OS::atoi (get_opts.opt_arg ());
        case 'c':
          use_naming = true;
          break;

        case '?':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "usage: %s\n"
                             "-o <Distributor IOR> (default is file://StockDistributor.ior) \n"
                             "-r <Distributor notification rate> (default is 1 second)\n"
                             "-u <Database update rate> (default is 1 second)\n"
                             "-c Flag that indicates that the object should be registered with naming service\n"
                             "\n",
                             argv [0]),
                            -1);
        }
    }

  return 0;
}

static int
set_distributor_reference (CORBA::ORB_ptr orb,
                           CORBA::Object_ptr obj)
{
  if (use_naming)
    {
      // Naming Service related operations
      CORBA::Object_var naming_context_object =
        orb->resolve_initial_references ("NameService");

      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_context_object.in ());

      // Initialize the Naming Sequence
      CosNaming::Name name (1);
      name.length (1);

      name[0].id = distributor_name.c_str ();

      // Register the servant with the Naming Service
      try
        {
          // Register the servant with the Naming Service
          naming_context->bind (name, obj);
        }
      catch (CosNaming::NamingContext::AlreadyBound &)
        {
          ACE_DEBUG ((LM_DEBUG, "%s already bound, rebinding....\n",
                      distributor_name.c_str ()));
          naming_context->rebind (name, obj);
        }
    }
  else
    {
      // Write the object reference for the <stock_distributor> to a
      // file so the <StockBroker> can read it when it's
      // bootstrapping.
      CORBA::String_var str = orb->object_to_string (obj);
      FILE *output_file = ACE_OS::fopen (ior.c_str (), "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s\n",
                           ior.c_str ()),
                          -1);
      ACE_OS::fprintf (output_file, "%s", str.in ());
      ACE_OS::fclose (output_file);
    }

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initalize the ORB.
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      // This call MUST come after ORB_init(), which may need to
      // extract -ORB options first.
      if (parse_args (argc, argv) != 0)
        return 1;

      // Get the RootPOA.
      CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var poa = PortableServer::POA::_narrow (obj.in ());

      // Activate the POAManager.
      PortableServer::POAManager_var mgr = poa->the_POAManager ();
      mgr->activate ();

      // Create the factory object. Create a <Stock::StockDistributor>.
      StockDistributorHome_i stock_distributor_home (orb.in ());

      Stock::StockDistributor_var stock_distributor = stock_distributor_home.create ();

      if (CORBA::is_nil (stock_distributor.in ()))
        ACE_ERROR_RETURN ((LM_DEBUG,
                           "Nil StockDistributor object reference <%s>\n",
                           ior.c_str ()),
                          1);

      // Store the distributor reference in a location that's
      // accessible by the client.
      if (set_distributor_reference (orb.in (),
                                     stock_distributor.in ()) == -1)
        return 1;

      // Set the initial notification rate.
      stock_distributor->notification_rate (rate);

      // Set the database update rate
      STOCK_DATABASE->update_rate (update_freq);

      // Enter into the event looping.
      ACE_DEBUG ((LM_DEBUG,
                  "*** message: ready for transmission...\n"));
      orb->run ();

      // Cleanup the POA and ORB.
      poa->destroy (1, 1);
      orb->destroy ();
    }
  catch (CORBA::Exception &ex)
    {
      ex._tao_print_exception ("Admin: ");

      return 1;
    }
  return 0;
}
