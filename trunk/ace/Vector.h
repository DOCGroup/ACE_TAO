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

// Default ACE_Vector growth factor.
// ACE_Vector will grow by
//     ACE_DEFAULT_VECTOR_GROWTH_FACTOR * ACE_Vector::capacity()
// in order to ensure insertion a series of elements into a vector is
// a linear time operation.  If growth is not proportional to
// ACE_Vector::capacity(), and instead grows by a constant amount,
// insertion becomes a quadratic time operation.
#if !defined (ACE_DEFAULT_VECTOR_GROWTH_FACTOR)
# define ACE_DEFAULT_VECTOR_GROWTH_FACTOR 2
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

  ACE_Vector (ACE_Vector<T>::iterator first,
              ACE_Vector<T>::iterator last,
              ACE_Allocator *alloc = 0);
  // Creates an ACE_Vector with a copy of a range.

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
  // Inserts a new element at the end.  The push_back() method will
  // automatically allocate memory as needed.  See the notes for the
  // ACE_Vector::insert() methods for information about how the
  // ACE_Vector grows when necessary.

  void pop_back (void);
  // Removes the last element

  void swap (ACE_Vector<T>&);
  // Swaps the contents of two vectors

  // = ACE_Vector Insertion Methods
  ACE_Vector<T>::iterator insert (ACE_Vector<T>::iterator pos, const T& x);
  // Inserts x before pos and returns the iterator that points to the
  // location of the element in memory.  The original location of the
  // element in memory may have changed if memory was reallocated
  // during insertion so the returned iterator should be used instead
  // of the one that was passed to the insert() method.

  void insert (ACE_Vector<T>::iterator pos,
               ACE_Vector<T>::iterator first,
               ACE_Vector<T>::iterator last);
  // Inserts the range [first, last) before pos.

  void insert (ACE_Vector<T>::iterator pos, 
               size_t n,
               const T& x);
  // Inserts n copies of x before pos.

  ACE_Vector<T>::iterator erase (ACE_Vector<T>::iterator pos);
  // Erases the element at position pos.

  ACE_Vector<T>::iterator erase (ACE_Vector<T>::iterator first,
                                 ACE_Vector<T>::iterator last);
  // Erases the range [first, last)

  // = Iterators

  ACE_Vector<T>::iterator begin (void);
  // Returns an iterator pointing to the beginning of the vector

  const ACE_Vector<T>::iterator begin (void) const;
  // Returns a constant iterator pointing to the beginning of the vector

  ACE_Vector<T>::iterator end (void);
  // Returns an iterator pointing to the end of the vector. 

  const ACE_Vector<T>::iterator end (void) const;
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
  // ACE_Vector growth will always be proportional to the current
  // capacity.  Unless specified by using ACE_Vector<T>::reserve(),
  // the capacity will automatically grow by:
  //
  //     ACE_DEFAULT_VECTOR_GROWTH_FACTOR * ACE_Vector::capacity()
  //
  // In order to ensure insertion of a series of elements into a
  // vector is a linear time operation growth must be proportionall to 
  // the current capacity.  Otherwise, if growth is not proportional
  // to the current capacity, and instead grows by a constant amount,
  // insertion becomes a quadratic time operation.

  void grow (void);
  // Increase the capacity of the ACE_Vector.  Growth will only occur
  // if necessary, i.e. if this->size () == this->capacity ().  The
  // resulting capacity will be greater than or equal to the previous
  // capacity.

  void grow (size_t amount);
  // Increase the capacity of the ACE_Vector by the specified amount.
  // Growth will only occur if necessary, i.e. if this->size () +
  // amount == this->capacity ().  The resulting capacity will
  // be greater than or equal to the previous capacity.

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
