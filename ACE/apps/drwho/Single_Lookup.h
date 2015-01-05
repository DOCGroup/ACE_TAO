/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Single_Lookup.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt
 */
//=============================================================================


#ifndef _SINGLE_LOOKUP_H
#define _SINGLE_LOOKUP_H

#include "Options.h"
#include "Search_Struct.h"

/**
 * @class Single_Lookup
 *
 * Provides the client's single user lookup table abstraction.
 */
class Single_Lookup : public Search_Struct
{
public:
  Single_Lookup (const char *usr_name);
  virtual ~Single_Lookup (void);
  virtual Protocol_Record *insert (const char *key_name,
                                   int max_len = MAXUSERIDNAMELEN) = 0;
  virtual Protocol_Record *get_next_entry (void);
  virtual Protocol_Record *get_each_entry (void);

protected:
  Protocol_Record *prp_;
};

#endif /* _SINGLE_LOOKUP_H */
