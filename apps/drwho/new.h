/* -*- C++ -*- */
// $Id$

/* Provides a fast, fixed-sized pool of memory. */

#ifndef _NEW_H
#define _NEW_H

#include <stddef.h>

enum New_Pool
{
  MALLOC_POOL  = 1,
  PRIVATE_POOL = 2
};

void *operator new (size_t size, New_Pool new_pool);

void operator delete (void *);

/* Release all the memory... */
void release_memory (void);

/* Set the mark for future memory allocation... */
void mark_memory (int start_using_the_pool = 1);

#endif /* _NEW_H */
