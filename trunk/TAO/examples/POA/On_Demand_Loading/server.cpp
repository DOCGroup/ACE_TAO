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

#include "Server_Manager.h"

ACE_RCSID(On_Demand_Loading, server, "$Id$")

int 
main (int argc, char **argv)
{
  Server_i server;
  
  // @@ Kirthika, I don't think the error-handling logic is quite
  // correct yet.  Can you please make sure that you check for error
  // returns from these methods and bail out of main() if you do,
  // rather than fall through to other methods?
  server.init (argc, argv);
  
  // This POA is used to test the Servant Activator interface and
  // hence has an RETAIN policy.
  PortableServer::POA_var first_poa = 
    server.create_poa ("firstPOA", 1);
      
  // This poa is used to test the Servant Activator interface and
  // hence has an NONRETAIN policy.
  PortableServer::POA_var second_poa =
    server.create_poa ("secondPOA", 0);
     
  // The Servant Activator interface is created and initialised.
  server.create_activator (first_poa);
          
  // The Servant Locator interface is created and initialised.
  server.create_locator (second_poa);
         
  // The server begins processing requests.
  server.run ();
  
  return 0;
}
