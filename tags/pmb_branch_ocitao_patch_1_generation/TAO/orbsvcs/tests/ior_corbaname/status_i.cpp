//
// $Id$
//

#include "status_i.h"

corbaname_Status_i::corbaname_Status_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  // Constructor
}

CORBA::Boolean
corbaname_Status_i::print_status (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // If the client makes a succesful request, return a true value
  // indicating that it has successfully reached the server.
  return 0;
}
