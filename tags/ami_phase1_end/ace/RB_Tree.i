/* -*- C++ -*- */
// $Id$

/////////////////////////////////////////////////////
// template class ACE_RB_Tree_Node<EXT_ID, INT_ID> //
/////////////////////////////////////////////////////

// Key accessor.

template <class EXT_ID, class INT_ID> ACE_INLINE EXT_ID &
ACE_RB_Tree_Node<EXT_ID, INT_ID>::key ()
{
  return k_;
}


// Item accessor.

template <class EXT_ID, class INT_ID> ACE_INLINE INT_ID &
ACE_RB_Tree_Node<EXT_ID, INT_ID>::item ()
{
  return t_;
}


// Set color of the node.

template <class EXT_ID, class INT_ID> ACE_INLINE void
ACE_RB_Tree_Node<EXT_ID, INT_ID>::color (ACE_RB_Tree_Node_Base::RB_Tree_Node_Color c)
{
  color_ = c;
}


// Get color of the node.

template <class EXT_ID, class INT_ID>
ACE_INLINE ACE_RB_Tree_Node_Base::RB_Tree_Node_Color
ACE_RB_Tree_Node<EXT_ID, INT_ID>::color ()
{
  return color_;
}


// Accessor for node's parent pointer.

template <class EXT_ID, class INT_ID> 
ACE_INLINE ACE_RB_Tree_Node<EXT_ID, INT_ID> *
ACE_RB_Tree_Node<EXT_ID, INT_ID>::parent ()
{
  return parent_;
}


// Mutator for node's parent pointer.

template <class EXT_ID, class INT_ID> ACE_INLINE void
ACE_RB_Tree_Node<EXT_ID, INT_ID>::parent (ACE_RB_Tree_Node<EXT_ID, INT_ID> * p)
{
  parent_ = p;
}
  


// Accessor for node's left child pointer.

template <class EXT_ID, class INT_ID> 
ACE_INLINE ACE_RB_Tree_Node<EXT_ID, INT_ID> *
ACE_RB_Tree_Node<EXT_ID, INT_ID>::left ()
{
  return left_;
}


// Mutator for node's left child pointer.

template <class EXT_ID, class INT_ID> ACE_INLINE void
ACE_RB_Tree_Node<EXT_ID, INT_ID>::left (ACE_RB_Tree_Node<EXT_ID, INT_ID> * l)
{
  left_ = l;
}


// Accessor for node's right child pointer.

template <class EXT_ID, class INT_ID> 
ACE_INLINE ACE_RB_Tree_Node<EXT_ID, INT_ID> *
ACE_RB_Tree_Node<EXT_ID, INT_ID>::right ()
{
  return right_;
}


// Mutator for node's right child pointer.

template <class EXT_ID, class INT_ID> ACE_INLINE void
ACE_RB_Tree_Node<EXT_ID, INT_ID>::right (ACE_RB_Tree_Node<EXT_ID, INT_ID> * r)
{
  right_ = r;
}



////////////////////////////////////////////////////////////////////////
// template class ACE_RB_Tree<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> //
////////////////////////////////////////////////////////////////////////


// Destroys all nodes and sets the root pointer null.

template <class EXT_ID, class INT_ID, class COMPARE_KEYS, class ACE_LOCK> 
ACE_INLINE void
ACE_RB_Tree<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK>::clear ()
{
  delete root_;
  root_ = 0;
  current_size_ = 0;
}

// Returns the current number of nodes in the tree.

template <class EXT_ID, class INT_ID, class COMPARE_KEYS, class ACE_LOCK>
ACE_INLINE size_t 
ACE_RB_Tree<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK>::current_size ()
{
  return current_size_;
}



//////////////////////////////////////////////////////////////////
// template class                                               //
// ACE_RB_Tree_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK> //
//////////////////////////////////////////////////////////////////

// Accessor for key of node under iterator (if any).

template <class EXT_ID, class INT_ID, class COMPARE_KEYS, class ACE_LOCK>
ACE_INLINE EXT_ID *
ACE_RB_Tree_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK>::key ()
{
  return node_ ? (&(node_->key ())) : 0;
}


// Accessor for item of node under iterator (if any).

template <class EXT_ID, class INT_ID, class COMPARE_KEYS, class ACE_LOCK> 
ACE_INLINE INT_ID *
ACE_RB_Tree_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK>::item ()
{
  return node_ ? (&(node_->item ())) : 0;
}


// Move to the first item in the tree.

template <class EXT_ID, class INT_ID, class COMPARE_KEYS, class ACE_LOCK>
ACE_INLINE int
ACE_RB_Tree_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK>::first ()
{
  node_ = tree_.RB_tree_minimum (tree_.root_);
  return node_ ? 1 : 0;
}


// Move to the last item in the tree.

template <class EXT_ID, class INT_ID, class COMPARE_KEYS, class ACE_LOCK>
ACE_INLINE int
ACE_RB_Tree_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK>::last ()
{
  node_ = tree_.RB_tree_maximum (tree_.root_);
  return node_ ? 1 : 0;
}


// Moves to the next item in the tree,
// returns 1 if there is a next item, 0 otherwise.

template <class EXT_ID, class INT_ID, class COMPARE_KEYS, class ACE_LOCK> 
ACE_INLINE int
ACE_RB_Tree_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK>::next ()
{
  node_ = tree_.RB_tree_successor (node_);
  return node_ ? 1 : 0;
}


// Moves to the previous item in the tree,
// returns 1 if there is a previous item, 0 otherwise.

template <class EXT_ID, class INT_ID, class COMPARE_KEYS, class ACE_LOCK> 
ACE_INLINE int
ACE_RB_Tree_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK>::previous ()
{
  node_ = tree_.RB_tree_predecessor (node_);
  return node_ ? 1 : 0;
}

template <class EXT_ID, class INT_ID, class COMPARE_KEYS, class ACE_LOCK> 
ACE_INLINE int
ACE_RB_Tree_Iterator<EXT_ID, INT_ID, COMPARE_KEYS, ACE_LOCK>::is_done ()
{
  return node_ ? 0 : 1;
}
