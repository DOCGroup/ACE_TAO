/* -*- C++ -*- */
// $Id$

/* Provides the client's hash table abstraction. */

#ifndef _HT_CLIENT_H
#define _HT_CLIENT_H

#include "Hash_Table.h"

class HT_Client : public Hash_Table
{
public:
  virtual Protocol_Record *insert (char *key_name, int max_len = MAXUSERIDNAMELEN);
};
#endif /* _HT_CLIENT_H */
