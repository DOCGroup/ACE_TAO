// $Id$

//

#include "status_i.h"

corbaloc_Status_i::corbaloc_Status_i (CORBA::Environment & = TAO_default_environment())
{
  // Constructor
}

CORBA::Boolean
corbaloc_Status_i::print_status (CORBA::Environment & = TAO_default_environment())
  throw (CORBA::SystemException)
{

  // If the server received the request from the client,
  // return true == 0;

  return 0;
}
