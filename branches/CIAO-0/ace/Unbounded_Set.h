/* -*- C++ -*- */

//=============================================================================
/**
 *  @file Unbounded_Set.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_UNBOUNDED_SET_H
#define ACE_UNBOUNDED_SET_H
#include "ace/pre.h"

#include "ace/Node.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Allocator;

/**
 * @class ACE_Unbounded_Set_Iterator
 *
 * @brief Implement an iterator over an unbounded set.
 */
template <class T>
class ACE_Unbounded_Set_Iterator
{
public:
  // = Initialization method.
  ACE_Unbounded_Set_Iterator (ACE_Unbounded_Set<T> &s, int end = 0);

  // = Iteration methods.

  /// Pass back the <next_item> that hasn't been seen in the Set.
  /// Returns 0 when all items have been seen, else 1.
  int next (T *&next_item);

  /// Move forward by one element in the set.  Returns 0 when all the
  /// items in the set have been seen, else 1.
  int advance (void);

  /// Move to the first element in the set.  Returns 0 if the
  /// set is empty, else 1.
  int first (void);

  /// Returns 1 when all items have been seen, else 0.
  int done (void) const;

  /// Dump the state of an object.
  void dump (void) const;

  // = STL styled iteration, compare, and reference functions.

  /// Postfix advance.
  ACE_Unbounded_Set_Iterator<T> operator++ (int);

  /// Prefix advance.
  ACE_Unbounded_Set_Iterator<T>& operator++ (void);

  /// Returns a reference to the internal element <this> is pointing to.
  T& operator* (void);

  /// Check if two iterators point to the same position
  int operator== (const ACE_Unbounded_Set_Iterator<T> &) const;
  int operator!= (const ACE_Unbounded_Set_Iterator<T> &) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:

  /// Pointer to the current node in the iteration.
  ACE_Node<T> *current_;

  /// Pointer to the set we're iterating over.
  ACE_Unbounded_Set<T> *set_;
};

/**
 * @class ACE_Unbounded_Set_Const_Iterator
 *
 * @brief Implement an const iterator over an unbounded set.
 */
template <class T>
class ACE_Unbounded_Set_Const_Iterator
{
public:
  // = Initialization method.
  ACE_Unbounded_Set_Const_Iterator (const ACE_Unbounded_Set<T> &s, int end = 0);

  // = Iteration methods.

  /// Pass back the <next_item> that hasn't been seen in the Set.
  /// Returns 0 when all items have been seen, else 1.
  int next (T *&next_item);

  /// Move forward by one element in the set.  Returns 0 when all the
  /// items in the set have been seen, else 1.
  int advance (void);

  /// Move to the first element in the set.  Returns 0 if the
  /// set is empty, else 1.
  int first (void);

  /// Returns 1 when all items have been seen, else 0.
  int done (void) const;

  /// Dump the state of an object.
  void dump (void) const;

  // = STL styled iteration, compare, and reference functions.

  /// Postfix advance.
  ACE_Unbounded_Set_Const_Iterator<T> operator++ (int);

  /// Prefix advance.
  ACE_Unbounded_Set_Const_Iterator<T>& operator++ (void);

  /// Returns a reference to the internal element <this> is pointing to.
  T& operator* (void);

  /// Check if two iterators point to the same position
  int operator== (const ACE_Unbounded_Set_Const_Iterator<T> &) const;
  int operator!= (const ACE_Unbounded_Set_Const_Iterator<T> &) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:

  /// Pointer to the current node in the iteration.
  ACE_Node<T> *current_;

  /// Pointer to the set we're iterating over.
  const ACE_Unbounded_Set<T> *set_;
};

/**
 * @class ACE_Unbounded_Set
 *
 * @brief Implement a simple unordered set of <T> of unbounded size.
 *
 * This implementation of an unordered set uses a circular
 * linked list with a dummy node.  This implementation does not
 * allow duplicates, but it maintains FIFO ordering of insertions.
 */
template <class T>
class ACE_Unbounded_Set
{
public:
  friend class ACE_Unbounded_Set_Iterator<T>;
  friend class ACE_Unbounded_Set_Const_Iterator<T>;

  // Trait definition.
  typedef ACE_Unbounded_Set_Iterator<T> ITERATOR;
  typedef ACE_Unbounded_Set_Iterator<T> iterator;
  typedef ACE_Unbounded_Set_Const_Iterator<T> CONST_ITERATOR;
  typedef ACE_Unbounded_Set_Const_Iterator<T> const_iterator;

  // = Initialization and termination methods.
  /// Constructor.  Use user specified allocation strategy
  /// if specified.
  /**
   * Initialize an empty set using the allocation strategy of the user if 
   * provided. 
   */
  ACE_Unbounded_Set (ACE_Allocator *alloc = 0);

  /// Copy constructor.
  /**
   * Initialize this set to be an exact copy of the set provided. 
   */
  ACE_Unbounded_Set (const ACE_Unbounded_Set<T> &);

  /// Assignment operator.
  /**
   * Perform a deep copy of the rhs into the lhs. 
   */
  void operator= (const ACE_Unbounded_Set<T> &);

  /// Destructor.
  /**
   * Destroy the nodes of the set. 
   */
  ~ACE_Unbounded_Set (void);

  // = Check boundary conditions.

  /// Returns 1 if the container is empty, otherwise returns 0.
  /**
   * Constant time is_empty check. 
   */
  int is_empty (void) const;

  /// Returns 0.
  /** 
   * Always returns 0 since the set can never fill up. 
   */
  int is_full (void) const;

  // = Classic unordered set operations.

  ///Linear insertion of an item. 
  /**
   * Insert <new_item> into the set (doesn't allow duplicates).
   * Returns -1 if failures occur, 1 if item is already present, else
   * 0.
   */
  int insert (const T &new_item);

  /// Insert <item> at the tail of the set (doesn't check for
  /// duplicates).
  /**
   * Constant time insert at the end of the set. 
   */
  int insert_tail (const T &item);

  ///Linear remove operation. 
  /**
   * Remove first occurrence of <item> from the set.  Returns 0 if
   * it removes the item, -1 if it can't find the item, and -1 if a
   * failure occurs.
   */
  int remove (const T &item);

  /// Finds if <item> occurs in the set.  Returns 0 if find succeeds,
  /// else -1.
  /**
   * Performs a linear find operation. 
   */
  int find (const T &item) const;

  /// Size of the set.
  /**
   * Access the size of the set. 
   */
  size_t size (void) const;

  /// Dump the state of an object.
  void dump (void) const;

  /// Reset the <ACE_Unbounded_Set> to be empty.
  /**
   * Delete the nodes of the set. 
   */
  void reset (void);

  // = STL-styled unidirectional iterator factory.
  ACE_Unbounded_Set_Iterator<T> begin (void);
  ACE_Unbounded_Set_Iterator<T> end (void);

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  /// Delete all the nodes in the Set.
  void delete_nodes (void);

  /// Copy nodes into this set.
  void copy_nodes (const ACE_Unbounded_Set<T> &);

  /// Head of the linked list of Nodes.
  ACE_Node<T> *head_;

  /// Current size of the set.
  size_t cur_size_;

  /// Allocation strategy of the set.
  ACE_Allocator *allocator_;
};

#if defined (__ACE_INLINE__)
#include "ace/Unbounded_Set.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Unbounded_Set.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Unbounded_Set.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* ACE_UNBOUNDED_SET_H */
