// $Id$
#include "Options.h"
#include "SL_Client.h"

#ifndef __OPTIMIZE__
SL_Client::SL_Client (char *usr_name): Single_Lookup (usr_name)
{}

Protocol_Record *
SL_Client::insert (char *, int)
{
  return this->frp;
}
#endif /* __OPTIMIZE__ */
