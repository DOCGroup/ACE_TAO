#include "status_i.h"

CORBA::Boolean
corbaname_Status_i::print_status ()
{
  // A false result lets the client verify that the server handled the request.
  return false;
}
