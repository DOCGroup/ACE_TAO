/* -*- C++ -*- */
// $Id$

/* Provides an "Abstract Base Class" lookup table abstraction that 
   stores and manipulates friend records. */

#ifndef _SEARCH_STRUCT_H
#define _SEARCH_STRUCT_H

#include "Protocol_Record.h"

class Search_Struct
{
protected:
  int count;

public:
	                Search_Struct (void);
  virtual	        ~Search_Struct (void);
  virtual int		n_elems (void);

  virtual Protocol_Record *insert (char *key_name, int max_len = MAXUSERIDNAMELEN) = 0;
  virtual Protocol_Record *get_next_entry (void)	 = 0;
  virtual Protocol_Record *get_each_entry (void)	 = 0;
};

#ifdef __OPTIMIZE__
inline
Search_Struct::~Search_Struct (void)
{}

inline 
Search_Struct::Search_Struct (void): count (0)
{}

inline int
Search_Struct::n_elems (void)
{
  return this->count;
}
#endif /* __OPTIMIZE__ */
#endif /* _SEARCH_STRUCT_H */
