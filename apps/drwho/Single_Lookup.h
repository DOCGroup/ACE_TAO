/* -*- C++ -*- */
// $Id$

/* Provides the client's single user lookup table abstraction. */

#ifndef _SINGLE_LOOKUP_H
#define _SINGLE_LOOKUP_H

#include "Options.h"
#include "Search_Struct.h"

class Single_Lookup : public Search_Struct
{
protected:
  Protocol_Record	        *frp;

public:
	                Single_Lookup (char *usr_name);
  virtual		~Single_Lookup (void);
  virtual Protocol_Record *insert (char *key_name, int max_len = MAXUSERIDNAMELEN) = 0;
  virtual Protocol_Record *get_next_entry (void);
  virtual Protocol_Record *get_each_entry (void);
};

#ifdef __OPTIMIZE__
inline
Single_Lookup::Single_Lookup (char *usr_name)
{
  this->frp = new Protocol_Record (ACE::strnew (usr_name));
}

inline
Single_Lookup::~Single_Lookup (void)
{
  if (Options::DEBUG)
    fprintf (stderr, "disposing Single_Lookup\n");
}

inline Protocol_Record *
Single_Lookup::get_each_entry (void)
{
  return this->frp;
}

inline Protocol_Record *
Single_Lookup::get_next_entry (void)
{
  return this->get_each_entry ();
}
#endif /* __OPTIMIZE__ */
#endif /* _SINGLE_LOOKUP_H */
