// $Id$

#include "Options.h"
#include "SL_Client.h"

SL_Client::SL_Client (char *usr_name)
  : Single_Lookup (usr_name)
{
}

Protocol_Record *
SL_Client::insert (char *, int)
{
  return this->frp;
}
