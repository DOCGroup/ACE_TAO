/* -*- C++ -*- */

//=============================================================================
/**
 *  @file Unbounded_Set_Ex.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 *  ACE_Unbounded_Set Extension by Rudolf Weber <rfweber@tesionmail.de>
 *
 *  If iterators are working in an Unbounded_Set_Ex, the elements are not
 *  deleted physically, but marked as deleted.
 *  There is a bookkeeping of the iterators active in the set.
 *  It is an error if a set is reset() or destructed while iterators are
 *  still working on the set.
 *
 *  CAUTION: Pay attention to the state of the iterators.
 *           Deleting a set, or an element in a set, is only feasible
 *           when no iterator is active.
 *
 */
//=============================================================================

#ifndef ACE_UNBOUNDED_SET_EX_H
#define ACE_UNBOUNDED_SET_EX_H
#include /**/ "ace/pre.h"

#include "ace/Node.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Allocator;

/**
 * @class ACE_Unbounded_Set_Ex_Iterator
 *
 * @brief Implement an iterator over an unbounded set.
 */
template <class T>
class ACE_Unbounded_Set_Ex_Iterator
{
public:
  // = Initialization method.
  ACE_Unbounded_Set_Ex_Iterator (ACE_Unbounded_Set_Ex<T> &s, int end = 0);
  ACE_Unbounded_Set_Ex_Iterator (const  ACE_Unbounded_Set_Ex_Iterator &o);
  void operator= (const ACE_Unbounded_Set_Ex_Iterator &o);
  ~ACE_Unbounded_Set_Ex_Iterator ();

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
  ACE_Unbounded_Set_Ex_Iterator<T> operator++ (int);

  /// Prefix advance.
  ACE_Unbounded_Set_Ex_Iterator<T>& operator++ (void);

  /// Returns a reference to the internal element <this> is pointing to.
  T& operator* (void);

  /// Check if two iterators point to the same position
  int operator== (const ACE_Unbounded_Set_Ex_Iterator<T> &) const;
  int operator!= (const ACE_Unbounded_Set_Ex_Iterator<T> &) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:

  /// Pointer to the current node in the iteration.
  ACE_Node<T> *current_;

  /// Pointer to the set we're iterating over.
  ACE_Unbounded_Set_Ex<T> *set_;

  // Flag that indicates whether this iterator is registered at the set.
  int registered_in_set_;
};

/**
 * @class ACE_Unbounded_Set_Ex_Const_Iterator
 *
 * @brief Implement a const iterator over an unbounded set.
 *
 *  The bookkeeping operations are regarded as const. (The member
 *  number_of_iterators_ in ACE_Unbounded_Set_Ex is `mutable'.)
 *  Some asynchronous activity may cause a deletion under a Const_Iterator
 *  so Const_Iterators are registered too.
 *  However, the cleanup operation isn't const at all, thus it is not
 *  called from const iterators.
 *
 */
template <class T>
class ACE_Unbounded_Set_Ex_Const_Iterator
{
public:
  // = Initialization method.
  ACE_Unbounded_Set_Ex_Const_Iterator (const ACE_Unbounded_Set_Ex<T> &s, int end = 0);
  ACE_Unbounded_Set_Ex_Const_Iterator (const ACE_Unbounded_Set_Ex_Const_Iterator& o);
  void operator= (const ACE_Unbounded_Set_Ex_Const_Iterator& o);
  ~ACE_Unbounded_Set_Ex_Const_Iterator ();

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
  ACE_Unbounded_Set_Ex_Const_Iterator<T> operator++ (int);

  /// Prefix advance.
  ACE_Unbounded_Set_Ex_Const_Iterator<T>& operator++ (void);

  /// Returns a reference to the internal element <this> is pointing to.
  T& operator* (void);

  /// Check if two iterators point to the same position
  int operator== (const ACE_Unbounded_Set_Ex_Const_Iterator<T> &) const;
  int operator!= (const ACE_Unbounded_Set_Ex_Const_Iterator<T> &) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:

