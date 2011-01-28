// $Id$

#include "ace/Auto_Ptr.h"
#include "ace/Get_Opt.h"
#include "tao/Utils/ORB_Manager.h"
#include "Offer_Exporter.h"
#include "Offer_Importer.h"
#include "Service_Type_Exporter.h"
#include "orbsvcs/Trader/Trader.h"
#include "orbsvcs/Trader/Service_Type_Repository.h"

void
parse_args (int argc, ACE_TCHAR *argv[],
            CORBA::Boolean& verbose)
{
  int opt;
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("fq"));

  verbose = 1;
  while ((opt = get_opt ()) != EOF)
    {
      if (opt == 'q')
        verbose = 0;
    }
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      TAO_ORB_Manager orb_manager;
      orb_manager.init (argc, argv);

      // Command line argument interpretation.
      CORBA::Boolean verbose = 0;
      ::parse_args (argc, argv, verbose);

      // Initialize ORB.
      CORBA::ORB_var orb = orb_manager.orb ();

      // Create a Service Type Repository and a Trader Object.
      TAO_Service_Type_Repository type_repos;
      auto_ptr<TAO_Trader_Factory::TAO_TRADER> trader (TAO_Trader_Factory::create_trader (argc, argv));
      TAO_Support_Attributes_i& sup_attr = trader->support_attributes ();
      TAO_Trading_Components_i& trd_comp = trader->trading_components ();

      // Set the service type repository
      sup_attr.type_repos (type_repos._this ());

      // Run the Service Type Exporter tests
      ACE_DEBUG ((LM_DEBUG, "*** Running the Service Type Exporter tests.\n"));
      TAO_Service_Type_Exporter type_exporter
        (CosTrading::Lookup::_duplicate (trd_comp.lookup_if ()),
         verbose);

      type_exporter.remove_all_types ();

      type_exporter.add_all_types ();

      type_exporter.list_all_types ();

      type_exporter.describe_all_types ();

      type_exporter.fully_describe_all_types ();

      // Run the Offer Exporter tests
      ACE_DEBUG ((LM_DEBUG, "*** Running the Offer Exporter tests.\n"));
      TAO_Offer_Exporter offer_exporter
        (CosTrading::Lookup::_duplicate (trd_comp.lookup_if ()),
         verbose);

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

      // Run the Offer Importer tests
      ACE_DEBUG ((LM_DEBUG, "*** Running the Offer Importer tests.\n"));
      TAO_Offer_Importer offer_importer
        (CosTrading::Lookup::_duplicate (trd_comp.lookup_if ()), verbose);

      offer_importer.perform_queries ();
    }
  catch (const CORBA::Exception&)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "Trader Export Tests Failed"), -1);
    }

  return 0;
}
