/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Obstack.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt
 */
//=============================================================================


#ifndef ACE_OBSTACK_H
#define ACE_OBSTACK_H
#include "ace/pre.h"

#include "ace/Malloc.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ACE_Obchunk
 *
 * @brief Defines the state that represents a "chunk" of memory.
 */
class ACE_Export ACE_Obchunk
{
public:
  friend class ACE_Obstack;

  /// Constructor.
  ACE_Obchunk (size_t size);

  /// dtor.
  ~ACE_Obchunk (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  /// Pointer to the end of the chunk.
  ACE_TCHAR *end_;

  /// Pointer to the current location in the chunk.
  ACE_TCHAR *cur_;

  /// Next chunk in the chain.
  ACE_Obchunk *next_;

  /**
   * Pointer to the beginning contents of this chunk.  This field is
   * actually overlayed by the memory allocated by
   * <ACE_Obstack::new_chunk>.  Therefore, it *must* come last.
   */
  ACE_TCHAR contents_[4];
};

/**
 * @class ACE_Obstack
 *
 * @brief Define a simple "mark and release" memory allocation utility.
 *
 * The implementation is similar to the GNU obstack utility,
 * which is used extensively in the GCC compiler.
 */
class ACE_Export ACE_Obstack
{
public:
  // = Initialization and termination methods.
  ACE_Obstack (size_t size = 4096 - sizeof (ACE_Obchunk),
               ACE_Allocator *allocator_strategy = 0);
  ~ACE_Obstack (void);

  /// Copy the data into the current Obchunk.
  ACE_TCHAR *copy (const ACE_TCHAR *data,
                   size_t len);

  /// "Release" the entire stack of Obchunks, putting it back on the
  /// free list.
  void release (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  class ACE_Obchunk *new_chunk (void);

  /// Pointer to the allocator used by this Obstack.
  ACE_Allocator *allocator_strategy_;

  /// Current size of the Obstack;
  size_t size_;

  // = Don't change the order of the following two fields.
/**
 * @class ACE_Obchunk
 Head of the Obchunk chain.
 */
  class ACE_Obchunk *head_;

/**
 * @class ACE_Obchunk
 Pointer to the current Obchunk.
 */
  class ACE_Obchunk *curr_;
};

#if defined (__ACE_INLINE__)
#include "ace/Obstack.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_OBSTACK_H */
