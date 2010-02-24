// $Id$

#include "status_i.h"

corbaloc_Status_i::corbaloc_Status_i ()
  : server_name_()
{
  // Constructor
}

CORBA::Boolean
corbaloc_Status_i::print_status (void)
{
  // If the server received the request from the client, return true
  // == 0;
  ACE_DEBUG ((LM_DEBUG,
              "Invoking print_status() method for servant with name: %s\nregistered in Naming Service\n",
              server_name_.c_str ()));
  return 0;
}
