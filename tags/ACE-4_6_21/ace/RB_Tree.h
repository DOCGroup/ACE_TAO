/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    RB_Tree.h
//
// = AUTHOR
//     Chris Gill
//
// ============================================================================

#ifndef ACE_RB_TREE_H
#define ACE_RB_TREE_H

#include "ace/ACE.h"
#include "ace/Functor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_RB_Tree_Node_Base
{
public:
  enum RB_Tree_Node_Color {RED, BLACK};
};

template <class KEY, class T>
class ACE_RB_Tree_Node : public ACE_RB_Tree_Node_Base
{
  // = TITLE
  //   Implements a node in a Red-Black Tree ADT.
public:

  // Initialization and termination methods.

  ACE_RB_Tree_Node (const KEY &k, const T &t);
  // Constructor.

  ~ACE_RB_Tree_Node (void);
  // Destructor.

  KEY &key (void);
  // Key accessor.

  T &item (void);
  // Item accessor.

  void color (RB_Tree_Node_Color c);
  // Set color of the node.

  RB_Tree_Node_Color color (void);
  // Get color of the node.

  ACE_RB_Tree_Node<KEY, T> *parent (void);
  // Accessor for node's parent pointer.

  void parent (ACE_RB_Tree_Node<KEY, T> * p);
  // Mutator for node's parent pointer.

  ACE_RB_Tree_Node<KEY, T> *left (void);
  // Accessor for node's left child pointer.

  void left (ACE_RB_Tree_Node<KEY, T> *l);
  // Mutator for node's left child pointer.

  ACE_RB_Tree_Node<KEY, T> *right (void);
  // Accessor for node's right child pointer.

  void right (ACE_RB_Tree_Node<KEY, T> * r);
  // Mutator for node's right child pointer

private:

  KEY k_;
  // The key.

  T t_;
  // The item.

  RB_Tree_Node_Color color_;
  // Color of the node.

  ACE_RB_Tree_Node<KEY, T> *parent_;
  // Pointer to node's parent.

  ACE_RB_Tree_Node<KEY, T> *left_;
  // Pointer to node's left child.

  ACE_RB_Tree_Node<KEY, T> *right_;
  // Pointer to node's right child.
};

template <class KEY, class T, class COMPARE_KEYS, class ACE_LOCK>
class ACE_RB_Tree
{
  // = TITLE
  //   Implements a Red-Black Tree ADT, according to T. H. Corman,
  //   C. E. Leiserson, and R. L. Rivest, "Introduction to Algorithms"
  //   1990, MIT, chapter 14.
  //
  // = Description
  //   If an ACE allocator is passed to the RB_Tree constructor, it is used 
  //   for all dynamic allocations done by the tree.
public:
  // = Initialization and termination methods.
  ACE_RB_Tree ();
  // Constructor.

  ACE_RB_Tree (const ACE_RB_Tree<KEY, T, COMPARE_KEYS, ACE_LOCK> &rbt);
  // Copy constructor.

  virtual ~ACE_RB_Tree (void);
  // Destructor.

  void operator= (const ACE_RB_Tree<KEY, T, COMPARE_KEYS, ACE_LOCK> &rbt);
  // Assignment operator.

  virtual int lessthan (const KEY &k1, const KEY &k2);
  // Less than comparison function for keys, using comparison functor.

  T* find (const KEY &k);
  // Returns a pointer to the item corresponding to the
  // given key, or 0 if it cannot find the key in the tree.

  T* insert (const KEY &k, const T &t);
  // Inserts a *copy* of the key and the item into the tree: both the
  // key type KEY and the item type T must have well defined semantics
  // for copy construction.  The default implementation also requires that
  // the key type support welll defined < semantics.  This method returns a
  // pointer to the inserted item copy, or 0 if an error occurred.
  // NOTE: if an identical key already exists in the tree, no new item
  // is created, and the returned pointer addresses the existing item
  // associated with the existing key.

  int remove (const KEY &k);
  // Removes the item associated with the given key from the tree and
  // destroys it.  Returns 1 if it found the item and successfully
  // destroyed it, 0 if it did not find the item, or -1 if an error
  // occurred.

