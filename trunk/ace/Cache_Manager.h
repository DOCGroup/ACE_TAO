/* -*- c++ -*- */
// $Id$

#ifndef ACE_CACHE_MANAGER_H
#define ACE_CACHE_MANAGER_H

#include "ace/OS.h"
#include "ace/ACE.h"
#include "ace/Synch.h"

#include "ace/Cache_Manager_T.h"

class ACE_String_Hash_Functor
{
public:
  ACE_String_Hash_Functor (const char *s);
  operator unsigned long (void) const;

private:
  unsigned long i_;
};

class ACE_String_Equal_Functor
{
public:
  ACE_String_Equal_Functor (const char *s1, const char *s2);
  operator int (void) const;

private:
  int i_;
};

class ACE_Strdup_String
{
public:

  ACE_Strdup_String (void);
  ACE_Strdup_String (const char *s);
  ACE_Strdup_String (const ACE_Strdup_String &s);
  ~ACE_Strdup_String (void);

  operator const char * (void) const;
  void operator = (const char *s);
  void operator = (const ACE_Strdup_String &s);

private:

  int *c_;
  char *s_;

};

typedef ACE_Cache_Manager<ACE_Strdup_String,
                          ACE_Referenced_Cache_Object_Factory,
                          ACE_String_Hash_Functor,
                          ACE_String_Equal_Functor>
        ACE_String_Referenced_Cache_Manager;

typedef ACE_Cache_Manager<ACE_Strdup_String,
                          ACE_Counted_Cache_Object_Factory,
                          ACE_String_Hash_Functor,
                          ACE_String_Equal_Functor>
        ACE_String_Counted_Cache_Manager;


#endif /* ACE_CACHE_MANAGER_H */