  /// Pointer to the current node in the iteration.
  ACE_Node<T> *current_;

  /// Pointer to the set we're iterating over.
  const ACE_Unbounded_Set_Ex<T> *set_;

  // Flag that indicates whether this iterator is registered at the set.
  int registered_in_set_;
};

/**
 * @class ACE_Unbounded_Set_Ex
 *
 * @brief Implement a simple unordered set of <T> of unbounded size.
 *
 * This implementation of an unordered set uses a circular
 * linked list with a dummy node.  This implementation does not
 * allow duplicates, but it maintains FIFO ordering of insertions.
 *
 * <b> Requirements and Performance Characteristics</b>
 *   - Internal Structure
 *       Circular linked list
 *   - Duplicates allowed?
 *       No
 *   - Random access allowed?
 *       No
 *   - Search speed
 *       Linear
 *   - Insert/replace speed
 *       Linear
 *   - Iterator still valid after change to container?
 *       Yes
 *   - Frees memory for removed elements?
 *       Yes
 *   - Items inserted by
 *       Value
 *   - Requirements for contained type
 *       -# Default constructor
 *       -# Copy constructor
 *       -# operator=
 *       -# operator==
 *
 */
template <class T>
class ACE_Unbounded_Set_Ex
{
public:
  friend class ACE_Unbounded_Set_Ex_Iterator<T>;
  friend class ACE_Unbounded_Set_Ex_Const_Iterator<T>;

  // Trait definition.
  typedef ACE_Unbounded_Set_Ex_Iterator<T> ITERATOR;
  typedef ACE_Unbounded_Set_Ex_Iterator<T> iterator;
  typedef ACE_Unbounded_Set_Ex_Const_Iterator<T> CONST_ITERATOR;
  typedef ACE_Unbounded_Set_Ex_Const_Iterator<T> const_iterator;

  // = Initialization and termination methods.
  /// Constructor.  Use user specified allocation strategy
  /// if specified.
  /**
   * Initialize an empty set using the allocation strategy of the user if
   * provided.
   */
  ACE_Unbounded_Set_Ex (ACE_Allocator *alloc = 0);

  /// Copy constructor.
  /**
   * Initialize this set to be an exact copy of the set provided.
   */
  ACE_Unbounded_Set_Ex (const ACE_Unbounded_Set_Ex<T> &);

  /// Assignment operator.
  /**
   * Perform a deep copy of the rhs into the lhs.
   */
  void operator= (const ACE_Unbounded_Set_Ex<T> &);

  /// Destructor.
  /**
   * Destroy the nodes of the set.
   */
  ~ACE_Unbounded_Set_Ex (void);

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

  /// Reset the <ACE_Unbounded_Set_Ex> to be empty.
  /**
   * Delete the nodes of the set.
   */
  void reset (void);

  // = STL-styled unidirectional iterator factory.
  ACE_Unbounded_Set_Ex_Iterator<T> begin (void);
  ACE_Unbounded_Set_Ex_Iterator<T> end (void);

  /// An Iterator has to register itself here.
  void iterator_add () const;
  /// A non-const Iterator has to unregister itself here.
  void iterator_leave ();
  /// A Const_Iterator has to unregister itself here.
  void const_iterator_leave () const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  /// Delete all the nodes in the Set.
  void delete_nodes (void);

  /// Copy nodes into this set.
  void copy_nodes (const ACE_Unbounded_Set_Ex<T> &);

  /// Really delete all nodes marked for deletion.
  void cleanup ();

  /// Head of the linked list of Nodes.
  ACE_Node<T> *head_;

  /// Current size of the set.
  size_t cur_size_;

  /// Allocation strategy of the set.
  ACE_Allocator *allocator_;

  /// Number of iterators working on this set.
  mutable int number_of_iterators_;
};

#if defined (__ACE_INLINE__)
#include "ace/Unbounded_Set_Ex.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Unbounded_Set_Ex.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Unbounded_Set_Ex.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* ACE_UNBOUNDED_SET_EX_H */
