// $Id$
#include "global.h"
#include "SL_Server.h"

#ifndef __OPTIMIZE__
SL_Server::SL_Server (char *usr_name): Single_Lookup (usr_name)
{}

Protocol_Record *
SL_Server::get_each_entry (void)
{
  Protocol_Record *frp = Single_Lookup::get_each_entry ();
  return frp->get_drwho_list () == 0 ? 0 : frp;
}

Protocol_Record *
SL_Server::insert (char *key_name, int max_len)
{
  return strncmp (key_name, this->frp->get_login (), max_len) == 0 ? this->frp : 0;
}
#endif /* __OPTIMIZE__ */
