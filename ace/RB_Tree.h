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

// Forward decl.
template <class EXT_ID, class INT_ID, class COMPARE_KEYS, class ACE_LOCK>
class ACE_RB_Tree_Iterator_Base;

// Forward decl.
template <class EXT_ID, class INT_ID, class COMPARE_KEYS, class ACE_LOCK>
class ACE_RB_Tree_Iterator;

// Forward decl.
template <class EXT_ID, class INT_ID, class COMPARE_KEYS, class ACE_LOCK>
class ACE_RB_Tree_Reverse_Iterator;

// Forward decl.
class ACE_Allocator;

class ACE_RB_Tree_Node_Base
{
public:
  enum RB_Tree_Node_Color {RED, BLACK};
};

template <class EXT_ID, class INT_ID>
class ACE_RB_Tree_Node : public ACE_RB_Tree_Node_Base
{
  // = TITLE
  //   Implements a node in a Red-Black Tree ADT.  
  //
public:

  // Initialization and termination methods.

  ACE_RB_Tree_Node (const EXT_ID &k, const INT_ID &t);
  // Constructor.

  ~ACE_RB_Tree_Node (void);
  // Destructor.

  EXT_ID &key (void);
  // Key accessor.

  INT_ID &item (void);
  // Item accessor.

  void color (RB_Tree_Node_Color c);
  // Set color of the node.

  RB_Tree_Node_Color color (void);
  // Get color of the node.

  ACE_RB_Tree_Node<EXT_ID, INT_ID> *parent (void);
  // Accessor for node's parent pointer.

  void parent (ACE_RB_Tree_Node<EXT_ID, INT_ID> * p);
  // Mutator for node's parent pointer.

  ACE_RB_Tree_Node<EXT_ID, INT_ID> *left (void);
  // Accessor for node's left child pointer.

  void left (ACE_RB_Tree_Node<EXT_ID, INT_ID> *l);
  // Mutator for node's left child pointer.

  ACE_RB_Tree_Node<EXT_ID, INT_ID> *right (void);
  // Accessor for node's right child pointer.

  void right (ACE_RB_Tree_Node<EXT_ID, INT_ID> * r);
  // Mutator for node's right child pointer

private:

  EXT_ID k_;
  // The key.

  INT_ID t_;
  // The item.

  RB_Tree_Node_Color color_;
  // Color of the node.

  ACE_RB_Tree_Node<EXT_ID, INT_ID> *parent_;
  // Pointer to node's parent.

  ACE_RB_Tree_Node<EXT_ID, INT_ID> *left_;
  // Pointer to node's left child.

  ACE_RB_Tree_Node<EXT_ID, INT_ID> *right_;
  // Pointer to node's right child.
};

template <class EXT_ID, class INT_ID, class COMPARE_KEYS, class ACE_LOCK>
class ACE_RB_Tree
{
  // = TITLE
  //   Implements a Red-Black Tree ADT, according to T. H. Corman,
  //   C. E. Leiserson, and R. L. Rivest, "Introduction to Algorithms"
  //   1990, MIT, chapter 14.
  //
  // = Description
  //   A number of Changes have been made to this class template 
  //   in order to conform to the ACE_Hash_Map_Manager_Ex 
  //   interface.  All previously supported public methods are 
  //   still part of this class. However, these are marked as
  //   DEPRECATED and will be removed from this class in
  //   a future version of ACE.  Please migrate your code
  //   to the appropriate public methods indicated in the
  //   method deprecation comments.

public:

  typedef EXT_ID KEY;
  typedef INT_ID VALUE;
  typedef ACE_RB_Tree_Node<EXT_ID, INT_ID> ENTRY;

  // = ACE-style iterator typedefs.
  typedef ACE_RB_Tree_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> ITERATOR;
  typedef ACE_RB_Tree_Reverse_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> REVERSE_ITERATOR;

  // = STL-style iterator typedefs.
  typedef ACE_RB_Tree_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> iterator;
  typedef ACE_RB_Tree_Reverse_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> reverse_iterator;

  // = Initialization and termination methods.
  ACE_RB_Tree ();
  // Constructor.

  ACE_RB_Tree (const ACE_RB_Tree<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> &rbt);
  // Copy constructor.

  virtual ~ACE_RB_Tree (void);
  // Destructor.

  size_t current_size (void);
  // Returns the current number of nodes in the tree.

  void operator= (const ACE_RB_Tree<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> &rbt);
  // Assignment operator.

  virtual int lessthan (const EXT_ID &k1, const EXT_ID &k2);
  // Less than comparison function for keys, using comparison functor.

  // = DEPRECATED methods.  Please migrate your code to use the new methods instead

  INT_ID* find (const EXT_ID &k);
  // Returns a pointer to the item corresponding to the
  // given key, or 0 if it cannot find the key in the tree.
  // DEPRECATED

