/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ACE
//
// = FILENAME
//    Vector.h
//
// = AUTHOR
//    Ossama Othman <othman@cs.wustl.edu>
//
// ============================================================================

#ifndef ACE_VECTOR_H
#define ACE_VECTOR_H

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Default size of the block the ACE_Vector should grow by.
#if !defined (ACE_DEFAULT_VECTOR_BLOCK_SIZE)
# define ACE_DEFAULT_VECTOR_BLOCK_SIZE 4
#endif

// Forward declarations
class ACE_Allocator;

template <class T>
class ACE_Vector
{
  // = TITLE
  //     Implementation of an STL-like Vector
  //
  // = DESCRIPTION
  //     This implementation is based on the ACE_Array implementation.
  //     However, it provides an STL Vector-like interface.
public:

  // Define a trait
  typedef T* iterator;

  // = Constructors and Destructors

  ACE_Vector (size_t n = 0,
              ACE_Allocator *alloc = 0);
  // Dynamically create an uninitialized array.

  ACE_Vector (size_t size,
              const T &default_value,
              ACE_Allocator *alloc = 0);
  // Dynamically initialize the entire array to the <default_value>.

  ACE_Vector (const ACE_Vector<T>& vector);
  // The copy constructor performs initialization by making an exact
  // copy of the contents of parameter <s>, i.e., *this == s will
  // return true.

  ~ACE_Vector (void);
  // The destructor

  // = Accessor Methods

  T& front (void);
  // Returns the first element

  const T& front (void) const;
  // Returns the first element

  T& back (void);
  // Returns the last element

  const T& back (void) const;
  // Returns the last element

  void push_back (const T&);
  // Inserts a new element at the end

  void pop_back (void);
  // Removes the last element

  void swap (ACE_Vector<T>&);
  // Swaps the contents of two vectors

  // = Iterators

  iterator begin (void);
  // Returns an iterator pointing to the beginning of the vector

  const iterator begin (void) const;
  // Returns a constant iterator pointing to the beginning of the vector

  iterator end (void);
  // Returns an iterator pointing to the end of the vector. 

  const iterator end (void) const;
  // Returns a constant iterator pointing to the end of the vector.

  // = Utility Methods

  size_t size (void) const;
  // Returns the number of elements in the ACE_Vector

  size_t max_size (void) const;
  // Returns the largest possible size of the ACE_Vector

  size_t capacity (void) const;
  // Number of elements for which memory has been
  // allocated. capacity() is always greater than or equal to size()

  int reserve (size_t n);
  // If n is less than or equal to capacity(), this call has no
  // effect. Otherwise, it is a request for allocation of additional
  // memory. If the request is successful, then capacity() is greater
  // than or equal to n; otherwise, capacity() is unchanged. In either
  // case, size() is unchanged.

  int empty (void) const;
  // Returns 1 (true) if the ACE_Vector is empty

  void operator= (const ACE_Vector<T> &rhs);
  // Assignment operator performs an assignment by making an exact
  // copy of the contents of parameter <rhs>, i.e., *this == rhs will
  // return true.  Note that if the <capacity_> of <vector_> is >=
  // <s.capacity_> we can copy it without reallocating.  However, if
  // <capacity_> is < <s.capacity_> we must delete the <vector_>,
  // reallocate a new <vector_>, and then copy the contents of <rhs>.

  T &operator[] (size_t index);
  // Set item in the array at location <index>.  Doesn't
  // perform range checking.

  const T &operator[] (size_t index) const;
  // Get item in the array at location <index>.  Doesn't
  // perform range checking.

  int operator== (const ACE_Vector<T>& rhs) const;
  // Tests two vectors for equality.

  int operator< (const ACE_Vector<T>& rhs) const;
  // Lexicographical comparison.

  int operator!= (const ACE_Vector<T> &rhs) const;
  // Compare this array with <s> for inequality such that <*this> !=
  // <s> is always the complement of the boolean return value of
  // <*this> == <s>.

private:
  size_t size_;
  // The current number of elements in the ACE_Vector.

  size_t capacity_;
  // Number of elements for which memory has been allocated.

  T* vector_;
  // The underlying array.

  ACE_Allocator *allocator_;
  // Allocation strategy of the ACE_Array_Base.
};

#if defined (__ACE_INLINE__)
#include "ace/Vector.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Vector.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Vector.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif  /* ACE_VECTOR_H */
