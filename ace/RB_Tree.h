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

#include "ace/OS.h"
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
  //     Implements a Red-Black Tree ADT, according to T. H. Corman,
  //     C. E. Leiserson, and R. L. Rivest, "Introduction to Algorithms"
  //     1990, MIT, chapter 14.
  //
  // = Description
  //     A number of Changes have been made to this class template 
  //     in order to conform to the ACE_Hash_Map_Manager_Ex 
  //     interface.  All previously supported public methods are 
  //     still part of this class. However, these are marked as
  //     DEPRECATED and will be removed from this class in
  //     a future version of ACE.  Please migrate your code
  //     to the appropriate public methods indicated in the
  //     method deprecation comments.
  //
  //     This class uses an <ACE_Allocator> to allocate memory.  The
  //     user can make this a persistent class by providing an
  //     <ACE_Allocator> with a persistable memory pool.

public:
  friend class ACE_RB_Tree_Iterator_Base<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK>;
  friend class ACE_RB_Tree_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK>;
  friend class ACE_RB_Tree_Reverse_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK>;

  typedef EXT_ID KEY;
  typedef INT_ID VALUE;
  typedef ACE_RB_Tree_Node<EXT_ID, INT_ID> ENTRY;

  // = ACE-style iterator typedefs.
  typedef ACE_RB_Tree_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> ITERATOR;
  typedef ACE_RB_Tree_Reverse_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> REVERSE_ITERATOR;

  // = STL-style iterator typedefs.
  typedef ACE_RB_Tree_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> iterator;
  typedef ACE_RB_Tree_Reverse_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> reverse_iterator;

  // = Search result enumeration.
  enum RB_SearchResult {LEFT, EXACT, RIGHT};

  // = Initialization and termination methods.

  ACE_RB_Tree (ACE_Allocator *alloc = 0);
  // Constructor.

  ACE_RB_Tree (const ACE_RB_Tree<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> &rbt);
  // Copy constructor.

  int open (ACE_Allocator *alloc = 0);
  // Initialize an RB Tree.

  int close (void);
  // Close down an RB_Tree and release dynamically allocated
  // resources.

  virtual ~ACE_RB_Tree (void);
  // Destructor.

  // = insertion, removal, and search methods.

  int bind (const EXT_ID &item,
            const INT_ID &int_id);
  // Associate <ext_id> with <int_id>.  If <ext_id> is already in the
  // tree then the <ACE_RB_Tree_Node> is not changed.  Returns 0 if a
  // new entry is bound successfully, returns 1 if an attempt is made
  // to bind an existing entry, and returns -1 if failures occur.

  int bind (const EXT_ID &ext_id,
            const INT_ID &int_id,
            ACE_RB_Tree_Node<EXT_ID, INT_ID> *&entry);
  // Same as a normal bind, except the tree entry is also passed back
  // to the caller.  The entry in this case will either be the newly
  // created entry, or the existing one.


  int trybind (const EXT_ID &ext_id,
               INT_ID &int_id);
  // Associate <ext_id> with <int_id> if and only if <ext_id> is not
  // in the tree.  If <ext_id> is already in the tree then the <int_id>
  // parameter is assigned the existing value in the tree.  Returns 0
  // if a new entry is bound successfully, returns 1 if an attempt is
  // made to bind an existing entry, and returns -1 if failures occur.

  int trybind (const EXT_ID &ext_id,
               INT_ID &int_id,
               ACE_RB_Tree_Node<EXT_ID, INT_ID> *&entry);
  // Same as a normal trybind, except the tree entry is also passed
  // back to the caller.  The entry in this case will either be the
  // newly created entry, or the existing one.

  int rebind (const EXT_ID &ext_id,
              const INT_ID &int_id);
  // Reassociate <ext_id> with <int_id>.  If <ext_id> is not in the
  // tree then behaves just like <bind>.  Returns 0 if a new entry is
  // bound successfully, returns 1 if an existing entry was rebound,
  // and returns -1 if failures occur.

  int rebind (const EXT_ID &ext_id,
              const INT_ID &int_id,
              ACE_RB_Tree_Node<EXT_ID, INT_ID> *&entry);
  // Same as a normal rebind, except the tree entry is also passed back
  // to the caller.  The entry in this case will either be the newly
  // created entry, or the existing one.

  int rebind (const EXT_ID &ext_id,
              const INT_ID &int_id,
              INT_ID &old_int_id);
  // Associate <ext_id> with <int_id>.  If <ext_id> is not in the tree
  // then behaves just like <bind>.  Otherwise, store the old value of
  // <int_id> into the "out" parameter and rebind the new parameters.
  // Returns 0 if a new entry is bound successfully, returns 1 if an
  // existing entry was rebound, and returns -1 if failures occur.

  int rebind (const EXT_ID &ext_id,
              const INT_ID &int_id,
              INT_ID &old_int_id,
              ACE_RB_Tree_Node<EXT_ID, INT_ID> *&entry);
  // Same as a normal rebind, except the tree entry is also passed back
  // to the caller.  The entry in this case will either be the newly
  // created entry, or the existing one.

  int rebind (const EXT_ID &ext_id,
              const INT_ID &int_id,
              EXT_ID &old_ext_id,
              INT_ID &old_int_id);
  // Associate <ext_id> with <int_id>.  If <ext_id> is not in the tree
  // then behaves just like <bind>.  Otherwise, store the old values
  // of <ext_id> and <int_id> into the "out" parameters and rebind the
  // new parameters.  This is very useful if you need to have an
  // atomic way of updating <ACE_RB_Tree_Nodes> and you also need
  // full control over memory allocation.  Returns 0 if a new entry is
  // bound successfully, returns 1 if an existing entry was rebound,
  // and returns -1 if failures occur.

  int rebind (const EXT_ID &ext_id,
              const INT_ID &int_id,
              EXT_ID &old_ext_id,
              INT_ID &old_int_id,
              ACE_RB_Tree_Node<EXT_ID, INT_ID> *&entry);
  // Same as a normal rebind, except the tree entry is also passed back
  // to the caller.  The entry in this case will either be the newly
  // created entry, or the existing one.

  int find (const EXT_ID &ext_id,
            INT_ID &int_id);
  // Locate <ext_id> and pass out parameter via <int_id>.  If found,
  // return 0, returns -1 if not found.

  int find (const EXT_ID &ext_id,
            ACE_RB_Tree_Node<EXT_ID, INT_ID> *&entry);
  // Locate <ext_id> and pass out parameter via <entry>.  If found,
  // return 0, returns -1 if not found.

  int unbind (const EXT_ID &ext_id);
  // Unbind (remove) the <ext_id> from the tree.  Don't return the
  // <int_id> to the caller (this is useful for collections where the
  // <int_id>s are *not* dynamically allocated...)

  int unbind (const EXT_ID &ext_id,
              INT_ID &int_id);
  // Break any association of <ext_id>.  Returns the value of <int_id>
  // in case the caller needs to deallocate memory.

  int unbind (ACE_RB_Tree_Node<EXT_ID, INT_ID> *entry);
  // Remove entry from tree.  This method should be used with *extreme*
  // caution, and only for optimization purposes.  The node being passed
  // in had better have been allocated by the tree that is unbinding it.

  // = Public helper methods.

  size_t current_size (void);
  // Returns the current number of nodes in the tree.

  void operator= (const ACE_RB_Tree<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> &rbt);
  // Assignment operator.

  virtual int lessthan (const EXT_ID &k1, const EXT_ID &k2);
  // Less than comparison function for keys, using comparison functor.

  ACE_LOCK &mutex (void);
  // Returns a reference to the underlying <ACE_LOCK>.  This makes it
  // possible to acquire the lock explicitly, which can be useful in
  // some cases if you instantiate the <ACE_Atomic_Op> with an
  // <ACE_Recursive_Mutex> or <ACE_Process_Mutex>, or if you need to
  // guard the state of an iterator.  NOTE: the right name would be
  // <lock>, but HP/C++ will choke on that!

  void dump (void) const;
  // Dump the state of an object.

  // = STL styled iterator factory functions.

  ACE_RB_Tree_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> begin (void);
  // Return forward iterator positioned at first node in tree.

  ACE_RB_Tree_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> end (void);
  // Return forward iterator positioned at last node in tree.

  ACE_RB_Tree_Reverse_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> rbegin (void);
  // Return reverse iterator positioned at last node in tree.

  ACE_RB_Tree_Reverse_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> rend (void);
  // Return reverse iterator positioned at first node in tree.

  // = DEPRECATED methods.  Please migrate your code to use the new methods instead

  INT_ID* find (const EXT_ID &k);
  // Returns a pointer to the item corresponding to the
  // given key, or 0 if it cannot find the key in the tree.
  //
  // DEPRECATED: signature will change to become
  // int find (const EXT_ID &ext_id); which will return
  // 0 if the <ext_id> is in the tree, otherwise -1.



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

