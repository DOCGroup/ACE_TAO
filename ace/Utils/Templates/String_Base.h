/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    String_Base.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_STRING_BASE_H
#define ACE_STRING_BASE_H
#include "ace/pre.h"

#include "ace/ACE.h"
#include "ace/Utils/String_Base_Const.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward decl.
class ACE_Allocator;

/**
 * @class ACE_String_Base
 *
 * @brief This class provides a wrapper facade for C strings.
 *
 * This class uses an <ACE_Allocator> to allocate memory.  The
 * user can make this a persistant class by providing an
 * ACE_Allocator with a persistable memory pool.  This class is
 * optimized for efficiency, so it doesn't provide any internal
 * locking.
 * NOTE: if an instance of this class is constructed from or
 * assigned an empty string (with first element of '\0'), then it
 * is _not_ allocated new space.  Instead, its internal
 * representation is set equal to a global empty string.
 * CAUTION: in cases when ACE_String_Base is constructed from a
 * provided buffer with the release parameter set to 0,
 * ACE_String_Base is not guaranteed to be '\0' terminated.
 */
template <class CHAR>
class ACE_Export ACE_String_Base : public ACE_String_Base_Const
{
public:
  /// Default constructor.
  ACE_String_Base (ACE_Allocator *alloc = 0);

  /**
   * Constructor that copies @a s into dynamically allocated memory.
   * If @a release is non-0 then the @a ACE_allocator is responsible for
   * freeing this memory. Memory is _not_ allocated/freed if @a release
   * is 0.
   */
  ACE_String_Base (const CHAR *s,
                   ACE_Allocator *alloc = 0,
                   int release = 1);

  /**
   * Constructor that copies <len> CHARs of <s> into dynamically
   * allocated memory (will NUL terminate the result).  If <release>
   * is non-0 then the <ACE_allocator> is responsible for freeing this
   * memory.  Memory is _not_ allocated/freed if <release> is 0.
   */
  ACE_String_Base (const CHAR *s,
                   size_t len,
                   ACE_Allocator *alloc = 0,
                   int release = 1);

  /// Copy constructor.
  ACE_String_Base (const ACE_String_Base<CHAR> &);

  /// Constructor that copies @a c into dynamically allocated memory.
  ACE_String_Base (CHAR c, ACE_Allocator *alloc = 0);

  /// Constructor that dynamically allocate @a len long of char array
  /// and initialize it to @a c using @a alloc to allocate the memory.
  ACE_String_Base (size_t len, CHAR c = 0, ACE_Allocator *alloc = 0);

  /// Deletes the memory...
  ~ACE_String_Base (void);

  /// Return the <slot'th> character in the string (doesn't perform
  /// bounds checking).
  const CHAR &operator [] (size_t slot) const;

  /// Return the <slot'th> character by reference in the string
  /// (doesn't perform bounds checking).
  CHAR &operator [] (size_t slot);

  /// Assignment operator (does copy memory).
  ACE_String_Base<CHAR> &operator = (const ACE_String_Base<CHAR> &);

  /// Copy @a s into this @a ACE_String_Base.  Memory is _not_
  /// allocated/freed if @a release is 0.
  void set (const CHAR *s, int release = 1);

  /// Copy @a len bytes of @a s (will NUL terminate the result).
  /// Memory is _not_ allocated/freed if @a release is 0.
  void set (const CHAR *s,
            size_t len,
            int release);

  /// Clear this string. Memory is _not_ freed is <release> is 0.
  void clear (int release = 0);

  /**
   * Return a substring given an offset and length, if length == -1
   * use rest of str.  Return empty substring if offset or
   * offset/length are invalid.
   */
  ACE_String_Base<CHAR> substring (size_t offset, ssize_t length = -1) const;

  /// Same as <substring>.
  ACE_String_Base<CHAR> substr (size_t offset, ssize_t length = -1) const;

  /// Concat operator (copies memory).
  ACE_String_Base<CHAR> &operator += (const ACE_String_Base<CHAR> &);

  /// Returns a hash value for this string.
  u_long hash (void) const;

  /// Return the length of the string.
  size_t length (void) const;

  /// Get a copy of the underlying representation.
  CHAR *rep (void) const;

  /**
   * Get at the underlying representation directly!
   * _Don't_ even think about casting the result to (char *) and modifying it,
   * if it has length 0!
   */
  const CHAR *fast_rep (void) const;

  /// Same as STL String's <c_str> and <fast_rep>.
  const CHAR *c_str (void) const;

  /// Comparison operator that will match substrings.  Returns the
  /// slot of the first location that matches, else -1.
  int strstr (const ACE_String_Base<CHAR> &s) const;

  /// Find <str> starting at pos.  Returns the slot of the first
  /// location that matches (will be >= pos), else npos.
  int find (const ACE_String_Base<CHAR> &str, int pos = 0) const;

  /// Find <s> starting at pos.  Returns the slot of the first
  /// location that matches (will be >= pos), else npos.
  int find (const CHAR *s, int pos = 0) const;

  /// Find <c> starting at pos.  Returns the slot of the first
  /// location that matches (will be >= pos), else npos.
  int find (CHAR c, int pos = 0) const;

  /// Find <c> starting at pos (counting from the end).  Returns the
  /// slot of the first location that matches, else npos.
  int rfind (CHAR c, int pos = npos) const;

  /// Equality comparison operator (must match entire string).
  int operator == (const ACE_String_Base<CHAR> &s) const;

  /// Less than comparison operator.
  int operator < (const ACE_String_Base<CHAR> &s) const;

  /// Greater than comparison operator.
  int operator > (const ACE_String_Base<CHAR> &s) const;

  /// Inequality comparison operator.
  int operator != (const ACE_String_Base<CHAR> &s) const;

  /// Performs a <strcmp>-style comparison.
  int compare (const ACE_String_Base<CHAR> &s) const;

  /// Dump the state of an object.
  void dump (void) const;

  /**
   * This method is designed for high-performance. Please use with
   * care ;-) If the current size of the string is less than <len>,
   * the string is resized to the new length. The data is zero'd
   * out after this operation.
   */
  void resize (size_t len, CHAR c = 0);

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  /// Pointer to a memory allocator.
  ACE_Allocator *allocator_;

  /// Length of the ACE_String_Base data (not counting the trailing '\0').
  size_t len_;

  /// Length of the ACE_String_Base data buffer.  Keeping track of the
  /// length allows to avoid unnecessary dynamic allocations.
  size_t buf_len_;

  /// Pointer to data.
  CHAR *rep_;

  /// Flag that indicates if we own the memory
  int release_;

  /// Represents the "NULL" string to simplify the internal logic.
  static CHAR NULL_String_;
};

template <class CHAR> ACE_INLINE
ACE_String_Base<CHAR> operator + (const ACE_String_Base<CHAR> &,
                                  const ACE_String_Base<CHAR> &);
template <class CHAR> ACE_INLINE
ACE_String_Base<CHAR> operator + (const ACE_String_Base<CHAR> &,
                                  const CHAR *);
template <class CHAR> ACE_INLINE
ACE_String_Base<CHAR> operator + (const CHAR* ,
                                  const ACE_String_Base<CHAR> &);
#if defined (__ACE_INLINE__)
#include "ace/Utils/Templates/String_Base.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Utils/Templates/String_Base.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("String_Base.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* ACE_STRING_BASE_H */