  INT_ID* insert (const EXT_ID &k, const INT_ID &t);
  // Inserts a *copy* of the key and the item into the tree: both the
  // key type EXT_ID and the item type INT_ID must have well defined semantics
  // for copy construction.  The default implementation also requires that
  // the key type support well defined < semantics.  This method returns a
  // pointer to the inserted item copy, or 0 if an error occurred.
  // NOTE: if an identical key already exists in the tree, no new item
  // is created, and the returned pointer addresses the existing item
  // associated with the existing key.
  // DEPRECATED

  int remove (const EXT_ID &k);
  // Removes the item associated with the given key from the tree and
  // destroys it.  Returns 1 if it found the item and successfully
  // destroyed it, 0 if it did not find the item, or -1 if an error
  // occurred.
  // DEPRECATED

  void clear (void);
  // Destroys all nodes and sets the root pointer null.
  // DEPRECATED

  // These could all be made private methods by making the corresponding
  // class template instantiations friends, but there are some problems
  // with this on certain compilers: leave them all public for now.

// private:

  void RB_rotate_right (ACE_RB_Tree_Node<EXT_ID, INT_ID> * x);
  // Method for right rotation of the tree about a given node.

  void RB_rotate_left (ACE_RB_Tree_Node<EXT_ID, INT_ID> * x);
  // Method for left rotation of the tree about a given node.

  void RB_delete_fixup (ACE_RB_Tree_Node<EXT_ID, INT_ID> * x);
  // Method for restoring Red-Black properties after deletion.

  ACE_RB_Tree_Node<EXT_ID, INT_ID> * 
    RB_tree_successor (ACE_RB_Tree_Node<EXT_ID, INT_ID> *x) const;
  // Method to find the successor node of the given node in the tree.

  ACE_RB_Tree_Node<EXT_ID, INT_ID> * 
    RB_tree_predecessor (ACE_RB_Tree_Node<EXT_ID, INT_ID> *x) const;
  // Method to find the predecessor node of the given node in the
  // tree.

  ACE_RB_Tree_Node<EXT_ID, INT_ID> * 
    RB_tree_minimum (ACE_RB_Tree_Node<EXT_ID, INT_ID> *x) const;
  // Method to find the minimum node of the subtree rooted at the
  // given node.

  ACE_RB_Tree_Node<EXT_ID, INT_ID> * 
    RB_tree_maximum (ACE_RB_Tree_Node<EXT_ID, INT_ID> *x) const;
  // Method to find the maximum node of the subtree rooted at the
  // given node.

  ACE_RB_Tree_Node<EXT_ID, INT_ID> * find_node (const EXT_ID &k);
  // Returns a pointer to a matching node if there is one, a pointer
  // to the node under which to insert the item if the tree is not
  // empty and there is no such match, or 0 if the tree is empty.

  void RB_rebalance (ACE_RB_Tree_Node<EXT_ID, INT_ID> * x);
  // Rebalance the tree after insertion of a node.

  // Private members.

  ACE_RB_Tree_Node<EXT_ID, INT_ID> *root_;
  // The root of the tree.

  COMPARE_KEYS compare_keys_;
  // Comparison functor for comparing nodes in the tree.

  size_t current_size_;
  // The current number of nodes in the tree.
};

template <class EXT_ID, class INT_ID, class COMPARE_KEYS, class ACE_LOCK>
class ACE_RB_Tree_Iterator_Base
{
  // = TITLE
  //   Implements a common base class for iterators for a Red-Black Tree ADT.
public:

  // = Initialization and termination methods.

  ACE_RB_Tree_Iterator_Base (const ACE_RB_Tree<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> &tree,
                             int set_first);
  // Constructor.  Takes an ACE_RB_Tree over which to iterate, and
  // an integer indicating (if non-zero) to position the iterator
  // at the first element in the tree (if this integer is 0, the
  // iterator is positioned at the last element in the tree).

  ~ACE_RB_Tree_Iterator_Base (void);
  // Destructor.

  // = Iteration methods.

  int next (ACE_RB_Tree_Node<EXT_ID, INT_ID> *&next_entry) const;
  // Passes back the <entry> under the iterator.  Returns 0 if
  // the iteration has completed, otherwise 1.

  int done (void) const;
  // Returns 1 when the iteration has completed, otherwise 0.

  ACE_RB_Tree_Node<EXT_ID, INT_ID> & operator* (void) const;
  // STL-like iterator dereference operator: returns a reference
  // to the node underneath the iterator.

  ACE_RB_Tree<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> &tree (void);
  // Returns a reference to the tree over which we're iterating.

  int operator== (const ACE_RB_Tree_Iterator_Base<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> &) const;
  // Comparison operator: returns 1 if both iterators point to the same position, otherwise 0.

  int operator!= (const ACE_RB_Tree_Iterator_Base<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> &) const;
  // Comparison operator: returns 1 if the iterators point to different positions, otherwise 0.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:

  // = protected methods

  int forward_i (void);
  // Move forward by one element in the tree.  Returns 0 when
  // there are no more elements in the tree, otherwise 1.

  int reverse_i (void);
  // Move forward by one element in the tree.  Returns 0 when
  // there are no more elements in the tree, otherwise 1.

  void dump_i (void) const;
  // Dump the state of an object.

  // = Protected members.

