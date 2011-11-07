/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    SL_Server.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt
 */
//=============================================================================


#ifndef _SL_SERVER_H
#define _SL_SERVER_H

#include "Single_Lookup.h"

/**
 * @class SL_Server
 *
 * @brief Provides the server's single user lookup table abstraction.
 */
class SL_Server : public Single_Lookup
{

public:
  SL_Server (const char *packet);
  virtual Protocol_Record *insert (const char *key_name,
                                   int max_len = MAXUSERIDNAMELEN);
  virtual Protocol_Record *get_each_entry (void);
};

#endif /* _SL_SERVER_H */
