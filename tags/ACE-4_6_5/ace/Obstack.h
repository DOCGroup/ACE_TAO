/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Obstack.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_OBSTACK_H
#define ACE_OBSTACK_H

#include "ace/Malloc.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Export ACE_Obchunk
{
  // = TITLE
  //     Defines the state that represents a "chunk" of memory.
public:
  friend class ACE_Obstack;

  ACE_Obchunk (size_t size);
  // Constructor.

  ~ACE_Obchunk (void);
  // dtor.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  char *end_;
  // Pointer to the end of the chunk.

  char *cur_;
  // Pointer to the current location in the chunk.

  ACE_Obchunk *next_;
  // Next chunk in the chain.

  char contents_[4];
  // Pointer to the beginning contents of this chunk.  This field is
  // actually overlayed by the memory allocated by
  // <ACE_Obstack::new_chunk>.  Therefore, it *must* come last.
};

class ACE_Export ACE_Obstack
{
  // = TITLE
  //     Define a simple "mark and release" memory allocation utility.
  //
  // = DESCRIPTION
  //     The implementation is similar to the GNU obstack utility,
  //     which is used extensively in the GCC compiler.
public:
  // = Initialization and termination methods.
  ACE_Obstack (size_t size = 4096 - sizeof (ACE_Obchunk),
               ACE_Allocator *allocator_strategy = 0);
  ~ACE_Obstack (void);

  char *copy (const char *data,
              size_t len);
  // Copy the data into the current Obchunk.

  void release (void);
  // "Release" the entire stack of Obchunks, putting it back on the
  // free list.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  class ACE_Obchunk *new_chunk (void);

  ACE_Allocator *allocator_strategy_;
  // Pointer to the allocator used by this Obstack.

  size_t size_;
  // Current size of the Obstack;

  // = Don't change the order of the following two fields.
  class ACE_Obchunk *head_;
  // Head of the Obchunk chain.

  class ACE_Obchunk *curr_;
  // Pointer to the current Obchunk.
};

#if defined (__ACE_INLINE__)
#include "ace/Obstack.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_OBSTACK_H */
