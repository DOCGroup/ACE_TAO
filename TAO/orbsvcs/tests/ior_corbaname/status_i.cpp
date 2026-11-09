#include "status_i.h"

CORBA::Boolean
corbaname_Status_i::print_status ()
{
  // If the client makes a succesful request, return a true value
  // indicating that it has successfully reached the server.
  return false;
}
