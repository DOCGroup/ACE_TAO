// $Id$

#include "Options.h"
#include "SL_Client.h"

SL_Client::SL_Client (const char *usr_name)
  : Single_Lookup (usr_name)
{
}

Protocol_Record *
SL_Client::insert (const char *, int)
{
  return this->prp_;
}
