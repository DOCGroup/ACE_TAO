/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    drwho
//
// = FILENAME
//    SL_Server.h
//
// = AUTHOR
//    Douglas C. Schmidt
//
// ============================================================================

#if !defined (_SL_SERVER_H)
#define _SL_SERVER_H

#include "Single_Lookup.h"

class SL_Server : public Single_Lookup
{
  // = TITLE
  //   Provides the server's single user lookup table abstraction. 

public:
  SL_Server (char *packet);
  virtual Protocol_Record *insert (char *key_name,
                                   int max_len = MAXUSERIDNAMELEN);
  virtual Protocol_Record *get_each_entry (void);
};

#endif /* _SL_SERVER_H */
