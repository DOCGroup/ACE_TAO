/* -*- C++ -*- */
// $Id$

/* Provides the server's single user lookup table abstraction. */

#ifndef _SL_SERVER_H
#define _SL_SERVER_H

#include "Single_Lookup.h"

class SL_Server : public Single_Lookup
{
public:
	                SL_Server (char *packet);
  virtual Protocol_Record *insert (char *key_name, int max_len = MAXUSERIDNAMELEN);
  virtual Protocol_Record *get_each_entry (void);
};

#ifdef __OPTIMIZE__
inline
SL_Server::SL_Server (char *usr_name): Single_Lookup (usr_name)
{}

inline Protocol_Record *
SL_Server::get_each_entry (void)
{
  Protocol_Record *frp = Single_Lookup::get_each_entry ();
  return frp->get_drwho_list () == 0 ? 0 : frp;
}

inline Protocol_Record *
SL_Server::insert (char *key_name, int max_len)
{
  return strncmp (key_name, this->frp->get_login (), max_len) == 0 ? this->frp : 0;
}
#endif /* __OPTIMIZE__ */
#endif /* _SL_SERVER_H */
