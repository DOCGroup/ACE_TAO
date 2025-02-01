// -*- C++ -*-

//=============================================================================
/**
 *  @file Node.h
 *
 *  @author Doug Schmidt
 */
//=============================================================================


#ifndef ACE_NODE_H
#define ACE_NODE_H
#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward declarations.
template <class T, class C> class ACE_Unbounded_Set_Ex;
template <class T, class C> class ACE_Unbounded_Set_Ex_Iterator;
template <class T, class C> class ACE_Unbounded_Set_Ex_Const_Iterator;
template <class T> class ACE_Unbounded_Queue;
template <class T> class ACE_Unbounded_Queue_Iterator;
template <class T> class ACE_Unbounded_Queue_Const_Iterator;
template <class T> class ACE_Unbounded_Stack;
template <class T> class ACE_Unbounded_Stack_Iterator;

/**
 * @class ACE_Node
 *
 * @brief Implementation element in a Queue, Set, and Stack.
 */
template<class T, class C = void>
class ACE_Node
{
public:
  friend class ACE_Unbounded_Queue<T>;
  friend class ACE_Unbounded_Queue_Iterator<T>;
  friend class ACE_Unbounded_Queue_Const_Iterator<T>;
  friend class ACE_Unbounded_Set_Ex<T, C>;
  friend class ACE_Unbounded_Set_Ex_Iterator<T, C>;
  friend class ACE_Unbounded_Set_Ex_Const_Iterator<T, C>;
  friend class ACE_Unbounded_Stack<T>;
  friend class ACE_Unbounded_Stack_Iterator<T>;

  ~ACE_Node () = default;

  ACE_ALLOC_HOOK_DECLARE;

private:
  ACE_Node (const T &i, ACE_Node<T, C> *n);
  ACE_Node (ACE_Node<T, C> *n = nullptr, int = 0);
  ACE_Node (const ACE_Node<T, C> &n);
private:
  void operator= (const ACE_Node<T, C> &) = delete;

private:
  /// Pointer to next element in the list of ACE_Nodes.
  ACE_Node<T, C> *next_;

  /// Current value of the item in this node.
  T item_;
};

ACE_END_VERSIONED_NAMESPACE_DECL

#include "ace/Node.cpp"

#include /**/ "ace/post.h"
#endif /* ACE_NODE_H */
