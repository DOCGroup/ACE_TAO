/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Handle_Set.h
//
// = AUTHOR
//    Douglas C. Schmidt <schmidt@cs.wustl.edu>
//
// ============================================================================

#ifndef ACE_HANDLE_SET_H
#include "ace/pre.h"
#define ACE_HANDLE_SET_H

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Export ACE_Handle_Set
{
  // = TITLE
  //     C++ wrapper facade for the socket <fd_set> abstraction.
  //
  // = DESCRIPTION
  //     This abstraction is a very efficient wrapper facade over
  //     <fd_set>.  In particular, no range checking is performed, so
  //     it's important not to set or clear bits that are outside the
  //     <ACE_DEFAULT_SELECT_REACTOR_SIZE>.
public:
  friend class ACE_Handle_Set_Iterator;

  // = Initialization and termination.

  enum
  {
    MAXSIZE = ACE_DEFAULT_SELECT_REACTOR_SIZE
  };

  // = Initialization methods.
  ACE_Handle_Set (void);
  // Constructor, initializes the bitmask to all 0s.

  ACE_Handle_Set (const ACE_FD_SET_TYPE &mask);
  // Constructor, initializes the handle set from a given mask.
  // <ACE_FD_SET_TYPE> is a <typedef> based on the platform's native
  // type used for masks passed to <select>.

#if defined (ACE_HAS_WINCE)
  ~ACE_Handle_Set (void);
  // Default dtor.
#endif /* ACE_HAS_WINCE */

  // = Methods for manipulating bitsets.
  void reset (void);
  // Initialize the bitmask to all 0s and reset the associated fields.

  int is_set (ACE_HANDLE handle) const;
  // Checks whether <handle> is enabled.  No range checking is
  // performed so <handle> must be less than
  // <ACE_DEFAULT_SELECT_REACTOR_SIZE>.

  void set_bit (ACE_HANDLE handle);
  // Enables the <handle>.  No range checking is performed so <handle>
  // must be less than <ACE_DEFAULT_SELECT_REACTOR_SIZE>.

  void clr_bit (ACE_HANDLE handle);
  // Disables the <handle>.  No range checking is performed so
  // <handle> must be less than <ACE_DEFAULT_SELECT_REACTOR_SIZE>.

  int num_set (void) const;
  // Returns a count of the number of enabled bits.

  ACE_HANDLE max_set (void) const;
  // Returns the number of the large bit.

  void sync (ACE_HANDLE max);
  // Rescan the underlying <fd_set> up to handle <max> to find the new
  // <max_handle> (highest bit set) and <size> (how many bits set) values.
  // This is useful for evaluating the changes after the handle set has
  // been manipulated in some way other than member functions; for example,
  // after <select> modifies the <fd_set>.

  operator fd_set *();
  // Returns a pointer to the underlying <fd_set>.  Returns 0 if
  // there are no handle bits set (<size_> == 0).

  fd_set *fdset (void);
  // Returns a pointer to the underlying <fd_set>.  Returns 0 if
  // there are no handle bits set (<size_> == 0).

#if defined (ACE_HAS_BIG_FD_SET)
  ACE_Handle_Set & operator= (const ACE_Handle_Set &);
  // Assignment operator optimizes for cases where <size_> == 0.
#endif /* ACE_HAS_BIG_FD_SET */

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  int size_;
  // Size of the set, i.e., a count of the number of enabled bits.

  ACE_HANDLE max_handle_;
  // Current max handle.

#if defined (ACE_HAS_BIG_FD_SET)
  ACE_HANDLE min_handle_;
  // Current min handle.
#endif /* ACE_HAS_BIG_FD_SET */

  fd_set mask_;
  // Bitmask.

  enum
  {
    WORDSIZE = NFDBITS,
#if !defined (ACE_WIN32)
    NUM_WORDS = howmany (MAXSIZE, NFDBITS),
#endif /* ACE_WIN32 */
    NBITS = 256
  };

  static int count_bits (u_long n);
  // Counts the number of bits enabled in N.  Uses a table lookup to
  // speed up the count.

#if defined (ACE_HAS_BIG_FD_SET)
  static int bitpos (u_long bit);
  // Find the position of the bit counting from right to left.
#endif /* ACE_HAS_BIG_FD_SET */

  void set_max (ACE_HANDLE max);
  // Resets the <max_handle_> after a clear of the original
  // <max_handle_>.

  static const char nbits_[NBITS];
  // Table that maps bytes to counts of the enabled bits in each value
  // from 0 to 255.
};

class ACE_Export ACE_Handle_Set_Iterator
{
  // = TITLE
  //     Iterator for the <ACE_Handle_Set> abstraction.
public:
  ACE_Handle_Set_Iterator (const ACE_Handle_Set &hs);
  // Constructor.

  ~ACE_Handle_Set_Iterator (void);
  // Default dtor.

  ACE_HANDLE operator () (void);
  // "Next" operator.  Returns the next unseen <ACE_HANDLE> in the
  // <Handle_Set> up to <handle_set_.max_handle_>).  When all the
  // handles have been seen returns <ACE_INVALID_HANDLE>.  Advances
  // the iterator automatically, so you need not call <operator++>
  // (which is now obsolete).

  void operator++ (void);
  // This is a no-op and no longer does anything.  It's only here for
  // backwards compatibility.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  const ACE_Handle_Set &handles_;
  // The <Handle_Set> we are iterating through.

#if defined (ACE_WIN32)
  u_int handle_index_;
#elif !defined (ACE_HAS_BIG_FD_SET)
  int handle_index_;
#elif defined (ACE_HAS_BIG_FD_SET)
  int handle_index_;
  u_long oldlsb_;
#endif /* ACE_WIN32 */
  // Index of the bit we're examining in the current <word_num_> word.

  int word_num_;
  // Number of the word we're iterating over (typically between 0..7).

#if defined (ACE_HAS_BIG_FD_SET)
  int word_max_;
  // Number max of the words with a possible bit on.
#endif /* ACE_HAS_BIG_FD_SET */

#if !defined (ACE_WIN32) && !defined (ACE_HAS_BIG_FD_SET)
  fd_mask word_val_;
  // Value of the bits in the word we're iterating on.
#elif !defined (ACE_WIN32) && defined (ACE_HAS_BIG_FD_SET)
  u_long word_val_;
  // Value of the bits in the word we're iterating on.
#endif /* !ACE_WIN32 && !ACE_HAS_BIG_FD_SET */
};

#if defined (__ACE_INLINE__)
#include "ace/Handle_Set.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_HANDLE_SET */
