// $Id$

#include "tao/PortableServer/ORB_Manager.h"
#include "Offer_Importer.h"

ACE_RCSID(Trading, import_test, "$Id$")

int
main (int argc, char** argv)
{
  ACE_TRY_NEW_ENV
    {
      TAO_ORB_Manager orb_manager;
      orb_manager.init (argc, argv ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Command line argument interpretation.
      TT_Parse_Args parse_args (argc, argv);

      // Initialize the ORB and bootstrap to the Lookup interface.
      CORBA::ORB_var orb = orb_manager.orb ();
      ACE_DEBUG ((LM_ERROR, "*** Bootstrap to the Lookup interface.\n"));
      char* ior = parse_args.ior ();
      CORBA::Object_var trading_obj = (ior == 0) ?
        orb->resolve_initial_references ("TradingService") :
        orb->string_to_object (ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (trading_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the Trading Service.\n"),
                          -1);

      // Narrow the lookup interface.
      ACE_DEBUG ((LM_DEBUG, "*** Narrowing the lookup interface.\n"));
      CosTrading::Lookup_var lookup_if =
        CosTrading::Lookup::_narrow (trading_obj.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Run the Offer Importer tests
      ACE_DEBUG ((LM_DEBUG, "*** Running the Offer Importer tests.\n"));
      TAO_Offer_Importer offer_importer (lookup_if.in (), ! parse_args.quiet ());

      offer_importer.perform_queries (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args.federated ())
        {
          offer_importer.perform_directed_queries (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_ERROR_RETURN ((LM_ERROR, "Trader Import Tests Failed."), -1);
    }
  ACE_ENDTRY;

  return 0;
}
