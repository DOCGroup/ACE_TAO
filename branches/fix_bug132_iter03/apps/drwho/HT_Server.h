/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    drwho
//
// = FILENAME
//    HT_Server.h
//
// = AUTHOR
//    Douglas C. Schmidt
//
// ============================================================================

#ifndef _HT_SERVER_H
#define _HT_SERVER_H

#include "Hash_Table.h"

class HT_Server : public Hash_Table
{
  // = TITLE
  //   Provides the server's hash table abstraction.

public:
  virtual Protocol_Record *insert (const char *key_name,
                                   int max_len = MAXUSERIDNAMELEN);
};

#endif /* _HT_SERVER_H */
