// $Id$

#include "tao/TAO.h"
#include "Offer_Exporter.h"
#include "Offer_Importer.h"
#include "Service_Type_Exporter.h"
#include "ace/Auto_Ptr.h"
#include "orbsvcs/Trader/Trader.h"
#include "orbsvcs/Trader/Service_Type_Repository.h"

ACE_RCSID(Trading, export_test, "$Id$")

int
main (int argc, char** argv)
{
  ACE_TRY_NEW_ENV
    {
      TAO_ORB_Manager orb_manager;
      orb_manager.init (argc, argv, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Command line argument interpretation.
      TT_Parse_Args parse_args (argc, argv);

      // Init the orb and bootstrap to the trading service.
      CORBA::ORB_var orb = orb_manager.orb ();
      ACE_DEBUG ((LM_ERROR, "*** Bootstrap to the Lookup interface.\n"));

      char* ior = parse_args.ior ();
      CORBA::Object_var trading_obj = (ior == 0) ?
        orb->resolve_initial_references ("TradingService") :
        orb->string_to_object (ior);
      
      if (CORBA::is_nil (trading_obj.in ()))
      	ACE_ERROR_RETURN ((LM_ERROR,
      			   " (%P|%t) Unable to bootstrap to the Trading Service.\n"),
			   -1);
      
      // Narrow the lookup interface.
      ACE_DEBUG ((LM_DEBUG, "*** Narrowing the lookup interface.\n"));
      CosTrading::Lookup_var lookup_if = 
      	CosTrading::Lookup::_narrow (trading_obj.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;
            
      // Run the Service Type Exporter tests
      ACE_DEBUG ((LM_DEBUG, "*** Running the Service Type Exporter tests.\n"));
      TAO_Service_Type_Exporter type_exporter (lookup_if.in (),
                                               ! parse_args.quiet (),
                                               ACE_TRY_ENV);
      ACE_TRY_CHECK;

      type_exporter.remove_all_types (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      type_exporter.add_all_types (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      type_exporter.remove_all_types (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      type_exporter.add_all_types (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (parse_args.federated ())
        {
          type_exporter.add_all_types_to_all (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      
      type_exporter.list_all_types (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      type_exporter.describe_all_types (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      type_exporter.fully_describe_all_types (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "*** Service Type Exporter tests complete.\n"));

      // Run the Offer Exporter tests
      ACE_DEBUG ((LM_DEBUG, "*** Running the Offer Exporter tests.\n"));
      TAO_Offer_Exporter offer_exporter (lookup_if.in (), ! parse_args.quiet (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // = Test series.
      
      offer_exporter.withdraw_offers (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      offer_exporter.export_offers (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      offer_exporter.describe_offers (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      offer_exporter.modify_offers (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      offer_exporter.describe_offers (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      offer_exporter.withdraw_offers_using_constraints (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      offer_exporter.describe_offers (ACE_TRY_ENV);
      ACE_TRY_CHECK;      

      offer_exporter.withdraw_offers (ACE_TRY_ENV);
      ACE_TRY_CHECK;      

      offer_exporter.export_offers (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      if (parse_args.federated ())
        {
          offer_exporter.export_offers_to_all (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      
      offer_exporter.describe_offers (ACE_TRY_ENV);
      ACE_TRY_CHECK;      

      ACE_DEBUG ((LM_DEBUG, "*** Offer Exporter tests complete.\n"));
      ACE_DEBUG ((LM_DEBUG, "*** Now serving dynamic properties.\n"));

      orb_manager.run (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_ERROR_RETURN ((LM_ERROR, "Trader Export Tests Failed"), -1);
    }
  ACE_ENDTRY;

  return 0;
}
