// $Id$

#include "tao/TAO.h"
#include "ace/Auto_Ptr.h"
#include "Offer_Exporter.h"
#include "Offer_Importer.h"
#include "Service_Type_Exporter.h"
#include "orbsvcs/Trader/Trader.h"
#include "orbsvcs/Trader/Service_Type_Repository.h"

void
parse_args (int argc, char *argv[],
            CORBA::Boolean& verbose)
{
  int opt;
  ACE_Get_Opt get_opt (argc, argv, "fq");

  verbose = CORBA::B_TRUE;
  while ((opt = get_opt ()) != EOF)
    {
      if (opt == 'q')
        verbose = CORBA::B_FALSE;
    }
}

int
main (int argc, char** argv)
{
  TAO_TRY
    {
      TAO_ORB_Manager orb_manager;
      orb_manager.init (argc, argv, TAO_TRY_ENV);
      TAO_CHECK_ENV

      // Command line argument interpretation.
      CORBA::Boolean verbose = CORBA::B_FALSE;
      ::parse_args (argc, argv, verbose);
        
      // Initialize ORB.
      CORBA::ORB_var orb = orb_manager.orb ();
      
      // Create a Service Type Repository and a Trader Object.
      TAO_Service_Type_Repository type_repos;
      auto_ptr<TAO_Trader_Factory::TAO_TRADER> trader =
	auto_ptr<TAO_Trader_Factory::TAO_TRADER>(TAO_Trader_Factory::create_trader (argc, argv));
      TAO_Support_Attributes_Impl& sup_attr = trader->support_attributes ();
      TAO_Trading_Components_Impl& trd_comp = trader->trading_components ();

      // Set the service type repository
      sup_attr.type_repos (type_repos._this (TAO_TRY_ENV));
      TAO_CHECK_ENV;

      // Run the Service Type Exporter tests
      ACE_DEBUG ((LM_DEBUG, "Running the Service Type Exporter tests.\n"));
      TAO_Service_Type_Exporter type_exporter
        (CosTrading::Lookup::_duplicate (trd_comp.lookup_if ()),
         verbose,
         TAO_TRY_ENV);
      TAO_CHECK_ENV;

      type_exporter.remove_all_types (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      type_exporter.add_all_types (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      type_exporter.list_all_types (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      type_exporter.describe_all_types (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      type_exporter.fully_describe_all_types (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Run the Offer Exporter tests
      ACE_DEBUG ((LM_DEBUG, "Running the Offer Exporter tests.\n"));
      TAO_Offer_Exporter offer_exporter
	(CosTrading::Lookup::_duplicate (trd_comp.lookup_if ()),
         verbose,
	 TAO_TRY_ENV);
      TAO_CHECK_ENV;

      offer_exporter.withdraw_offers (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      offer_exporter.export_offers (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      offer_exporter.describe_offers (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      //      offer_exporter.modify_offers (TAO_TRY_ENV);
      //TAO_CHECK_ENV;

      //offer_exporter.describe_offers (TAO_TRY_ENV);
      //TAO_CHECK_ENV;

      //offer_exporter.withdraw_offers_using_constraints (TAO_TRY_ENV);
      //TAO_CHECK_ENV;

      //offer_exporter.describe_offers (TAO_TRY_ENV);
      //TAO_CHECK_ENV;      

      offer_exporter.withdraw_offers (TAO_TRY_ENV);
      TAO_CHECK_ENV;      

      offer_exporter.export_offers (TAO_TRY_ENV);
      TAO_CHECK_ENV;      

      offer_exporter.describe_offers (TAO_TRY_ENV);
      TAO_CHECK_ENV;      
      
      // Run the Offer Importer tests
      ACE_DEBUG ((LM_DEBUG, "Running the Offer Exporter tests.\n"));
      TAO_Offer_Importer offer_importer
	(CosTrading::Lookup::_duplicate (trd_comp.lookup_if ()), verbose);
      
      offer_importer.perform_queries (TAO_TRY_ENV);
      TAO_CHECK_ENV;      
    }
  TAO_CATCHANY
    {
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "Trader Export Tests Failed"), -1);
    }
  TAO_ENDTRY;

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class auto_ptr<TAO_Trader_Factory::TAO_TRADER>;
template class ACE_Auto_Basic_Ptr<TAO_Trader_Factory::TAO_TRADER>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate auto_ptr<TAO_Trader_Factory::TAO_TRADER>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Trader_Factory::TAO_TRADER>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
