// $Id$

#include "tao/ORB.h"
#include "tao/corba.h"
#include "orbsvcs/IOR_Multicast.h"
#include "orbsvcs/Naming/Naming_Utils.h"
#include "BankC.h"
#include "BankS.h"

int
main (int argc, char *argv[])
{
  Bank::AccountManager_var accountmanager_server;
  Bank::Account_var server;
  
  TAO_TRY
    {
      
      // Retrieve the ORB.
      CORBA::ORB_var orb_ = CORBA::ORB_init (argc,
					     argv,
					     0,
					     TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      if (argc < 2)
	{
	  ACE_DEBUG ((LM_DEBUG,
		      "No Name specified to be resolved\n"));
	}
      else
	{
	  CORBA::Object_var naming_obj =
	    orb_->resolve_initial_references (argv[1]);
	  
	  if (CORBA::is_nil (naming_obj.in ()))
	    ACE_ERROR_RETURN ((LM_ERROR,
			       " (%P|%t) Unable to resolve the given Name.\n"),
			      -1);
	  
	  ACE_DEBUG ((LM_DEBUG,
		      "Fail now\n"));
	  
	  accountmanager_server =
	    Bank::AccountManager::_narrow (naming_obj.in (),
					   TAO_TRY_ENV);
	  TAO_CHECK_ENV;
	  
	  server = accountmanager_server->open ("Vishal",
						100.00,
						TAO_TRY_ENV);
	  ACE_DEBUG ((LM_DEBUG,
		      "Unreachable\n"));
	  
	  TAO_CHECK_ENV;
	  
	  server->deposit (100.00,
			   TAO_TRY_ENV);
	  TAO_CHECK_ENV;
	  
	  ACE_DEBUG ((LM_DEBUG,
		      "Balance returned from Server : %f\n",
		      server->balance (TAO_TRY_ENV)));
		      
	  
	  ACE_DEBUG ((LM_DEBUG,
		      "Resolved IOR for %s : %s\n",
		      argv[1],
		      orb_->object_to_string (naming_obj.in ())));
	  
	}
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("init");
      return -1;
    }
  TAO_ENDTRY;
  
  return 0;
}




