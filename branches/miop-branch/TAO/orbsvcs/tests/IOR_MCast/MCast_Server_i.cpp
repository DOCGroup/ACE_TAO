// $Id$

#include "MCast_Server_i.h"

MCast_Server_i::MCast_Server_i (void)
{
  //Constructor.
}

CORBA::Boolean
MCast_Server_i::connect_server (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}
