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

      ACE_DEBUG ((LM_ERROR, "Bootstrap to the Lookup interface.\n"));
      CORBA::Object_var trading_obj =
      	orb->resolve_initial_references ("TradingService");
      
      if (CORBA::is_nil (trading_obj.in ()))
      	ACE_ERROR_RETURN ((LM_ERROR,
      			   " (%P|%t) Unable to bootstrap to the Trading Service.\n"),
			   -1);

      
      // Narrow the lookup interface.
      ACE_DEBUG ((LM_DEBUG, "Narrowing the lookup interface.\n"));
      CosTrading::Lookup_var lookup_if = 
      	CosTrading::Lookup::_narrow (trading_obj.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      // Obtain the register interface
      ACE_DEBUG ((LM_DEBUG, "Obtaining the register interface.\n"));
      CosTrading::Register_ptr register_if =
      	lookup_if->register_if (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Obtain the Service Type Repository.
      ACE_DEBUG ((LM_DEBUG, "Obtaining the Service Type Repository.\n"));
      CosTrading::TypeRepository_ptr obj = lookup_if->type_repos (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      // Narrow the Service Type Repository.
      ACE_DEBUG ((LM_DEBUG, "Narrowing the Service Type Repository.\n"));
      CosTradingRepos::ServiceTypeRepository_var tr =
	CosTradingRepos::ServiceTypeRepository::_narrow (obj, TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      // Run the Service Type Exporter tests
      ACE_DEBUG ((LM_DEBUG, "Running the Service Type Exporter tests.\n"));
      TAO_Service_Type_Exporter type_exporter (tr.ptr ());

      //      type_exporter.remove_all_types (TAO_TRY_ENV);
      //      TAO_CHECK_ENV;
      
      type_exporter.add_all_types (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      //type_exporter.list_all_types (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      type_exporter.describe_all_types (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      type_exporter.fully_describe_all_types (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Run the Offer Exporter tests
      ACE_DEBUG ((LM_DEBUG, "Running the Offer Exporter tests.\n"));
      TAO_Offer_Exporter offer_exporter (root_poa.ptr (),
					 register_if,
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

      // Begin trading! 
      if (orb->run () == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "CORBA::ORB::run"), -1);
    }
  TAO_CATCHANY
    {
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "Trader Export Tests Failed"), -1);
    }
  TAO_ENDTRY;

  return 0;
}
