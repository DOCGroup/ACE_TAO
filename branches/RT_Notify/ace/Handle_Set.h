/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Handle_Set.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_HANDLE_SET_H
#define ACE_HANDLE_SET_H
#include "ace/pre.h"

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ACE_Handle_Set
 *
 * @brief C++ wrapper facade for the socket <fd_set> abstraction.
 *
 * This abstraction is a very efficient wrapper facade over
 * <fd_set>.  In particular, no range checking is performed, so
 * it's important not to set or clear bits that are outside the
 * <ACE_DEFAULT_SELECT_REACTOR_SIZE>.
 */
class ACE_Export ACE_Handle_Set
{
public:
  friend class ACE_Handle_Set_Iterator;

  // = Initialization and termination.

  enum
  {
    MAXSIZE = ACE_DEFAULT_SELECT_REACTOR_SIZE
  };

  // = Initialization methods.
  /// Constructor, initializes the bitmask to all 0s.
  ACE_Handle_Set (void);

  /**
   * Constructor, initializes the handle set from a given mask.
   * <ACE_FD_SET_TYPE> is a <typedef> based on the platform's native
   * type used for masks passed to <select>.
   */
  ACE_Handle_Set (const ACE_FD_SET_TYPE &mask);

#if defined (ACE_HAS_WINCE)
  /// Default dtor.
  ~ACE_Handle_Set (void);
#endif /* ACE_HAS_WINCE */

  // = Methods for manipulating bitsets.
  /// Initialize the bitmask to all 0s and reset the associated fields.
  void reset (void);

  /**
   * Checks whether <handle> is enabled.  No range checking is
   * performed so <handle> must be less than
   * <ACE_DEFAULT_SELECT_REACTOR_SIZE>.
   */
  int is_set (ACE_HANDLE handle) const;

  /// Enables the <handle>.  No range checking is performed so <handle>
  /// must be less than <ACE_DEFAULT_SELECT_REACTOR_SIZE>.
  void set_bit (ACE_HANDLE handle);

  /// Disables the <handle>.  No range checking is performed so
  /// <handle> must be less than <ACE_DEFAULT_SELECT_REACTOR_SIZE>.
  void clr_bit (ACE_HANDLE handle);

  /// Returns a count of the number of enabled bits.
  int num_set (void) const;

  /// Returns the number of the large bit.
  ACE_HANDLE max_set (void) const;

  /**
   * Rescan the underlying <fd_set> up to handle <max> to find the new
   * <max_handle> (highest bit set) and <size> (how many bits set) values.
   * This is useful for evaluating the changes after the handle set has
   * been manipulated in some way other than member functions; for example,
   * after <select> modifies the <fd_set>.
   */
  void sync (ACE_HANDLE max);

  /// Returns a pointer to the underlying <fd_set>.  Returns 0 if
  /// there are no handle bits set (<size_> == 0).
  operator fd_set *();

  /// Returns a pointer to the underlying <fd_set>.  Returns 0 if
  /// there are no handle bits set (<size_> == 0).
  fd_set *fdset (void);

#if defined (ACE_HAS_BIG_FD_SET)
  /// Assignment operator optimizes for cases where <size_> == 0.
  ACE_Handle_Set & operator= (const ACE_Handle_Set &);
#endif /* ACE_HAS_BIG_FD_SET */

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  /// Size of the set, i.e., a count of the number of enabled bits.
  int size_;

  /// Current max handle.
  ACE_HANDLE max_handle_;

#if defined (ACE_HAS_BIG_FD_SET)
  /// Current min handle.
  ACE_HANDLE min_handle_;
#endif /* ACE_HAS_BIG_FD_SET */

  /// Bitmask.
  fd_set mask_;

  enum
  {
    WORDSIZE = NFDBITS,
#if !defined (ACE_WIN32)
    NUM_WORDS = howmany (MAXSIZE, NFDBITS),
#endif /* ACE_WIN32 */
    NBITS = 256
  };

  /// Counts the number of bits enabled in N.  Uses a table lookup to
  /// speed up the count.
  static int count_bits (u_long n);

#if defined (ACE_HAS_BIG_FD_SET)
  /// Find the position of the bit counting from right to left.
  static int bitpos (u_long bit);
#endif /* ACE_HAS_BIG_FD_SET */

  /// Resets the <max_handle_> after a clear of the original
  /// <max_handle_>.
  void set_max (ACE_HANDLE max);

  /// Table that maps bytes to counts of the enabled bits in each value
  /// from 0 to 255.
  static const char nbits_[NBITS];
};

/**
 * @class ACE_Handle_Set_Iterator
 *
 * @brief Iterator for the <ACE_Handle_Set> abstraction.
 */
class ACE_Export ACE_Handle_Set_Iterator
{
public:
  /// Constructor.
  ACE_Handle_Set_Iterator (const ACE_Handle_Set &hs);

  /// Default dtor.
  ~ACE_Handle_Set_Iterator (void);

  /// Reset the state of the iterator by reinitializing the state
  /// that we maintain.
  void reset_state (void);

  /**
   * "Next" operator.  Returns the next unseen <ACE_HANDLE> in the
   * <Handle_Set> up to <handle_set_.max_handle_>).  When all the
   * handles have been seen returns <ACE_INVALID_HANDLE>.  Advances
   * the iterator automatically, so you need not call <operator++>
   * (which is now obsolete).
   */
  ACE_HANDLE operator () (void);

  /// This is a no-op and no longer does anything.  It's only here for
  /// backwards compatibility.
  void operator++ (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  /// The <Handle_Set> we are iterating through.
  const ACE_Handle_Set &handles_;

#if defined (ACE_WIN32)
  u_int handle_index_;
#elif !defined (ACE_HAS_BIG_FD_SET)
  int handle_index_;
#elif defined (ACE_HAS_BIG_FD_SET)
  int handle_index_;
  u_long oldlsb_;
#endif /* ACE_WIN32 */
  // Index of the bit we're examining in the current <word_num_> word.

  /// Number of the word we're iterating over (typically between 0..7).
  int word_num_;

#if defined (ACE_HAS_BIG_FD_SET)
  /// Number max of the words with a possible bit on.
  int word_max_;
#endif /* ACE_HAS_BIG_FD_SET */

#if !defined (ACE_WIN32) && !defined (ACE_HAS_BIG_FD_SET)
  /// Value of the bits in the word we're iterating on.
  fd_mask word_val_;
#elif !defined (ACE_WIN32) && defined (ACE_HAS_BIG_FD_SET)
  /// Value of the bits in the word we're iterating on.
  u_long word_val_;
#endif /* !ACE_WIN32 && !ACE_HAS_BIG_FD_SET */
};

#if defined (__ACE_INLINE__)
#include "ace/Handle_Set.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_HANDLE_SET */
