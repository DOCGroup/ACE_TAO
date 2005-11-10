// $Id$

#include "Server_i.h"

ACE_RCSID (PSS, Server_i, "$Id$")

Server_i::Server_i (void)
{
}

Server_i::~Server_i (void)
{
}

char *
Server_i::get_status (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("active");
}
