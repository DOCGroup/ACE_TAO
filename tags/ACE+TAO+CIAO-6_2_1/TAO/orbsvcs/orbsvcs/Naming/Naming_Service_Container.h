// -*- C++ -*-

//=============================================================================
/**
 *  @file    Naming_Service_Container.h
 *
 *  $Id$
 *
 *  @author Bruce Trask <trask_b@ociweb.com>
 */
//=============================================================================

#ifndef NS_CONTAINER_H
#define NS_CONTAINER_H
#include /**/ "ace/pre.h"

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"

// Need by ACE_DLList_Node.
#include "ace/Containers.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Allocator;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class T> class ACE_Unbounded_List;
template <class T> class ACE_Unbounded_List_Iterator;

/**
 * @class ACE_NS_Node
 *
 * @brief Implementation element in a Queue, List, and Stack.
 */
template<class T>
class ACE_NS_Node
{
public:
  friend class ACE_Unbounded_List<T>;
  friend class ACE_Unbounded_List_Iterator<T>;

  ~ACE_NS_Node (void);

private:
  // = Initialization methods
  ACE_NS_Node (const T &i, ACE_NS_Node<T> *n);
  ACE_NS_Node (ACE_NS_Node<T> *n = 0, int = 0);
  ACE_NS_Node (const ACE_NS_Node<T> &n);

  /// Pointer to next element in the list of <ACE_NS_Node>s.
  ACE_NS_Node<T> *next_;

  /// Current value of the item in this node.
  T item_;
};

/**
 * @class ACE_Unbounded_List_Iterator
 *
 * @brief Implement an iterator over an unbounded List.
 */
template <class T>
class ACE_Unbounded_List_Iterator
{
public:
  // = Initialization method.
  ACE_Unbounded_List_Iterator (ACE_Unbounded_List<T> &s, int end = 0);

  // = Iteration methods.

  /// Pass back the <next_item> that hasn't been seen in the List.
  /// Returns 0 when all items have been seen, else 1.
  int next (T *&next_item);

  /// Move forward by one element in the List.  Returns 0 when all the
  /// items in the List have been seen, else 1.
  int advance (void);

  /// Move to the first element in the List.  Returns 0 if the
  /// List is empty, else 1.
  int first (void);

  /// Returns 1 when all items have been seen, else 0.
  int done (void) const;

  /// Dump the state of an object.
  void dump (void) const;

  // = STL styled iteration, compare, and reference functions.

  /// Postfix advance.
  ACE_Unbounded_List_Iterator<T> operator++ (int);

  /// Prefix advance.
  ACE_Unbounded_List_Iterator<T>& operator++ (void);

  /// Returns a reference to the interal element <this> is pointing to.
  T& operator* (void);

  /// Check if two iterators point to the same position
  bool operator== (const ACE_Unbounded_List_Iterator<T> &) const;
  bool operator!= (const ACE_Unbounded_List_Iterator<T> &) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:

  /// Pointer to the current node in the iteration.
  ACE_NS_Node<T> *current_;

  /// Pointer to the set we're iterating over.
  ACE_Unbounded_List<T> *set_;
};

/**
 * @class ACE_Unbounded_List
 *
 * @brief Implement a simple unordered set of <T> of unbounded size.
 *
 * This implementation of an unordered set uses a circular
 * linked list with a dummy node.  This implementation does not
 * allow duplicates, but it maintains FIFO ordering of insertions.
 */
template <class T>
class ACE_Unbounded_List
{
public:
  friend class ACE_Unbounded_List_Iterator<T>;

  // Trait definition.
  typedef ACE_Unbounded_List_Iterator<T> ITERATOR;
  typedef ACE_Unbounded_List_Iterator<T> iterator;

  // = Initialization and termination methods.
  /// Constructor.  Use user specified allocation strategy
  /// if specified.
  ACE_Unbounded_List (ACE_Allocator *alloc = 0);

  /// Copy constructor.
  ACE_Unbounded_List (const ACE_Unbounded_List<T> &);

  /// Assignment operator.
  void operator= (const ACE_Unbounded_List<T> &);

  /// Destructor.
  ~ACE_Unbounded_List (void);

  // = Check boundary conditions.

  /// Returns 1 if the container is empty, otherwise returns 0.
  int is_empty (void) const;

  /// Returns 1 if the container is full, otherwise returns 0.
  int is_full (void) const;

  // = Classic unordered set operations.

  /**
   * Insert <new_item> into the set (doesn't allow duplicates).
   * Returns -1 if failures occur, 1 if item is already present, else
   * 0.
   */
  int insert (const T &new_item);

  /**
   * Remove first occurrence of <item> from the set.  Returns 0 if
   * it removes the item, -1 if it can't find the item, and -1 if a
   * failure occurs.
   */
  int remove (const T &item);

  /// Size of the set.
  size_t size (void) const;

  /// Dump the state of an object.
  void dump (void) const;

  /// Reset the <ACE_Unbounded_List> to be empty.
  void reset (void);

  // = STL-styled unidirectional iterator factory.
  ACE_Unbounded_List_Iterator<T> begin (void);
  ACE_Unbounded_List_Iterator<T> end (void);

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  /// Insert <item> at the tail of the set (doesn't check for
  /// duplicates).
  int insert_tail (const T &item);

  /// Delete all the nodes in the List.
  void delete_nodes (void);

  /// Copy nodes into this set.
  void copy_nodes (const ACE_Unbounded_List<T> &);

  /// Head of the linked list of NS_Nodes.
  ACE_NS_Node<T> *head_;

  /// Current size of the set.
  size_t cur_size_;

  /// Allocation strategy of the set.
  ACE_Allocator *allocator_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "orbsvcs/Naming/Naming_Service_Container.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Naming_Service_Container.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* NS_CONTAINERS_T_H */
