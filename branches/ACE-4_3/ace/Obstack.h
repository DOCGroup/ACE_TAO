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

#if !defined (ACE_OBSTACK_H)
#define ACE_OBSTACK_H

#include "ace/ACE.h"

class ACE_Export ACE_Obchunk
  // = TITLE
  //     A "chunk" of memory.  This should be a nested class but some
  //     compilers don't like them yet. 
{
friend class ACE_Obstack;
  
public:
  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  char *end_;
  char *cur_;
  ACE_Obchunk *next_;
  char contents_[4];
};

class ACE_Export ACE_Obstack
  // = TITLE
  //    Define a simple "mark and release" memory allocation utility.
  //    This class is based on the GNU obstack utility.
{
public:
  // = Initialization and termination methods.
  ACE_Obstack (int size = 4080);
  ~ACE_Obstack (void);

  char *copy (const char* data, size_t len);
  // Copy the data into the current Obchunk.
  
  void release (void);
  // "Release" the entire stack (without freeing it).

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  class ACE_Obchunk *new_chunk (void);

  size_t size_;
  // Current size of the Obstack;

  class ACE_Obchunk *head_;
  // Head of the Obchunk chain.

  class ACE_Obchunk *curr_;
  // Pointer to the current Obchunk.
};

#endif /* ACE_OBSTACK_H */
