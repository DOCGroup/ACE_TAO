// -*- C++ -*-

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

#include /**/ "ace/pre.h"

#include "ace/String_Base_Const.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Global_Macros.h"
#include "ace/OS_String.h"
#include "ace/OS_Memory.h"


// Forward decl.
class ACE_Allocator;

/**
 * @class ACE_String_Base
 *
 * @brief This class provides a wrapper facade for C strings.
 *
 * This class uses an ACE_Allocator to allocate memory.  The
 * user can make this a persistant class by providing an
 * ACE_Allocator with a persistable memory pool.  This class is
 * optimized for efficiency, so it doesn't provide any internal
 * locking.
 * NOTE: if an instance of this class is constructed from or
 * assigned an empty string (with first element of '\0'), then it
 * is not allocated new space.  Instead, its internal
 * representation is set equal to a global empty string.
 * CAUTION: in cases when ACE_String_Base is constructed from a
 * provided buffer with the release parameter set to 0,
 * ACE_String_Base is not guaranteed to be '\0' terminated.
 */
template <class CHAR> class ACE_String_Base : public ACE_String_Base_Const
{
public:
   /**
    *  Default constructor.
    *
    *  @param the_allocator ACE_Allocator associated with string
    *  @return Default ACE_String_Base string.
    */
  ACE_String_Base (ACE_Allocator *the_allocator = 0);

  /**
   * Constructor that copies @a s into dynamically allocated memory.
   * If @a release is non-0 then the @a ACE_Allocator is responsible for
   * freeing this memory. Memory is _not_ allocated/freed if @a release
   * is 0.
   *
   * @param s Zero terminated input string
   * @param the_allocator ACE_Allocator associated with string
   * @param release Allocator responsible(1)/not reponsible(0) for
   *    freeing memory.
   * @return ACE_String_Base containing const CHAR *s
   */
  ACE_String_Base (const CHAR *s,
                   ACE_Allocator *the_allocator = 0,
                   int release = 1);

  /**
   * Constructor that copies @a len CHARs of @a s into dynamically
   * allocated memory (will zero terminate the result).  If @a release
   * is non-0 then the @a ACE_allocator is responsible for freeing this
   * memory.  Memory is _not_ allocated/freed if @a release is 0.
   *
   * @param s Non-zero terminated input string
   * @param len Length of non-zero terminated input string
   * @param the_allocator ACE_Allocator associated with string
   * @param release Allocator responsible(1)/not reponsible(0) for
   *    freeing memory.
   * @return ACE_String_Base containing const CHAR *s
   */
  ACE_String_Base (const CHAR *s,
                   size_t len,
                   ACE_Allocator *the_allocator = 0,
                   int release = 1);

  /**
   *  Copy constructor.
   *
   *  @param s Input ACE_String_Base string to copy
   *  @return Copy of input string @a s
   */
  ACE_String_Base (const ACE_String_Base < CHAR > &s);

  /**
   *  Constructor that copies @a c into dynamically allocated memory.
   *
   *  @param c Single input character.
   *  @param the_allocator ACE_Allocator associated with string
   *  @return ACE_String_Base containing CHAR 'c'
   */
  ACE_String_Base (CHAR c,
                   ACE_Allocator *the_allocator = 0);

  /**
   *  Constructor that dynamically allocate @a len long of char array
   *  and initialize it to @a c using @a alloc to allocate the memory.
   *
   *  @param len Length of character array 'c'
   *  @param c Input character array
   *  @param the_allocator ACE_Allocator associated with string
   *  @return ACE_String_Base containing character array 'c'
   */
  ACE_String_Base (size_t len,
                   CHAR c = 0,
                   ACE_Allocator *the_allocator = 0);

  /**
   *  Deletes the memory...
   */
  ~ACE_String_Base (void);

  /**
   * Return the <slot'th> character in the string (doesn't perform
   * bounds checking).
   *
   * @param slot Index of the desired character
   * @return The character at index @a slot
   */
  const CHAR & operator[] (size_t slot) const;

  /**
   * Return the <slot'th> character by reference in the string
   * (doesn't perform bounds checking).
   *
   * @param slot Index of the desired character
   * @return The character at index @a slot
   */
  CHAR & operator[] (size_t slot);

  /**
   *  Assignment operator (does copy memory).
   *
   *  @param s Input ACE_String_Base string to assign to this object.
   *  @return Return a copy of the this string.
   */
  ACE_String_Base < CHAR > &operator = (const ACE_String_Base < CHAR > &s);

  /**
   *  Assignment alternative method (does not copy memory).
   *
   *  @param s Input ACE_String_Base string to assign to this object.
   *  @return Return this string.
   */
  ACE_String_Base < CHAR > &assign_nocopy (const ACE_String_Base < CHAR > &s);

  /**
   * Copy @a s into this @a ACE_String_Base.  Memory is _not_
   * allocated/freed if @a release is 0.
   *
   * @param s Null terminated input string
   * @param release Allocator responsible(1)/not reponsible(0) for
   *    freeing memory.
   */
  void set (const CHAR * s, int release = 1);

  /**
   *  Copy @a len bytes of @a s (will zero terminate the result).
   *  Memory is _not_ allocated/freed if @a release is 0.
   *
   *  @param s Non-zero terminated input string
   *  @param len Length of input string 's'
   *  @param release Allocator responsible(1)/not reponsible(0) for
   *    freeing memory.
   */
  void set (const CHAR * s, size_t len, int release);

  /**
   * Clear this string. Memory is _not_ freed if <release> is 0.
   *
   * @param release Memory is freed if 1 or not if 0.
   */
  void clear (int release = 0);

  /**
   * Return a substring given an offset and length, if length == -1
   * use rest of str.  Return empty substring if offset or
   * offset/length are invalid.
   *
   * @param offset Index of first desired character of the substring.
   * @param length How many characters to return starting at the offset.
   * @return The string containing the desired substring
   */
  ACE_String_Base < CHAR > substring (size_t offset,
                                      ssize_t length = -1) const;

  /**
   *  Same as <substring>.
   *
   * @param offset Index of first desired character of the substring.
   * @param length How many characters to return starting at the offset.
   * @return The string containing the desired substring
   */
  ACE_String_Base < CHAR > substr (size_t offset, ssize_t length = -1) const;

  /**
   *  Concat operator (copies memory).
   *
   *  @param s Input ACE_String_Base string to concatenate to another string.
   *  @return The combined string (input append to the end of the old). New
   *    string is zero terminated.
   */
  ACE_String_Base < CHAR > &operator += (const ACE_String_Base < CHAR > &s);

  /**
   *  Returns a hash value for this string.
   *
   *  @return Hash value of string
   */
  u_long hash (void) const;

  /**
   *  Return the length of the string.
   *
   *  @return Length of stored string
   */
  size_t length (void) const;

  /**
   * Get a copy of the underlying representation.
   *
   * This method allocates memory for a copy of the string and returns
   * a pointer to the new area. The caller is responsible for freeing
   * the memory when finished; use delete []
   *
   * @return Pointer reference to the string data. Returned string is
   *    zero terminated.
   */
  CHAR *rep (void) const;

  /**
   * Get at the underlying representation directly!
   * _Don't_ even think about casting the result to (char *) and modifying it,
   * if it has length 0!
   *
   * @return Pointer reference to the stored string data. No guarantee is
   *    that the string is zero terminated.
   *
   */
  const CHAR *fast_rep (void) const;

  /**
   *  Same as STL String's <c_str> and <fast_rep>.
   */
  const CHAR *c_str (void) const;

  /**
   *  Comparison operator that will match substrings.  Returns the
   *  slot of the first location that matches, else -1.
   *
   *  @param s Input ACE_String_Base string
   *  @return Integer index value of the first location of string @a s or
   *    -1 (not found).
   */
  ssize_t strstr (const ACE_String_Base<CHAR> &s) const;

  /**
   *  Find <str> starting at pos.  Returns the slot of the first
   *  location that matches (will be >= pos), else npos.
   *
   *  @param str Input ACE_String_Base string to search for in stored string.
   *  @param pos Starting index position to start searching for string @a str.
   *  @return Index value of the first location of string @a str else npos.
   */
  ssize_t find (const ACE_String_Base<CHAR> &str, size_t pos = 0) const;

  /**
   *  Find @a s starting at pos.  Returns the slot of the first
   *  location that matches (will be >= pos), else npos.
   *
   *  @param s non-zero input string to search for in stored string.
   *  @param pos Starting index position to start searching for string @a str.
   *  @return Index value of the first location of string @a str else npos.
   */
  ssize_t find (const CHAR *s, size_t pos = 0) const;

  /**
   *  Find @a c starting at pos.  Returns the slot of the first
   *  location that matches (will be >= pos), else npos.
   *
   *  @param c Input character to search for in stored string.
   *  @param pos Starting index position to start searching for string @a str.
   *  @return Index value of the first location of string @a str else npos.
   */
  ssize_t find (CHAR c, size_t pos = 0) const;

  /**
   *  Find @a c starting at pos (counting from the end).  Returns the
   *  slot of the first location that matches, else npos.
   *
   *  @param c Input character to search for in stored string.
   *  @param pos Starting index position to start searching for string @a str.
   *  @return Index value of the first location of string @a str else npos.
   */
  ssize_t rfind (CHAR c, ssize_t pos = npos) const;

  /**
   *  Equality comparison operator (must match entire string).
   *
   * @param s Input ACE_String_Base string to compare against stored string.
   * @return Integer value of result (1 = found, 0 = not found).
   */
  int operator == (const ACE_String_Base<CHAR> &s) const;

  /**
   *  Less than comparison operator.
   *
   *  @param s Input ACE_String_Base string to compare against stored string.
   *  @return Integer value of result (1 = less than, 0 = greater than or
   *  equal).
   */
  int operator < (const ACE_String_Base<CHAR> &s) const;

  /**
   *  Greater than comparison operator.
   *
   *  @param s Input ACE_String_Base string to compare against stored string.
   *  @return Integer value of result (1 = greater than, 0 = less than or
   *  equal).
   */
  int operator > (const ACE_String_Base<CHAR> &s) const;

  /**
   *  Inequality comparison operator.
   *
   *  @param s Input ACE_String_Base string to compare against stored string.
   *  @return Integer value of result (1 = not equal, 0 = equal).
   */
  int operator != (const ACE_String_Base<CHAR> &s) const;

  /**
   *  Performs a strncmp comparison.
   *
   *  @param s Input ACE_String_Base string to compare against stored string.
   *  @return Integer value of result (less than 0, 0, greater than 0)
   *    depending on how input string @a s is to the stored string.
   */
  int compare (const ACE_String_Base<CHAR> &s) const;

  /**
   *  Dump the state of an object.
   */
  void dump (void) const;

  /**
   * This method is designed for high-performance. Please use with
   * care ;-) If the current size of the string is less than <len>,
   * the string is resized to the new length. The data is zero'd
   * out after this operation.
   *
   * @param len New string size
   * @param c New input string
   */
  void resize (size_t len, CHAR c = 0);

  /**
   *  Declare the dynamic allocation hooks.
   */
  ACE_ALLOC_HOOK_DECLARE;

protected:
  /**
   *  Pointer to a memory allocator.
   */
  ACE_Allocator *allocator_;

  /**
   *  Length of the ACE_String_Base data (not counting the trailing '\0').
   */
  size_t len_;

  /**
   *  Length of the ACE_String_Base data buffer.  Keeping track of the
   *  length allows to avoid unnecessary dynamic allocations.
   */
  size_t buf_len_;

  /**
   *  Pointer to data.
   */
  CHAR *rep_;

  /**
   *  Flag that indicates if we own the memory
   */
  int release_;

  /**
   *  Represents the "NULL" string to simplify the internal logic.
   */
  static CHAR NULL_String_;
};

template < class CHAR > ACE_INLINE
  ACE_String_Base < CHAR > operator + (const ACE_String_Base < CHAR > &,
                                       const ACE_String_Base < CHAR > &);
template < class CHAR > ACE_INLINE
  ACE_String_Base < CHAR > operator + (const ACE_String_Base < CHAR > &,
                                       const CHAR *);
template < class CHAR > ACE_INLINE
  ACE_String_Base < CHAR > operator + (const CHAR *,
                                       const ACE_String_Base < CHAR > &);

template < class CHAR > ACE_INLINE
  ACE_String_Base < CHAR > operator + (const ACE_String_Base < CHAR > &t,
                                       const CHAR c);

template < class CHAR > ACE_INLINE
  ACE_String_Base < CHAR > operator + (const CHAR c,
                                       const ACE_String_Base < CHAR > &t);

#if defined (__ACE_INLINE__)
#include "ace/String_Base.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/String_Base.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("String_Base.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* ACE_STRING_BASE_H */