  const ACE_RB_Tree<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> &tree_;
  // Reference to the ACE_RB_Tree over which we're iterating.

  ACE_RB_Tree_Node <EXT_ID, INT_ID> *node_;
  // Pointer to the node currently under the iterator.

private:

  // = Declare private and do not define.

  // Explicitly prevent assignment and copy construction of iterators.
  ACE_UNIMPLEMENTED_FUNC (
    ACE_RB_Tree_Iterator_Base (const ACE_RB_Tree_Iterator_Base<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> &))
  ACE_UNIMPLEMENTED_FUNC (
    void operator = (const ACE_RB_Tree_Iterator_Base<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> &))

};



template <class EXT_ID, class INT_ID, class COMPARE_KEYS, class ACE_LOCK>
class ACE_RB_Tree_Iterator :
  public ACE_RB_Tree_Iterator_Base<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK>
{
  // = TITLE
  //   Implements an iterator for a Red-Black Tree ADT.
public:
  // = Initialization and termination methods.
  ACE_RB_Tree_Iterator (const ACE_RB_Tree<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> &tree,
                        int set_first = 1);
  // Constructor.  Takes an ACE_RB_Tree over which to iterate, and
  // an integer indicating (if non-zero) to position the iterator
  // at the first element in the tree (if this integer is 0, the
  // iterator is positioned at the last element in the tree).

  ~ACE_RB_Tree_Iterator (void);
  // Destructor.

  // = ACE-style iteration methods.

  int advance (void);
  // Move forward by one element in the tree.  Returns
  // 0 when all elements have been seen, else 1.

  void dump (void) const;
  // Dump the state of an object.

  // = STL-style iteration methods.

  ACE_RB_Tree_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> &operator++ (void);
  // Prefix advance.

  ACE_RB_Tree_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> &operator++ (int);
  // Postfix advance.

  ACE_RB_Tree_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> &operator-- (void);
  // Prefix reverse.

  ACE_RB_Tree_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> &operator-- (int);
  // Postfix reverse.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

  // = DEPRECATED methods.  Please migrate your code to use the new methods instead

  EXT_ID *key (void);
  // Accessor for key of node under iterator (if any).
  // DEPRECATED

  INT_ID *item (void);
  // Accessor for item of node under iterator (if any).
  // DEPRECATED

  int first (void);
  // Move to the first item in the iteration (and in the tree).
  // DEPRECATED

  int last (void);
  // Move to the last item in the iteration (and in the tree).
  // DEPRECATED

  int next (void);
  // Move to the next item in the iteration (and in the tree).
  // DEPRECATED

  int previous (void);
  // Move to the previous item in the iteration (and in the tree).
  // DEPRECATED

  int is_done (void);
  // Returns 0 if the iterator is positioned over a valid ACE_RB_Tree
  // node, returns 1 if not.
  // DEPRECATED: use the base class done () method instead.

private:
  // = Declare private and do not define.

  // Explicitly prevent assignment and copy construction of iterators.
  ACE_UNIMPLEMENTED_FUNC (
    ACE_RB_Tree_Iterator (const ACE_RB_Tree_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> &))
  ACE_UNIMPLEMENTED_FUNC (
    void operator = (const ACE_RB_Tree_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> &))

};

template <class EXT_ID, class INT_ID, class COMPARE_KEYS, class ACE_LOCK>
class ACE_RB_Tree_Reverse_Iterator : 
  public ACE_RB_Tree_Iterator_Base<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK>
{
  // = TITLE
  //   Implements a reverse iterator for a Red-Black Tree ADT.
public:
  // = Initialization and termination methods.
  ACE_RB_Tree_Reverse_Iterator (const ACE_RB_Tree<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> &tree,
                                int set_last = 1);
  // Constructor.  Takes an ACE_RB_Tree over which to iterate, and
  // an integer indicating (if non-zero) to position the iterator
  // at the last element in the tree (if this integer is 0, the
  // iterator is positioned at the first element in the tree).

  ~ACE_RB_Tree_Reverse_Iterator (void);
  // Destructor.

  // = ACE-style iteration methods.

  int advance (void);
  // Move forward by one element in the tree.  Returns
  // 0 when all elements have been seen, else 1.

  void dump (void) const;
  // Dump the state of an object.

  // = STL-style iteration methods.

  ACE_RB_Tree_Reverse_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> &operator++ (void);
  // Prefix advance.

  ACE_RB_Tree_Reverse_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> &operator++ (int);
  // Postfix advance.

  ACE_RB_Tree_Reverse_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> &operator-- (void);
  // Prefix reverse.

  ACE_RB_Tree_Reverse_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> &operator-- (int);
  // Postfix reverse.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  // = Declare private and do not define.

  // Explicitly prevent assignment and copy construction of iterators.
  ACE_UNIMPLEMENTED_FUNC (
    ACE_RB_Tree_Reverse_Iterator (const ACE_RB_Tree_Reverse_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> &))
  ACE_UNIMPLEMENTED_FUNC (
    void operator = (const ACE_RB_Tree_Reverse_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> &))
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
