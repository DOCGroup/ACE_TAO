/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Search_Struct.h
 *
 *  @author Douglas C. Schmidt
 */
//=============================================================================


#ifndef _SEARCH_STRUCT_H
#define _SEARCH_STRUCT_H

#include "Protocol_Record.h"

/**
 * @class Search_Struct
 *
 * @brief Provides an "Abstract Base Class" lookup table abstraction that
 * stores and manipulates friend records.
 */
class Search_Struct
{
public:
  Search_Struct ();
  virtual ~Search_Struct ();
  virtual int n_elems ();

  virtual Protocol_Record *insert (const char *key_name,
                                   int max_len = MAXUSERIDNAMELEN) = 0;
  virtual Protocol_Record *get_next_entry () = 0;
  virtual Protocol_Record *get_each_entry () = 0;

protected:
  int count_;
};

#endif /* _SEARCH_STRUCT_H */
