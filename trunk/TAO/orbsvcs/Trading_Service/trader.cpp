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

#include "Trader.h"
#include "Service_Type_Repository.h"

typedef ACE_Trader<ACE_Null_Mutex, ACE_Null_Mutex> TRADER;
typedef ACE_Service_Type_Repository<ACE_Null_Mutex> TYPE_REPOS;

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

      // Create a Service Type Repository and a Trader Object.
      TYPE_REPOS type_repos;
      TRADER trader ((TRADER::Trader_Components)
		     (TRADER::LOOKUP | TRADER::REGISTER | TRADER::ADMIN)); 
      ACE_Support_Attributes_Impl& sup_attr = trader.support_attributes ();
      ACE_Trading_Components_Impl& trd_comp = trader.trading_components ();

      // Set the service type repository
      sup_attr.type_repos (&type_repos);

      // Activate the supported interfaces.
      poa_object->activate_object (trd_comp.lookup_if (), TAO_TRY_ENV);
      TAO_CHECK_ENV;
      poa_object->activate_object (trd_comp.register_if (), TAO_TRY_ENV);
      TAO_CHECK_ENV;
      poa_object->activate_object (trd_comp.admin_if (), TAO_TRY_ENV);
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

