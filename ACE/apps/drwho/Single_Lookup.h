/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Single_Lookup.h
 *
 *  @author Douglas C. Schmidt
 */
//=============================================================================

#ifndef _SINGLE_LOOKUP_H
#define _SINGLE_LOOKUP_H

#include "Options.h"
#include "Search_Struct.h"
#include <memory>

/**
 * @class Single_Lookup
 *
 * Provides the client's single user lookup table abstraction.
 */
class Single_Lookup : public Search_Struct
{
public:
  explicit Single_Lookup (const char *usr_name);
  ~Single_Lookup () override;
  Single_Lookup (Single_Lookup const &) = delete;
  Single_Lookup (Single_Lookup &&) = delete;
  Single_Lookup &operator= (Single_Lookup const &) = delete;
  Single_Lookup &operator= (Single_Lookup &&) = delete;
  Protocol_Record *insert (const char *key_name,
                           int max_len = MAXUSERIDNAMELEN) override = 0;
  Protocol_Record *get_next_entry () override;
  Protocol_Record *get_each_entry () override;

protected:
  std::unique_ptr<Protocol_Record> prp_;
};

#endif /* _SINGLE_LOOKUP_H */
