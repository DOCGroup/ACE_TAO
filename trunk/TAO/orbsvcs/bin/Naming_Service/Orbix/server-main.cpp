#include "NS_CosNaming.h"
#include <iostream.h>

// This is a startup for the naming server.
// This is used for testing of the Naming Service.

int
main (int argc, char ** argv)
{
  NS_NamingContext *c = new NS_NamingContext;

  CosNaming::NamingContext_ptr cont = new TIE_CosNaming_NamingContext 
    (NS_NamingContext) (c);
    
  c->initialize (cont);

  try 
    {
      CORBA::Orbix.impl_is_ready ("nameserver", 30000);
				  //CORBA::Orbix.INFINITE_TIMEOUT);
      // The server will not time out unless something goes wrong.
    }

  catch (const CORBA::SystemException& se)
    {
      cout << "Unexpected exception: " << endl << &se;
      return -1;
    }

  return 0;
}
