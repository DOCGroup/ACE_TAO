//
// $Id$
//

#include "status_i.h"

corbaname_Status_i::corbaname_Status_i (CORBA::Environment & = TAO_default_environment())
{
  // Constructor
}

CORBA::Boolean
corbaname_Status_i::print_status (CORBA::Environment & = TAO_default_environment())
  throw (CORBA::SystemException)
{
  // If the client makes a succesful request, return a true value
  // indicating that it has successfully reached the server.
  return 0;
}