protected:

  // = Protected methods. These should only be called with locks held.

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

  ACE_RB_Tree_Node<EXT_ID, INT_ID> * 
    find_node (const EXT_ID &k, RB_SearchResult &result);
  // Returns a pointer to a matching node if there is one, a pointer
  // to the node under which to insert the item if the tree is not
  // empty and there is no such match, or 0 if the tree is empty.
  // It stores the result of the search in the result argument:
  // LEFT if the node is to the left of the node to be inserted,
  // RIGHT if the node is to the right of the node to be inserted,
  // or EXACT if an exactly matching node already exists.

  void RB_rebalance (ACE_RB_Tree_Node<EXT_ID, INT_ID> * x);
  // Rebalance the tree after insertion of a node.

  int close_i (void);
  // Close down an RB_Tree.  this method should
  // only be called with locks already held.

  int find_i (const EXT_ID &ext_id, ACE_RB_Tree_Node<EXT_ID, INT_ID>* &entry);
  // Retrieves a pointer to the item corresponding to the
  // given key. Returns 0 for success, or -1 if it cannot find the key
  // in the tree.

  INT_ID* insert_i (const EXT_ID &k, const INT_ID &t);
  // Inserts a *copy* of the key and the item into the tree: both the
  // key type EXT_ID and the item type INT_ID must have well defined semantics
  // for copy construction.  The default implementation also requires that
  // the key type support well defined < semantics.  This method returns a
  // pointer to the inserted item copy, or 0 if an error occurred.
  // NOTE: if an identical key already exists in the tree, no new item
  // is created, and the returned pointer addresses the existing item
  // associated with the existing key.

  int remove_i (const EXT_ID &k, INT_ID &i);
  // Removes the item associated with the given key from the tree and
  // destroys it.  Returns 1 if it found the item and successfully
  // destroyed it, 0 if it did not find the item, or -1 if an error
  // occurred.  Returns the stored internal id in the second argument.

  int remove_i (ACE_RB_Tree_Node<EXT_ID, INT_ID> *z);
  // Removes the item associated with the given key from the tree and
  // destroys it.

private:

  // = Private members.

  ACE_Allocator *allocator_;
  // Pointer to a memory allocator.

  ACE_LOCK lock_;
  // Synchronization variable for the MT_SAFE <ACE_RB_Tree>.

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
  // Move back by one element in the tree.  Returns 0 when
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

  ACE_RB_Tree_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> & operator++ (void);
  // Prefix advance.

  ACE_RB_Tree_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> operator++ (int);
  // Postfix advance.

  ACE_RB_Tree_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> & operator-- (void);
  // Prefix reverse.

  ACE_RB_Tree_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> operator-- (int);
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

  ACE_RB_Tree_Reverse_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> & operator++ (void);
  // Prefix advance.

  ACE_RB_Tree_Reverse_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> operator++ (int);
  // Postfix advance.

  ACE_RB_Tree_Reverse_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> & operator-- (void);
  // Prefix reverse.

  ACE_RB_Tree_Reverse_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> operator-- (int);
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
