// $Id$

//============================================================================
//
// = FILENAME
//     server.cpp
//
// = DESCRIPTION
//     Server application to test the Servant Activator and Servant Locator.
//
// = AUTHOR
//     Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
//=============================================================================

#include "Server_i.h"

ACE_RCSID(On_Demand_Loading, server, "$Id$")

int 
main (int argc, char **argv)
{
  Server_i server;
  CORBA::Environment TAO_TRY_ENV;
  
  TAO_TRY
    {
      server.init (argc, argv);
      TAO_CHECK_ENV;
  
      // This poa is used to test the Servant Activator interface and
      // hence has an RETAIN policy.
      PortableServer::POA_var first_poa = 
        server.create_poa ("firstPOA",
                           1);
      TAO_CHECK_ENV;

      // This poa is used to test the Servant Activator interface and
      // hence has an NONRETAIN policy.
      PortableServer::POA_var second_poa =
        server.create_poa ("secondPOA",
                           0);

      TAO_CHECK_ENV;
     
      // The Servant Activator interface is created and initialised.
      server.create_activator (first_poa);
      TAO_CHECK_ENV;
    
      // The Servant Locator interface is created and initialised.
      server.create_locator (second_poa);
      TAO_CHECK_ENV;
    
      // The server begins processing requests.
      server.run ();
      TAO_CHECK_ENV; 
    
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("server::main ()\n");
      return 1;
    }
  TAO_ENDTRY;

return 0;
}
