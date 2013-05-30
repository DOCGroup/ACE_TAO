// $Id$

#include "tao/Utils/ORB_Manager.h"
#include "Offer_Importer.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
int failure = 0;

  try
    {
      if (argc<=1)
      {
        ACE_DEBUG ((LM_INFO,
                    "\nUsage: import_test <options>\n"
                    "Test the trader's importer role, the options are:\n"
                    "-ORBInitRef <orb_options> E.g. '-ORBInitRef TradingService=file://trading.ior'\n"
                    "-quiet or -q              (optional) Run the tests in quiet mode, don't show the query and list results.\n"
                    "-federate or -f           (optional) Perform federated trader tests.\n"
                    "-iorfile or -i            (optional) Get TradingService reference through IOR instead of resolve.\n"
                    "\n"
                    "This test needs the 'export_test' running so the trader has the right offers.\n"
                    "See also TAO/docs/releasenotes/trader.html.\n\n"));
        return 1;
      };
      TAO_ORB_Manager orb_manager;
      orb_manager.init (argc, argv);

      // Command line argument parser.
      TT_Parse_Args parse_args (argc, argv);

      // Initialize the ORB and bootstrap to the Lookup interface.
      CORBA::ORB_var orb = orb_manager.orb ();
      ACE_DEBUG ((LM_DEBUG, "*** Bootstrap to the Lookup interface.\n"));
      char* ior = parse_args.ior ();
      CORBA::Object_var trading_obj = (ior == 0) ?
        orb->resolve_initial_references ("TradingService") :
        orb->string_to_object (ior);

      if (CORBA::is_nil (trading_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT("ERROR, Unable to bootstrap to the Trading Service!\n")),
                           1);

      // Narrow the lookup interface.
      ACE_DEBUG ((LM_DEBUG, "*** Narrowing the lookup interface.\n"));
      CosTrading::Lookup_var lookup_if =
        CosTrading::Lookup::_narrow (trading_obj.in ());

      // Run the Offer Importer tests.
      ACE_DEBUG ((LM_DEBUG, "*** Running the Offer Importer tests.\n"));
      TAO_Offer_Importer offer_importer (lookup_if.in (), ! parse_args.quiet ());

      offer_importer.perform_queries ();

      if (parse_args.federated ())
        {
          offer_importer.perform_directed_queries ();
        }
    }
  catch (const CORBA::Exception& e)
    {
      e._tao_print_exception ("Trader import test ERROR!");
      failure = 1;
    }

  return failure;
}
