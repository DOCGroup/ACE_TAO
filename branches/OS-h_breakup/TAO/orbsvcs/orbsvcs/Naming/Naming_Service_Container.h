/* -*- C++ -*- */
// $Id$
// ============================================================================
//
// = LIBRARY
//    cos
//
// = FILENAME
//    Naming_Service_Container.h
//
// = AUTHOR
//    Bruce Trask <trask_b@ociweb.com>
//
// ============================================================================


#ifndef NS_CONTAINER_H
#define NS_CONTAINER_H
#include "ace/pre.h"

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Need by ACE_DLList_Node.
#include "ace/Containers.h"

class ACE_Allocator;


// Forward declarations.
template <class T> class ACE_Unbounded_List;
template <class T> class ACE_Unbounded_List_Iterator;


template<class T>
class ACE_NS_Node
{
  // = TITLE
  //     Implementation element in a Queue, List, and Stack.
public:
  friend class ACE_Unbounded_List<T>;
  friend class ACE_Unbounded_List_Iterator<T>;



# if ! defined (ACE_HAS_BROKEN_NOOP_DTORS)
  ~ACE_NS_Node (void);
  // This isn't necessary, but it keeps some compilers happy.
# endif /* ! defined (ACE_HAS_BROKEN_NOOP_DTORS) */

private:
  // = Initialization methods
  ACE_NS_Node (const T &i, ACE_NS_Node<T> *n);
  ACE_NS_Node (ACE_NS_Node<T> *n = 0, int = 0);
  ACE_NS_Node (const ACE_NS_Node<T> &n);

  ACE_NS_Node<T> *next_;
  // Pointer to next element in the list of <ACE_NS_Node>s.

  T item_;
  // Current value of the item in this node.
};





template <class T>
class ACE_Unbounded_List_Iterator
{
  // = TITLE
  //     Implement an iterator over an unbounded List.
public:
  // = Initialization method.
  ACE_Unbounded_List_Iterator (ACE_Unbounded_List<T> &s, int end = 0);

  // = Iteration methods.

  int next (T *&next_item);
  // Pass back the <next_item> that hasn't been seen in the List.
  // Returns 0 when all items have been seen, else 1.

  int advance (void);
  // Move forward by one element in the List.  Returns 0 when all the
  // items in the List have been seen, else 1.

  int first (void);
  // Move to the first element in the List.  Returns 0 if the
  // List is empty, else 1.

  int done (void) const;
  // Returns 1 when all items have been seen, else 0.

  void dump (void) const;
  // Dump the state of an object.

  // = STL styled iteration, compare, and reference functions.

  ACE_Unbounded_List_Iterator<T> operator++ (int);
  // Postfix advance.

  ACE_Unbounded_List_Iterator<T>& operator++ (void);
  // Prefix advance.

  T& operator* (void);
  // Returns a reference to the interal element <this> is pointing to.

  int operator== (const ACE_Unbounded_List_Iterator<T> &) const;
  int operator!= (const ACE_Unbounded_List_Iterator<T> &) const;
  // Check if two iterators point to the same position

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:

  ACE_NS_Node<T> *current_;
  // Pointer to the current node in the iteration.

  ACE_Unbounded_List<T> *set_;
  // Pointer to the set we're iterating over.
};

template <class T>
class ACE_Unbounded_List
{
  // = TITLE
  //     Implement a simple unordered set of <T> of unbounded size.
  //
  // = DESCRIPTION
  //     This implementation of an unordered set uses a circular
  //     linked list with a dummy node.  This implementation does not
  //     allow duplicates, but it maintains FIFO ordering of insertions.
public:
  friend class ACE_Unbounded_List_Iterator<T>;

  // Trait definition.
  typedef ACE_Unbounded_List_Iterator<T> ITERATOR;
  typedef ACE_Unbounded_List_Iterator<T> iterator;

  // = Initialization and termination methods.
  ACE_Unbounded_List (ACE_Allocator *alloc = 0);
  // Constructor.  Use user specified allocation strategy
  // if specified.

  ACE_Unbounded_List (const ACE_Unbounded_List<T> &);
  // Copy constructor.

  void operator= (const ACE_Unbounded_List<T> &);
  // Assignment operator.

  ~ACE_Unbounded_List (void);
  // Destructor.

  // = Check boundary conditions.

  int is_empty (void) const;
  // Returns 1 if the container is empty, otherwise returns 0.

  int is_full (void) const;
  // Returns 1 if the container is full, otherwise returns 0.

  // = Classic unordered set operations.

  int insert (const T &new_item);
  // Insert <new_item> into the set (doesn't allow duplicates).
  // Returns -1 if failures occur, 1 if item is already present, else
  // 0.

  int remove (const T &item);
  // Remove first occurrence of <item> from the set.  Returns 0 if
  // it removes the item, -1 if it can't find the item, and -1 if a
  // failure occurs.

  size_t size (void) const;
  // Size of the set.

  void dump (void) const;
  // Dump the state of an object.

  void reset (void);
  // Reset the <ACE_Unbounded_List> to be empty.

  // = STL-styled unidirectional iterator factory.
  ACE_Unbounded_List_Iterator<T> begin (void);
  ACE_Unbounded_List_Iterator<T> end (void);

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  int insert_tail (const T &item);
  // Insert <item> at the tail of the set (doesn't check for
  // duplicates).

  void delete_nodes (void);
  // Delete all the nodes in the List.

  void copy_nodes (const ACE_Unbounded_List<T> &);
  // Copy nodes into this set.

  ACE_NS_Node<T> *head_;
  // Head of the linked list of NS_Nodes.

  size_t cur_size_;
  // Current size of the set.

  ACE_Allocator *allocator_;
  // Allocation strategy of the set.
};


#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Naming_Service_Container.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Naming_Service_Container.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* NS_CONTAINERS_T_H */