  void clear (void);
  // Destroys all nodes and sets the root pointer null.

  // These could all be made private methods by making the corresponding
  // class template instantiations friends, but there are some problems
  // with this on certain compilers: leave them all public for now.

// private:

  void RB_rotate_right (ACE_RB_Tree_Node<KEY, T> * x);
  // Method for right rotation of the tree about a given node.

  void RB_rotate_left (ACE_RB_Tree_Node<KEY, T> * x);
  // Method for left rotation of the tree about a given node.

  void RB_delete_fixup (ACE_RB_Tree_Node<KEY, T> * x);
  // Method for restoring Red-Black properties after deletion.

  ACE_RB_Tree_Node<KEY, T> * 
    RB_tree_successor (ACE_RB_Tree_Node<KEY, T> *x) const;
  // Method to find the successor node of the given node in the tree.

  ACE_RB_Tree_Node<KEY, T> * 
    RB_tree_predecessor (ACE_RB_Tree_Node<KEY, T> *x) const;
  // Method to find the predecessor node of the given node in the
  // tree.

  ACE_RB_Tree_Node<KEY, T> * 
    RB_tree_minimum (ACE_RB_Tree_Node<KEY, T> *x) const;
  // Method to find the minimum node of the subtree rooted at the
  // given node.

  ACE_RB_Tree_Node<KEY, T> * 
    RB_tree_maximum (ACE_RB_Tree_Node<KEY, T> *x) const;
  // Method to find the maximum node of the subtree rooted at the
  // given node.

  ACE_RB_Tree_Node<KEY, T> * find_node (const KEY &k);
  // Returns a pointer to a matching node if there is one, a pointer
  // to the node under which to insert the item if the tree is not
  // empty and there is no such match, or 0 if the tree is empty.

  void RB_rebalance (ACE_RB_Tree_Node<KEY, T> * x);
  // Rebalance the tree after insertion of a node.

  // Private members.

  ACE_RB_Tree_Node<KEY, T> *root_;
  // The root of the tree.

  COMPARE_KEYS compare_keys_;
  // Comparison functor for comparing nodes in the tree.

};

template <class KEY, class T, class COMPARE_KEYS, class ACE_LOCK>
class ACE_RB_Tree_Iterator
{
  // = TITLE
  //   Implements an iterator for a Red-Black Tree ADT.
public:
  // = Initialization and termination methods.
  ACE_RB_Tree_Iterator (const ACE_RB_Tree<KEY, T, COMPARE_KEYS, ACE_LOCK> &tree);
  // Constructor.

  ~ACE_RB_Tree_Iterator (void);
  // Destructor.

  KEY *key (void);
  // Accessor for key of node under iterator (if any).

  T *item (void);
  // Accessor for item of node under iterator (if any).

  int first (void);
  // Move to the first item in the tree.

  int last (void);
  // Move to the last item in the tree.

  int next (void);
  // Move to the next item in the tree.

  int previous (void);
  // Move to the previous item in the tree.

  int is_done (void);
  // Returns 0 if the iterator is positioned over a valid ACE_RB_Tree
  // node, returns 1 if not.

private:
  // = Declare private and do not define.

  // Explicitly prevent assignment and copy construction of iterators.
  ACE_UNIMPLEMENTED_FUNC (
    ACE_RB_Tree_Iterator (const ACE_RB_Tree_Iterator<KEY, T, COMPARE_KEYS, ACE_LOCK> &))
  ACE_UNIMPLEMENTED_FUNC (
    void operator = (const ACE_RB_Tree_Iterator<KEY, T, COMPARE_KEYS, ACE_LOCK> &))

  // Private members.

  const ACE_RB_Tree<KEY, T, COMPARE_KEYS, ACE_LOCK> &tree_;
  // Reference to the ACE_RB_Tree over which we're iterating.

  ACE_RB_Tree_Node <KEY, T> *node_;
  // Pointer to the node currently under the iterator.

};

#if defined (__ACE_INLINE__)
#include "ace/RB_Tree.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/RB_Tree.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("RB_Tree.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ! defined (ACE_RB_TREE_H) */
