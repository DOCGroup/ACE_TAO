/* -*- c++ -*- */
// $Id$

// @@ James, please add a standard "header" here like you see in all
// the other ACE headerfiles.

#ifndef ACE_CACHE_MANAGER_H
#define ACE_CACHE_MANAGER_H

#include "ace/OS.h"
#include "ace/ACE.h"
#include "ace/Synch.h"

// @@ James, this #include is wrong...
#include "ace/Cache_Manager_T.h"

// @@ James, can you please update ALL of these classes and methods to
// use the standard ACE commenting style, i.e., add = TITLE and =
// DESCRIPTION headers and comments for each method.

class ACE_String_Hash_Functor
{
public:
  ACE_String_Hash_Functor (const char *s);
  operator int (void) const;

private:
  int i_;
};

// @@ James, can you put these functors into the same place that the
// ones used by Irfan in the Hash_Map_Manager_Ex file are used?!
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
