// $Id$

#include "Offer_Exporter.h"
#include "Offer_Importer.h"
#include "Service_Type_Exporter.h"
#include "ace/Auto_Ptr.h"
#include "orbsvcs/Trader/Trader.h"
#include "orbsvcs/Trader/Service_Type_Repository.h"

int
main (int argc, char** argv)
{
  TAO_TRY
    {
      // Initialize ORB.
      CORBA::ORB_var orb = 
	CORBA::ORB_init (argc, argv, "internet", TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CORBA::Object_var poa_object = 
	orb->resolve_initial_references("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
	ACE_ERROR_RETURN ((LM_ERROR,
			   " (%P|%t) Unable to initialize the POA.\n"),
			  1);

      PortableServer::POA_var root_poa =
	PortableServer::POA::_narrow (poa_object.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      PortableServer::POAManager_var poa_manager =
	root_poa->the_POAManager (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      // Create a Service Type Repository and a Trader Object.
      TAO_Service_Type_Repository type_repos;
      auto_ptr<TAO_Trader_Factory::TAO_TRADER> trader =
	TAO_Trader_Factory::create_linked_trader ();
      TAO_Support_Attributes_Impl& sup_attr = trader->support_attributes ();
      TAO_Trading_Components_Impl& trd_comp = trader->trading_components ();

      // Set the service type repository
      sup_attr.type_repos (type_repos._this (TAO_TRY_ENV));
      TAO_CHECK_ENV;

      // Obtain the Service Type Repository.
      ACE_DEBUG ((LM_DEBUG, "Obtaining the Service Type Repository.\n"));
      //CosTrading::TypeRepository_ptr obj = lookup_if->type_repos (TAO_TRY_ENV);
      CosTrading::TypeRepository_ptr obj = sup_attr.type_repos ();
      TAO_CHECK_ENV;
      
      // Narrow the Service Type Repository.
      ACE_DEBUG ((LM_DEBUG, "Narrowing the Service Type Repository.\n"));
      CosTradingRepos::ServiceTypeRepository_var tr =
	CosTradingRepos::ServiceTypeRepository::_narrow (obj, TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      // Run the Service Type Exporter tests
      ACE_DEBUG ((LM_DEBUG, "Running the Service Type Exporter tests.\n"));
      TAO_Service_Type_Exporter type_exporter (tr.ptr ());

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
	(root_poa.ptr (),
	 CosTrading::Register::_duplicate (trd_comp.register_if ()),
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
      
      poa_manager->activate (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Run the Offer Importer tests
      ACE_DEBUG ((LM_DEBUG, "Running the Offer Exporter tests.\n"));
      TAO_Offer_Importer offer_importer
	(CosTrading::Lookup::_duplicate (trd_comp.lookup_if ()));
      TAO_CHECK_ENV;
      
      offer_importer.perform_queries (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      // Begin trading! 
      //if (orb->run () == -1)
      //ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "CORBA::ORB::run"), -1);
    }
  TAO_CATCHANY
    {
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "Trader Export Tests Failed"), -1);
    }
  TAO_ENDTRY;
}
