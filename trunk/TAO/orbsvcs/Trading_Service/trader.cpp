// $Id$

// ========================================================================
//
// = BINARY
//    trader
//  
// = FILENAME 
//    trader.cpp
// 
// = AUTHOR 
//    Marina Spivak <marina@cs.wustl.edu> 
//    Seth Widoff <sbw1@cs.wustl.edu> 
//   
// ======================================================================= 

#include "tao/corba.h"
#include "ace/Auto_Ptr.h"
#include "orbsvcs/IOR_Multicast.h"
#include "orbsvcs/Trader/Trader.h"
#include "orbsvcs/Trader/Service_Type_Repository.h"

int
main(int argc, char * const *argv)
{ 
  TAO_TRY
    {      
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

#if defined TAO_HAS_DYNAMIC_PROPERTY_BUG
      auto_ptr<TAO_Trader_Factory::TAO_TRADER> trader =
	TAO_Trader_Factory::create_linked_trader (orb.ptr ());      
#else
      auto_ptr<TAO_Trader_Factory::TAO_TRADER> trader =
	TAO_Trader_Factory::create_linked_trader ();
#endif /* TAO_HAS_DYNAMIC_PROPERTY_BUG */
      
      TAO_Support_Attributes_Impl& sup_attr = trader->support_attributes ();
      TAO_Trading_Components_Impl& trd_comp = trader->trading_components ();

      // Set the service type repository
      sup_attr.type_repos (type_repos._this (TAO_TRY_ENV));
      TAO_CHECK_ENV;
      
#if defined (ACE_HAS_IP_MULTICAST)
      // Get reactor instance from TAO.
      ACE_Reactor *reactor =
        TAO_ORB_Core_instance ()->reactor ();
      
      // First, see if the user has given us a multicast port number
      // for the name service on the command-line;
      u_short port =
        TAO_ORB_Core_instance ()->orb_params ()->name_service_port ();

      if (port == 0)
	{
	  const char *port_number =
            ACE_OS::getenv ("TradingServicePort");

	  if (port_number != 0)
	    port = ACE_OS::atoi (port_number);
	  else
	    port = TAO_DEFAULT_TRADING_SERVER_REQUEST_PORT;
	}

      // The Spec says: return a reference to the Lookup interface
      // from the resolve_initial_references method.
      CosTrading::Lookup_var lookup = trd_comp.lookup_if ();
      CORBA::String_var trading_ior =
	orb->object_to_string (lookup.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Instantiate a server that will receive requests for an ior
      TAO_IOR_Multicast ior_multicast ((char *) trading_ior.in (),
				       port,
				       ACE_DEFAULT_MULTICAST_ADDR,
				       TAO_SERVICEID_TRADINGSERVICE);
      
      // Register event handler for the ior multicast.
      if (reactor->register_handler (&ior_multicast,
				     ACE_Event_Handler::READ_MASK) == -1)
	ACE_DEBUG ((LM_DEBUG,
                    "cannot register Event handler\n"));
      else
	ACE_DEBUG ((LM_DEBUG,
                    "The multicast server setup is done.\n"));
#else  /* ACE_HAS_IP_MULTICAST */

      // Should dump the IOR somewhere.
      
#endif /* ACE_HAS_IP_MULTICAST */

      
      poa_manager->activate (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Begin trading!
      if (orb->run () == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "CORBA::ORB::run"), -1);
    }
    TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("EC");
    }
  TAO_ENDTRY;

  return 0;
}

