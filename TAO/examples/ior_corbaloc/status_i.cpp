#include "status_i.h"

CORBA::Boolean
corbaloc_Status_i::print_status ()
{
  // A false result indicates that the server received the request.
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Invoking print_status() method for servant with name: %C\nregistered in Naming Service\n"),
              server_name_.c_str ()));
  return false;
}
