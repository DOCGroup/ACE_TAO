// $Id$

#include "status_i.h"

corbaloc_Status_i::corbaloc_Status_i (TAO_ENV_ARG_DECL_NOT_USED)
        : server_name_()
{
  // Constructor
}

CORBA::Boolean
corbaloc_Status_i::print_status (TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  // If the server received the request from the client,
  // return true == 0;
  ACE_DEBUG((LM_DEBUG,
             "Invoking print_status() method for servant with name: %s\nregistered in Naming Service\n",

              server_name_.c_str() ));
  return 0;
}
