// $Id$

#include "Offer_Importer.h"

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

      // Bootstrap to the Lookup interface.
      ACE_DEBUG ((LM_ERROR, "Bootstrap to the Lookup interface.\n"));
      CORBA::Object_var trading_obj =
	orb->resolve_initial_references ("TradingService");
      
      if (CORBA::is_nil (trading_obj.in ()))
	ACE_ERROR_RETURN ((LM_ERROR,
			   " (%P|%t) Unable to initialize the POA.\n"),
			  -1);
      
      // Narrow the lookup interface.
      ACE_DEBUG ((LM_DEBUG, "Narrowing the lookup interface.\n"));
      CosTrading::Lookup_var lookup_if = 
	CosTrading::Lookup::_narrow (trading_obj.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      // Run the Offer Importer tests
      ACE_DEBUG ((LM_DEBUG, "Running the Offer Importer tests.\n"));
      TAO_Offer_Importer offer_importer (lookup_if.in ());
      
      offer_importer.perform_queries (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      offer_importer.perform_federated_queries (TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "Trader Import Tests Failed."), -1);
    }
  TAO_ENDTRY;
}
