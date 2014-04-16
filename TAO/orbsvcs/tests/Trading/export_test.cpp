// $Id$

#include "tao/Utils/ORB_Manager.h"
#include "Offer_Exporter.h"
#include "Offer_Importer.h"
#include "Service_Type_Exporter.h"
#include "ace/Auto_Ptr.h"
#include "orbsvcs/Trader/Trader.h"
#include "orbsvcs/Trader/Service_Type_Repository.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *done_file = ACE_TEXT("export_test_ready");

int
parse_args_spec (int argc, ACE_TCHAR *argv[], int help)
{
  if (help!=0)
  {
    ACE_DEBUG ((LM_INFO,
                "\nUsage: export_test <options>\n"
                "Test the trader's exporter role, the options are:\n"
                "-ORBInitRef <orb_options> E.g. '-ORBInitRef TradingService=iiop://localhost:8901/TradingService'\n"
                "-quiet or -q              (optional) Run the tests in quiet mode, don't show the query and list results.\n"
                "-federate or -f           (optional) Setup for federated trader tests.\n"
                "-iorfile or -i            (optional) Get TradingService reference through IOR instead of resolve.\n"
                "-d <done_file>            (optional) Filename for signalling that we are ready for serving dynamic properties (for import_test).\n"
                "\n"
                "This test needs an external TradingServer running, example:\n"
                "   Trading_Service -ORBEndpoint iiop://:8901 -TSdumpior trading.ior\n"
                "See also TAO/docs/releasenotes/trader.html.\n\n"));
    return -1;
  }
  else
  {
    ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("d:"));
    for (int opt; (opt = get_opts()) != -1;)
    {
      if (opt == 'd')
      {
        done_file = get_opts.opt_arg ();
      };
    };
  };
  return 0;
}


int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
int failure = 0;

  try
    {
      int help=(argc<=1);

      TAO_ORB_Manager orb_manager;
      orb_manager.init (argc, argv);

      // Command line argument parser for use with comlex pearl tests.
      if (parse_args_spec (argc, argv, help) != 0)
        return 1;

      // Command line argument parser for the trading tests.
      TT_Parse_Args parse_args (argc, argv);

      // Init the orb and bootstrap to the trading service.
      CORBA::ORB_var orb = orb_manager.orb ();
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT("*** Bootstrap to the Lookup interface.\n")));

      char* ior = parse_args.ior ();
      CORBA::Object_var trading_obj = (ior == 0) ?
        orb->resolve_initial_references ("TradingService") :
        orb->string_to_object (ior);

      if (CORBA::is_nil (trading_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT("ERROR, Unable to bootstrap to the Trading Service!\n")),
                           1);

      // Narrow the lookup interface.
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT("*** Narrowing the lookup interface.\n")));
      CosTrading::Lookup_var lookup_if =
        CosTrading::Lookup::_narrow (trading_obj.in ());

      // Run the Service Type Exporter tests.
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT("*** Running the Service Type Exporter tests.\n")));
      TAO_Service_Type_Exporter type_exporter (lookup_if.in (), ! parse_args.quiet ());

      type_exporter.remove_all_types ();

      type_exporter.add_all_types ();

      type_exporter.remove_all_types ();

      type_exporter.add_all_types ();

      if (parse_args.federated ())
        {
          type_exporter.add_all_types_to_all ();
        }

      type_exporter.list_all_types ();

      type_exporter.describe_all_types ();

      type_exporter.fully_describe_all_types ();

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT("*** Service Type Exporter tests complete.\n")));

      // Run the Offer Exporter tests.
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT("*** Running the Offer Exporter tests.\n")));
      TAO_Offer_Exporter offer_exporter (lookup_if.in (), ! parse_args.quiet ());

      // = Test series.

      offer_exporter.withdraw_offers ();

      offer_exporter.export_offers ();

      offer_exporter.describe_offers ();

      offer_exporter.modify_offers ();

      offer_exporter.describe_offers ();

      offer_exporter.withdraw_offers_using_constraints ();

      offer_exporter.describe_offers ();

      offer_exporter.withdraw_offers ();

      offer_exporter.export_offers ();

      if (parse_args.federated ())
        {
          offer_exporter.export_offers_to_all ();
        }

      offer_exporter.describe_offers ();

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT("*** Offer Exporter tests complete.\n")));
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT("*** Now serving dynamic properties.\n")));

      FILE *ready_file = ACE_OS::fopen (done_file, ACE_TEXT("w"));
      if (ready_file != 0)
        {
          ACE_OS::fprintf (ready_file, "The export test is ready\n");
          ACE_OS::fclose (ready_file);
        }
      else
        {
          ACE_DEBUG ((LM_WARNING,
                      ACE_TEXT("Unable to open %s for output.\n"),
                      done_file));
        }

      orb_manager.run ();
    }
  catch (const CORBA::Exception& e)
    {
      e._tao_print_exception ("Trader export test ERROR!");
      failure = 1;
    }

  return failure;
}
