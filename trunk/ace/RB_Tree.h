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

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class RB_Tree_Node_Base
{
public:
  enum RB_Tree_Node_Color {RED, BLACK};
};

template <class KEY, class T>
class RB_Tree_Node : public RB_Tree_Node_Base
{
  // = TITLE
  //   Implements a node in a Red-Black Tree ADT.
public:
  // Initialization and termination methods.
  RB_Tree_Node (const KEY &k, const T &t);
  // Constructor.

  ~RB_Tree_Node (void);
  // Destructor.

  KEY &key (void);
  // Key accessor.

  T &item (void);
  // Item accessor.

  void color (RB_Tree_Node_Color c);
  // Set color of the node.

  RB_Tree_Node_Color color (void);
  // Get color of the node.

  RB_Tree_Node<KEY, T> *parent (void);
  // Accessor for node's parent pointer.

  void parent (RB_Tree_Node<KEY, T> * p);
  // Mutator for node's parent pointer.

  RB_Tree_Node<KEY, T> *left (void);
  // Accessor for node's left child pointer.

  void left (RB_Tree_Node<KEY, T> *l);
  // Mutator for node's left child pointer.

  RB_Tree_Node<KEY, T> *right (void);
  // Accessor for node's right child pointer.

  void right (RB_Tree_Node<KEY, T> * r);
  // Mutator for node's right child pointer

private:

  KEY k_;
  // The key.

  T t_;
  // The item.

  RB_Tree_Node_Color color_;
  // Color of the node.

  RB_Tree_Node<KEY, T> *parent_;
  // Pointer to node's parent.

  RB_Tree_Node<KEY, T> *left_;
  // Pointer to node's left child.

  RB_Tree_Node<KEY, T> *right_;
  // Pointer to node's righ child.
};

template <class KEY, class T>
class RB_Tree
{
  // = TITLE
  //   Implements a Red-Black Tree ADT, according to T. H. Corman,
  //   C. E. Leiserson, and R. L. Rivest, "Introduction to Algorithms"
  //   1990, MIT, chapter 14
public:
  // = Initialization and termination methods.
  RB_Tree (void);
  // Constructor

  RB_Tree (const RB_Tree<KEY, T> &rbt);
  // copy constructor.

  virtual ~RB_Tree (void);
  // Destructor

  void operator= (const RB_Tree<KEY, T> &rbt);
  // Assignment operator.

  virtual int lessthan (const KEY &k1, const KEY &k2);
  // Lessthan comparison function for keys.  returns 1 if k1 < k2, 0
  // otherwise.

  T* find (const KEY &k);
  // Returns a pointer to the item corresponding to the
  // given key, or 0 if it cannot find the key in the tree.

  T* insert (const KEY &k, const T &t);
  // Inserts a *copy* of the key and the item into the tree: both the
  // key type KEY and the item type T must have well defined semantics
  // for copy construction and < comparison.  This method returns a
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
  // destroys all nodes and sets the root pointer null.

  // These could all be made private methods by making the corresponding
  // class template instantiations friends, but there are some problems
  // with this on certain compilers: leave them all public for now

// private:

  void RB_rotate_right (RB_Tree_Node<KEY, T> * x);
  // Method for right rotation of the tree about a given node.

  void RB_rotate_left (RB_Tree_Node<KEY, T> * x);
  // Method for left rotation of the tree about a given node.

  void RB_delete_fixup (RB_Tree_Node<KEY, T> * x);
  // Method for restoring Red-Black properties after deletion.

  RB_Tree_Node<KEY, T> * RB_tree_successor (RB_Tree_Node<KEY, T> *x) const;
  // Method to find the successor node of the given node in the tree.

  RB_Tree_Node<KEY, T> * RB_tree_predecessor (RB_Tree_Node<KEY, T> *x) const;
  // Method to find the predecessor node of the given node in the
  // tree.

  RB_Tree_Node<KEY, T> * RB_tree_minimum (RB_Tree_Node<KEY, T> *x) const;
  // Method to find the minimum node of the subtree rooted at the
  // given node.

  RB_Tree_Node<KEY, T> * RB_tree_maximum (RB_Tree_Node<KEY, T> *x) const;
  // Method to find the maximum node of the subtree rooted at the
  // given node.

  RB_Tree_Node<KEY, T> * find_node (const KEY &k);
  // Returns a pointer to a matching node if there is one, a pointer
  // to the node under which to insert the item if the tree is not
  // empty and there is no such match, or 0 if the tree is empty.

  void RB_rebalance (RB_Tree_Node<KEY, T> * x);
  // Rebalance the tree after insertion of a node.

  // private members

  RB_Tree_Node<KEY, T> *root_;
  // The root of the tree.
};

template <class KEY, class T>
class RB_Tree_Iterator
{
  // = TITLE
  //   Implements an iterator for a Red-Black Tree ADT.
public:
  // = Initialization and termination methods.
  RB_Tree_Iterator (const RB_Tree<KEY, T> &tree);
  // Constructor.

  ~RB_Tree_Iterator (void);
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
  // Returns 0 if the iterator is positioned over a valid RB_Tree
  // node, returns 1 if not.

private:
  // = Declare private and do not define.
  
  // Explicitly prevent assignment and copy construction of iterators
  ACE_UNIMPLEMENTED_FUNC (RB_Tree_Iterator (const RB_Tree_Iterator<KEY, T> &))
  ACE_UNIMPLEMENTED_FUNC (void operator = (const RB_Tree_Iterator<KEY, T> &))

  // Private members.

  const RB_Tree<KEY, T> &tree_;
  // Reference to the RB_Tree over which we're iterating.

  RB_Tree_Node <KEY, T> *node_;
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
