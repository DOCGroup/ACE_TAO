/* -*- C++ -*- */
// $Id$

/* Provides the client's single user lookup table abstraction. */

#ifndef _SL_CLIENT_H
#define _SL_CLIENT_H

#include "Single_Lookup.h"

class SL_Client : public Single_Lookup
{
public:
	                SL_Client (char *usr_name);
  virtual Protocol_Record *insert (char *key_name, int max_len = MAXUSERIDNAMELEN);
};

#ifdef __OPTIMIZE__
inline 
SL_Client::SL_Client (char *usr_name): Single_Lookup (usr_name)
{}

inline Protocol_Record *
SL_Client::insert (char *key_name, int)
{
  return this->frp;
}
#endif /* __OPTIMIZE__ */
#endif /* _SL_CLIENT_H */
