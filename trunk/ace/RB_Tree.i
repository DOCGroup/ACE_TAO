/* -*- C++ -*- */
// $Id$

/////////////////////////////////////////
// template class RB_Tree_Node<KEY, T> //
/////////////////////////////////////////

template <class KEY, class T> ACE_INLINE KEY &
RB_Tree_Node<KEY, T>::key ()
{
  return k_;
}
  // key accessor

template <class KEY, class T> ACE_INLINE T &
RB_Tree_Node<KEY, T>::item ()
{
  return t_;
}
  // item accessor

template <class KEY, class T> ACE_INLINE void
RB_Tree_Node<KEY, T>::color (RB_Tree_Node_Base::RB_Tree_Node_Color c)
{
  color_ = c;
}
  // set color of the node

template <class KEY, class T>
ACE_INLINE RB_Tree_Node_Base::RB_Tree_Node_Color
RB_Tree_Node<KEY, T>::color ()
{
  return color_;
}
  // get color of the node


template <class KEY, class T> ACE_INLINE RB_Tree_Node<KEY, T> *
RB_Tree_Node<KEY, T>::parent ()
{
  return parent_;
}
  // accessor for node's parent pointer

template <class KEY, class T> ACE_INLINE void
RB_Tree_Node<KEY, T>::parent (RB_Tree_Node<KEY, T> * p)
{
  parent_ = p;
}
  // mutator for node's parent pointer

template <class KEY, class T> ACE_INLINE RB_Tree_Node<KEY, T> *
RB_Tree_Node<KEY, T>::left ()
{
  return left_;
}
  // accessor for node's left child pointer

template <class KEY, class T> ACE_INLINE void
RB_Tree_Node<KEY, T>::left (RB_Tree_Node<KEY, T> * l)
{
  left_ = l;
}
  // mutator for node's left child pointer

template <class KEY, class T> ACE_INLINE RB_Tree_Node<KEY, T> *
RB_Tree_Node<KEY, T>::right ()
{
  return right_;
}
  // accessor for node's right child pointer

template <class KEY, class T> ACE_INLINE void
RB_Tree_Node<KEY, T>::right (RB_Tree_Node<KEY, T> * r)
{
  right_ = r;
}
  // mutator for node's right child pointer


////////////////////////////////////
// template class RB_Tree<KEY, T> //
////////////////////////////////////

template <class KEY, class T> ACE_INLINE void
RB_Tree<KEY, T>::clear ()
{
  delete root_;
  root_ = 0;
}
  // destroys all nodes and sets the root pointer null.


/////////////////////////////////////////////
// template class RB_Tree_Iterator<KEY, T> //
/////////////////////////////////////////////



template <class KEY, class T> ACE_INLINE KEY *
RB_Tree_Iterator<KEY, T>::key ()
{
  return node_ ? (&(node_->key ())) : 0;
}
  // accessor for key of node under iterator (if any)

template <class KEY, class T> ACE_INLINE T *
RB_Tree_Iterator<KEY, T>::item ()
{
  return node_ ? (&(node_->item ())) : 0;
}
  // accessor for item of node under iterator (if any)

template <class KEY, class T> ACE_INLINE int
RB_Tree_Iterator<KEY, T>::first ()
{
  node_ = tree_.RB_tree_minimum (tree_.root_);
  return node_ ? 1 : 0;
}
  // move to the first item in the tree

template <class KEY, class T> ACE_INLINE int
RB_Tree_Iterator<KEY, T>::last ()
{
  node_ = tree_.RB_tree_maximum (tree_.root_);
  return node_ ? 1 : 0;
}
  // move to the last item in the tree

template <class KEY, class T> ACE_INLINE int
RB_Tree_Iterator<KEY, T>::next ()
{
  node_ = tree_.RB_tree_successor (node_);
  return node_ ? 1 : 0;
}
  // move to the next item in the tree
  // returns 1 if there is a next item, 0 otherwise

template <class KEY, class T> ACE_INLINE int
RB_Tree_Iterator<KEY, T>::previous ()
{
  node_ = tree_.RB_tree_predecessor (node_);
  return node_ ? 1 : 0;
}
  // move to the previous item in the tree
  // returns 1 if there is a previous item, 0 otherwise

template <class KEY, class T> ACE_INLINE int
RB_Tree_Iterator<KEY, T>::is_done ()
{
  return node_ ? 0 : 1;
}
