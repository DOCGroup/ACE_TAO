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
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Trader/Trader.h"
#include "orbsvcs/Trader/Service_Type_Repository.h"

const char* service_name = "TradingService";

typedef TAO_Trader<ACE_Null_Mutex, ACE_Null_Mutex> TRADER;
typedef TAO_Service_Type_Repository<ACE_Null_Mutex> TYPE_REPOS;

int main(int argc, char * const *argv)
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

      CORBA::Object_var naming_obj =
	orb->resolve_initial_references ("NameService");
      if (CORBA::is_nil (naming_obj.in ()))
	ACE_ERROR_RETURN ((LM_ERROR,
			   " (%P|%t) Unable to initialize the POA.\n"),
			  1);

      CosNaming::NamingContext_var naming_context = 
        CosNaming::NamingContext::_narrow (naming_obj.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      
      // Create a Service Type Repository and a Trader Object.
      TYPE_REPOS type_repos;
      TRADER trader ((TRADER::Trader_Components)
		     (TRADER::LOOKUP | TRADER::REGISTER | TRADER::ADMIN)); 
      TAO_Support_Attributes_Impl& sup_attr = trader.support_attributes ();
      TAO_Trading_Components_Impl& trd_comp = trader.trading_components ();

      // Set the service type repository
      sup_attr.type_repos (type_repos._this (TAO_TRY_ENV));
      TAO_CHECK_ENV;

      CosTrading::Lookup_var lookup = trd_comp.lookup_if ();
      
      CosNaming::Name trading_name (1);
      trading_name.length (1);
      trading_name[0].id = CORBA::string_dup (service_name);
      naming_context->bind (trading_name, lookup.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

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

