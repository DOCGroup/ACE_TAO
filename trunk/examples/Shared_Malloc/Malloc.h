/* -*- C++ -*- */
// @(#)Malloc.h	1.1	10/18/96

#if !defined (MY_MALLOC_H)
#define MY_MALLOC_H

#include "ace/Malloc.h"

class Malloc
  // = TITLE
  //      Allocator Singleton.
{
public:
  static ACE_Allocator *instance (void);
  // Returns static instance.

private:
  Malloc (void);
  // Ensure Singleton.

  static ACE_Allocator *instance_;
  // Malloc Singleton.
};

#endif /* MY_MALLOC_H */
