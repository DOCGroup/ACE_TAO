/* -*- c++ -*- */
// $Id$

#ifndef ACE_CACHE_MANAGER_H
#define ACE_CACHE_MANAGER_H

#include "ace/OS.h"
#include "ace/ACE.h"
#include "ace/Synch.h"

#include "ace_Cache_Manager_T.h"

class ACE_String_Hash_Functor
{
public:
  ACE_String_Hash_Functor (const char *s);
  operator int (void) const;

private:
  int i_;
};

class ACE_String_Equal_Functor
{
public:
  ACE_String_Equal_Functor (const char *s1, const char *s2);
  operator int (void) const;

private:
  int i_;
};

typedef ACE_Cache_Manager<const char *,
                          ACE_Referenced_Cache_Object_Factory,
                          ACE_String_Hash_Functor,
                          ACE_String_Equal_Functor>
        ACE_String_Referenced_Cache_Manager;

typedef ACE_Cache_Manager<const char *,
                          ACE_Counted_Cache_Object_Factory,
                          ACE_String_Hash_Functor,
                          ACE_String_Equal_Functor>
        ACE_String_Counted_Cache_Manager;


#endif /* ACE_CACHE_MANAGER_H */
