// $Id$

#include "ace/Auto_Ptr.h"
#include "ace/Get_Opt.h"
#include "tao/Utils/ORB_Manager.h"
#include "Offer_Exporter.h"
#include "Offer_Importer.h"
#include "Service_Type_Exporter.h"
#include "orbsvcs/Trader/Trader.h"
#include "orbsvcs/Trader/Service_Type_Repository.h"

int
parse_args (int argc, ACE_TCHAR *argv[],
            CORBA::Boolean& verbose)
{
  verbose = 1;
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("qh"));
  for (int opt; (opt = get_opts()) != -1;)
  {
    switch (opt)
    {
      case 'q':
        verbose = 0;
        break;

      case 'h':
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT("\nUsage: colocated_test <options>\n"
                  "Test both the trader's exporter and importer roles.\n"
                  "The options are:\n"
                  "-quiet or -q   (optional) Run the tests in quiet mode, don't show the query and\n"
                  "                          list results.\n"
                  "-h             (optional) Show this help.\n"
                  "\n"
                  "This test uses an embedded trading service.\n"
                  "If you want to test with an external trading server or test federated traders\n"
                  "then use the other tests:\n"
                  " - 'export_test' (needs running 'Trading_Service')\n"
                  " - 'import_test'\n"
                  "See also TAO/docs/releasenotes/trader.html.\n\n"
                  ) ));
        return -1;
    }
  }
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
int failure = 0;

  try
    {
      // Command line argument interpretation.
      CORBA::Boolean verbose = 0;
      if (parse_args (argc, argv, verbose) != 0)
        return 1;

      ACE_DEBUG ((LM_DEBUG, "*** Initialize ORB and POA manager.\n"));
      TAO_ORB_Manager orb_manager;
      orb_manager.init (argc, argv);

      //-------------------------------------------------------------------------
      ACE_DEBUG ((LM_DEBUG, "*** Setup an embedded TradingService.\n"));
      // Create a Service Type Repository and a Trader Object so the test can
      // run without an external trading service.
      // Start of Trading service scope.
      {
        TAO_Service_Type_Repository type_repos;
        auto_ptr<TAO_Trader_Factory::TAO_TRADER> trader (TAO_Trader_Factory::create_trader (argc, argv));
        TAO_Support_Attributes_i& sup_attr = trader->support_attributes ();
        TAO_Trading_Components_i& trd_comp = trader->trading_components ();

        // Set the service type repository
        sup_attr.type_repos (type_repos._this ());

        CosTrading::Lookup_var lookup_if = CosTrading::Lookup::_duplicate(
            trd_comp.lookup_if());

        // Run the POA manager.
        orb_manager.activate_poa_manager();

        // Run the Service Type Exporter tests.
        ACE_DEBUG ((LM_DEBUG, "*** Running the Service Type Exporter tests.\n"));
        TAO_Service_Type_Exporter type_exporter(lookup_if.in(), verbose);

        type_exporter.remove_all_types ();

        type_exporter.add_all_types ();

        type_exporter.list_all_types ();

        type_exporter.describe_all_types ();

        type_exporter.fully_describe_all_types ();

        // Run the Offer Exporter tests.
        ACE_DEBUG ((LM_DEBUG, "*** Running the Offer Exporter tests.\n"));
        TAO_Offer_Exporter offer_exporter(lookup_if.in(), verbose);

        offer_exporter.withdraw_offers ();

        offer_exporter.export_offers ();

        offer_exporter.describe_offers ();

        offer_exporter.modify_offers ();

        offer_exporter.describe_offers ();

        offer_exporter.withdraw_offers_using_constraints ();

        offer_exporter.describe_offers ();

        offer_exporter.withdraw_offers ();

        offer_exporter.export_offers ();

        offer_exporter.describe_offers ();

        // Run the Offer Importer tests.
        ACE_DEBUG ((LM_DEBUG, "*** Running the Offer Importer tests.\n"));
        TAO_Offer_Importer offer_importer(lookup_if.in(), verbose);

        offer_importer.perform_queries ();

      }; // End of Trading service scope.
      //-----------------------------------------------------------------------
      ACE_DEBUG ((LM_DEBUG, "*** Shutting down.\n"));
      orb_manager.fini();
    }
  catch (const CORBA::Exception& e)
    {
      e._tao_print_exception ("Trader colocated test ERROR!");
      failure = 1;
    }

  return failure;
}
