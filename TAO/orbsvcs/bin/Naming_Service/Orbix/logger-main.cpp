#include "logger_tie.h"
#include <iostream.h>
#include "CosNaming.hh"

// This is a a startup for a Logger_Factory server.
// This is used for testing of the Naming Service.

int
main (int argc, char ** argv)
{

  Logger_Factory_var f = new TIE_Logger_Factory 
    (Logger_Factory_tie) (new Logger_Factory_tie);
    
  try 
    {
      CORBA::Orbix.impl_is_ready ("logger_factory", 120000);
      // The server will time out if no requests come in for 120 seconds.
    }

  catch (const CORBA::SystemException& se)
    {
      cout << "Unexpected exception: " << endl << &se;
      return -1;
    }

  cout << "Server logger_factory is terminating" << endl;
  return 0;
}
