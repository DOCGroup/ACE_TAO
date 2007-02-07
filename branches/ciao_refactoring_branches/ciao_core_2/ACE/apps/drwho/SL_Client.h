/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    drwho
//
// = FILENAME
//    SL_Client.h
//
// = AUTHOR
//    Douglas C. Schmidt
//
// ============================================================================

#ifndef _SL_CLIENT_H
#define _SL_CLIENT_H

#include "Single_Lookup.h"

class SL_Client : public Single_Lookup
{
  // = TITLE
  //   Provides the client's single user lookup table abstraction.

public:
  SL_Client (const char *key_name);
  virtual Protocol_Record *insert (const char *key_name,
                                   int max_len = MAXUSERIDNAMELEN);
};

#endif /* _SL_CLIENT_H */
