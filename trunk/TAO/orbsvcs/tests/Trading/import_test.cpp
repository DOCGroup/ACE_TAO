
// $Id$

#include "tao/TAO.h"
#include "Offer_Importer.h"

int
is_federated (int argc, char *argv[])
{
  int opt;
  ACE_Get_Opt get_opt (argc, argv, "f");

  CORBA::Boolean return_value = CORBA::B_FALSE;
  while ((opt = get_opt ()) != EOF)
    {
      if (opt == 'f')
        return_value = CORBA::B_TRUE;
    }

  return return_value;
}

int
main (int argc, char** argv)
{
  TAO_TRY
    {
      TAO_ORB_Manager orb_manager;
      orb_manager.init (argc, argv, TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      // Initialize ORB.
      CORBA::ORB_var orb = orb_manager.orb ();
      CORBA::Boolean federated = ::is_federated (argc, argv);
      
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
      CosTrading::Lookup_ptr lookup_if = 
	CosTrading::Lookup::_narrow (trading_obj.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      // Run the Offer Importer tests
      ACE_DEBUG ((LM_DEBUG, "Running the Offer Importer tests.\n"));
      TAO_Offer_Importer offer_importer (lookup_if);
      
      offer_importer.perform_queries (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (federated)
        {
          offer_importer.perform_directed_queries (TAO_TRY_ENV);
          TAO_CHECK_ENV;
        }
    }
  TAO_CATCHANY
    {
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "Trader Import Tests Failed."), -1);
      TAO_TRY_ENV.print_exception ("main");
    }
  TAO_ENDTRY;

  return 0;
}
