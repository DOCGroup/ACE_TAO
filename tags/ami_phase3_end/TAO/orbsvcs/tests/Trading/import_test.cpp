// $Id$

#include "tao/TAO.h"
#include "Offer_Importer.h"

ACE_RCSID(Trading, import_test, "$Id$")

int
main (int argc, char** argv)
{
  TAO_TRY
    {
      TAO_ORB_Manager orb_manager;
      orb_manager.init (argc, argv, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Command line argument interpretation.
      TT_Parse_Args parse_args (argc, argv);
      
      // Initialize the ORB and bootstrap to the Lookup interface.
      CORBA::ORB_var orb = orb_manager.orb ();
      ACE_DEBUG ((LM_ERROR, "*** Bootstrap to the Lookup interface.\n"));
      char* ior = parse_args.ior ();
      CORBA::Object_var trading_obj = (ior == 0) ?
        orb->resolve_initial_references ("TradingService") :
        orb->string_to_object (ior);

      
      if (CORBA::is_nil (trading_obj.in ()))
	ACE_ERROR_RETURN ((LM_ERROR,
			   " (%P|%t) Unable to initialize the Trading Service.\n"),
			  -1);
      
      // Narrow the lookup interface.
      ACE_DEBUG ((LM_DEBUG, "*** Narrowing the lookup interface.\n"));
      CosTrading::Lookup_var lookup_if = 
	CosTrading::Lookup::_narrow (trading_obj.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      // Run the Offer Importer tests
      ACE_DEBUG ((LM_DEBUG, "*** Running the Offer Importer tests.\n"));
      TAO_Offer_Importer offer_importer (lookup_if.in (), ! parse_args.quiet ());
      
      offer_importer.perform_queries (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (parse_args.federated ())
        {
          offer_importer.perform_directed_queries (TAO_TRY_ENV);
          TAO_CHECK_ENV;
        }
    }
  TAO_CATCHANY
    {
      ACE_ERROR_RETURN ((LM_ERROR, "Trader Import Tests Failed."), -1);
    }
  TAO_ENDTRY;

  return 0;
}
