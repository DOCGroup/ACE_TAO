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

#if !defined (ACE_RB_TREE_H)
#define ACE_RB_TREE_H

enum RB_Tree_Node_Color {RED, BLACK};

#include "ace/ACE.h"

// Class Template: RB_Tree_Node
//
// Purpose:        Implements a node in a Red-Black Tree ADT
//
template <class KEY, class T>
class RB_Tree_Node
{
public:
  RB_Tree_Node (const KEY &k, const T &t);
  // constructor

  ~RB_Tree_Node ();
  // destructor

  KEY & key ();
  // key accessor

  T & item ();
  // item accessor

  void color (RB_Tree_Node_Color c);
  // set color of the node

  RB_Tree_Node_Color color ();
  // get color of the node

  RB_Tree_Node<KEY, T> * parent ();
  // accessor for node's parent pointer

  void parent (RB_Tree_Node<KEY, T> * p);
  // mutator for node's parent pointer

  RB_Tree_Node<KEY, T> * left ();
  // accessor for node's left child pointer

  void left (RB_Tree_Node<KEY, T> * l);
  // mutator for node's left child pointer

  RB_Tree_Node<KEY, T> * right ();
  // accessor for node's rightt child pointer

  void right (RB_Tree_Node<KEY, T> * r);
  // mutator for node's right child pointer

private:

  KEY k_;
  // the key

  T t_;
  // the item

  RB_Tree_Node_Color color_;
  // color of the node

  RB_Tree_Node<KEY, T> *parent_;
  // pointer to node's parent

  RB_Tree_Node<KEY, T> *left_;
  // pointer to node's left child

  RB_Tree_Node<KEY, T> *right_;
  // pointer to node's righ child

};

// Class Template: RB_Tree
//
// Purpose:        Implements a Red-Black Tree ADT, according to
//                 T. H. Corman, C. E. Leiserson, and R. L. Rivest,
//                 "Introduction to Algorithms" 1990, MIT, chapter 14
//
template <class KEY, class T>
class RB_Tree
{
public:

  RB_Tree ();
  // constructor

  RB_Tree (const RB_Tree<KEY, T> &rbt);
  // copy constructor

  virtual ~RB_Tree ();
  // destructor

  void operator = (const RB_Tree<KEY, T> &rbt);
  // assignment operator

  virtual int lessthan (const KEY &k1, const KEY &k2);
  // lessthan comparison function for keys.
  // returns 1 if k1 < k2, 0 otherwise

  T* find (const KEY &k);
  // Returns a pointer to the item corresponding to the
  // given key, or 0 if it cannot find the key in the tree.

  T* insert (const KEY &k, const T &t);
  // Inserts a *copy* of the key and the item into the tree:
  // both the key type KEY and the item type T must have well
  // defined semantics for copy construction and < comparison.
  // This method returns a pointer to the inserted item copy,
  // or 0 if an error occurred.  NOTE: if an identical key
  // already exists in the tree, no new item is created, and
  // the returned pointer addresses the existing item
  // associated with the existing key.

  int remove (const KEY &k);
  // removes the item associated with the given key from the
  // tree and destroys it.  Returns 1 if it found the item
  // and successfully destroyed it, 0 if it did not find the
  // item, or -1 if an error occurred.

  void clear ();
  // destroys all nodes and sets the root pointer null.

// These could all be made private methods by making the corresponding
// class template instantiations friends, but there are some problems
// with this on certain compilers: leave them all public for now

// private:

  void RB_rotate_right (RB_Tree_Node<KEY, T> * x);
  // method for right rotation of the tree about a given node

  void RB_rotate_left (RB_Tree_Node<KEY, T> * x);
  // method for left rotation of the tree about a given node

  void RB_delete_fixup (RB_Tree_Node<KEY, T> * x);
  // method for restoring Red-Black properties after deletion

  RB_Tree_Node<KEY, T> * RB_tree_successor (RB_Tree_Node<KEY, T> *x) const;
  // method to find the successor node of the given node in the tree

  RB_Tree_Node<KEY, T> * RB_tree_predecessor (RB_Tree_Node<KEY, T> *x) const;
  // method to find the predecessor node of the given node in the tree

  RB_Tree_Node<KEY, T> * RB_tree_minimum (RB_Tree_Node<KEY, T> *x) const;
  // method to find the minimum node of the subtree rooted at the given node

  RB_Tree_Node<KEY, T> * RB_tree_maximum (RB_Tree_Node<KEY, T> *x) const;
  // method to find the maximum node of the subtree rooted at the given node

  RB_Tree_Node<KEY, T> * find_node (const KEY &k);
  // returns a pointer to a matching node if there is one,
  // a pointer to the node under which to insert the item
  // if the tree is not empty and there is no such match,
  // or 0 if the tree is empty.

  void RB_rebalance (RB_Tree_Node<KEY, T> * x);
  // rebalance the tree after insertion of a node

  // private members

  RB_Tree_Node<KEY, T> *root_;
  // the root of the tree

};

// Class Template: RB_Tree_Iterator
//
// Purpose:        Implements an iterator for a Red-Black Tree ADT
//
template <class KEY, class T>
class RB_Tree_Iterator
{
public:

  RB_Tree_Iterator (const RB_Tree<KEY, T> &tree);
  // constructor

  ~RB_Tree_Iterator ();
  // destructor

  KEY * key ();
  // accessor for key of node under iterator (if any)

  T * item ();
  // accessor for item of node under iterator (if any)

  int first ();
  // move to the first item in the tree

  int last ();
  // move to the last item in the tree

  int next ();
  // move to the next item in the tree

  int previous ();
  // move to the previous item in the tree

  int is_done ();
  // returns 0 if the iterator is positioned over
  // a valid RB_Tree node, returns 1 if not

private:

  // declare private and do not define: explicitly
  // prevent assignment and copy construction of iterators
  RB_Tree_Iterator (const RB_Tree_Iterator<KEY, T> &);
  operator = (const RB_Tree_Iterator<KEY, T> &);

  // private members

  const RB_Tree<KEY, T> &tree_;
  // reference to the RB_Tree over which we're iterating

  RB_Tree_Node <KEY, T> *node_;
  // pointer to the node currently under the iterator

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
