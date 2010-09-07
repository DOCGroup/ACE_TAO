// $Id$

#include "Server_i.h"

Server_i::Server_i (void)
{
}

Server_i::~Server_i (void)
{
}

char *
Server_i::get_status (void)
{
  return CORBA::string_dup ("active");
}
