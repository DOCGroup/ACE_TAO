#include "global.h"
#include "SL_Server.h"
#include <cstring>

SL_Server::SL_Server (const char *usr_name)
  : Single_Lookup (usr_name)
{
}

Protocol_Record *
SL_Server::get_each_entry ()
{
  Protocol_Record *prp = Single_Lookup::get_each_entry ();
  return prp->get_drwho_list () == nullptr ? nullptr : prp;
}

Protocol_Record *
SL_Server::insert (const char *key_name, int max_len)
{
  return std::strncmp (key_name,
                       this->prp_->get_login (),
                       max_len) == 0 ? this->prp_ : nullptr;
}

