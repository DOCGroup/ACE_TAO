//
// $Id$
//

#include "status_i.h"

corbaloc_Status_i::corbaloc_Status_i (CORBA::Environment & = TAO_default_environment())
{
  // Constructor
}

void
corbaloc_Status_i::print_status (CORBA::Environment & = TAO_default_environment()) 
  throw (CORBA::SystemException)
{

  ACE_DEBUG ((LM_DEBUG, 
              "The connection between the server and client is setup\n"));
  
}
              
