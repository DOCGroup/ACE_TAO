// -*- C++ -*-

//=============================================================================
/**
 *  @file Array_Base.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_ARRAY_BASE_H
#define ACE_ARRAY_BASE_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Global_Macros.h"
#include "ace/OS.h"
#include "ace/Malloc_Base.h"

// Forward declaration.
template <class T> class ACE_Array_Iterator;

/**
 * @class ACE_Array_Base
 *
 * @brief Implement a simple dynamic array
 *
 * This parametric class implements a simple dynamic array;
 * resizing must be controlled by the user. No comparison or find
 * operations are implemented.
 */
template<class T>
class ACE_Array_Base
{
public:

  // Define a "trait"
  typedef T TYPE;
  typedef ACE_Array_Iterator<T> ITERATOR;

  // = Initialization and termination methods.

  /// Dynamically create an uninitialized array.
  ACE_Array_Base (size_t size = 0,
                  ACE_Allocator *the_allocator = 0);

  /// Dynamically initialize the entire array to the <default_value>.
  ACE_Array_Base (size_t size,
                  const T &default_value,
                  ACE_Allocator *the_allocator = 0);

  /**
   * The copy constructor performs initialization by making an exact
   * copy of the contents of parameter <s>, i.e., *this == s will
   * return true.
   */
  ACE_Array_Base (const ACE_Array_Base<T> &s);

  /**
   * Assignment operator performs an assignment by making an exact
   * copy of the contents of parameter <s>, i.e., *this == s will
   * return true.  Note that if the <max_size_> of <array_> is >= than
   * <s.max_size_> we can copy it without reallocating.  However, if
   * <max_size_> is < <s.max_size_> we must delete the <array_>,
   * reallocate a new <array_>, and then copy the contents of <s>.
   */
  void operator= (const ACE_Array_Base<T> &s);

  /// Clean up the array (e.g., delete dynamically allocated memory).
  ~ACE_Array_Base (void);

  // = Set/get methods.

  /// Set item in the array at location <slot>.  Doesn't
  /// perform range checking.
  T &operator [] (size_t slot);

  /// Get item in the array at location <slot>.  Doesn't
  /// perform range checking.
  const T &operator [] (size_t slot) const;

  /// Set an item in the array at location <slot>.  Returns
  /// -1 if <slot> is not in range, else returns 0.
  int set (const T &new_item, size_t slot);

  /**
   * Get an item in the array at location <slot>.  Returns -1 if
   * <slot> is not in range, else returns 0.  Note that this function
   * copies the item.  If you want to avoid the copy, you can use
   * the const operator [], but then you'll be responsible for range checking.
   */
  int get (T &item, size_t slot) const;

  /// Returns the <cur_size_> of the array.
  size_t size (void) const;

  /**
   * Changes the size of the array to match <new_size>.
   * It copies the old contents into the new array.
   * Return -1 on failure.
   */
  int size (size_t new_size);

  /// Returns the <max_size_> of the array.
  size_t max_size (void) const;

  /**
   * Changes the size of the array to match <new_size>.
   * It copies the old contents into the new array.
   * Return -1 on failure.
   * It does not affect new_size
   */
  int max_size (size_t new_size);

private:
  /// Returns 1 if <slot> is within range, i.e., 0 >= <slot> <
  /// <cur_size_>, else returns 0.
  int in_range (size_t slot) const;

  /// Maximum size of the array, i.e., the total number of <T> elements
  /// in <array_>.
  size_t max_size_;

  /**
   * Current size of the array.  This starts out being == to
   * <max_size_>.  However, if we are assigned a smaller array, then
   * <cur_size_> will become less than <max_size_>.  The purpose of
   * keeping track of both sizes is to avoid reallocating memory if we
   * don't have to.
   */
  size_t cur_size_;

  /// Pointer to the array's storage buffer.
  T *array_;

  /// Allocation strategy of the ACE_Array_Base.
  ACE_Allocator *allocator_;

  friend class ACE_Array_Iterator<T>;
};

// ****************************************************************

/**
 * @class ACE_Array_Iterator
 *
 * @brief Implement an iterator over an ACE_Array.
 *
 * This iterator is safe in the face of array element deletions.
 * But it is NOT safe if the array is resized (via the ACE_Array
 * assignment operator) during iteration.  That would be very
 * odd, and dangerous.
 */
template <class T>
class ACE_Array_Iterator
{
public:
  // = Initialization method.
  ACE_Array_Iterator (ACE_Array_Base<T> &);

  // = Iteration methods.

  /// Pass back the <next_item> that hasn't been seen in the Array.
  /// Returns 0 when all items have been seen, else 1.
  int next (T *&next_item);

  /// Move forward by one element in the Array.  Returns 0 when all the
  /// items in the Array have been seen, else 1.
  int advance (void);

  /// Returns 1 when all items have been seen, else 0.
  int done (void) const;

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  /// Pointer to the current item in the iteration.
  size_t current_;

  /// Pointer to the Array we're iterating over.
  ACE_Array_Base<T> &array_;
};

#if defined (__ACE_INLINE__)
#include "ace/Array_Base.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Array_Base.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Array_Base.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"

#endif /* ACE_ARRAY_BASE_H */
