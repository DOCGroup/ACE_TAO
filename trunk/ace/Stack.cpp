// Stack.cpp
// $Id$

#if !defined (ACE_STACK_C)
#define ACE_STACK_C

#define ACE_BUILD_DLL
#include "ace/Stack.h"

#if !defined (__ACE_INLINE__)
#include "ace/Stack.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Bounded_Stack)

template <class T> void
ACE_Bounded_Stack<T>::dump (void) const
{
  ACE_TRACE ("ACE_Bounded_Stack<T>::dump");
}

template<class T>
ACE_Bounded_Stack<T>::ACE_Bounded_Stack (size_t size)
  : top_ (0),
    size_ (size)
{
  ACE_NEW (this->stack_, T[size]);

  ACE_TRACE ("ACE_Bounded_Stack<T>::ACE_Bounded_Stack");
}

template<class T>
ACE_Bounded_Stack<T>::ACE_Bounded_Stack (const ACE_Bounded_Stack<T> &s)
  : top_ (s.top_),
    size_ (s.size_)
{
  ACE_NEW (this->stack_, T[s.size_]);

  ACE_TRACE ("ACE_Bounded_Stack<T>::ACE_Bounded_Stack");

  for (size_t i = 0; i < this->top_; i++)
    this->stack_[i] = s.stack_[i];
}

template<class T> void
ACE_Bounded_Stack<T>::operator= (const ACE_Bounded_Stack<T> &s)
{
  ACE_TRACE ("ACE_Bounded_Stack<T>::operator=");
  if (&s == this)
    return;
  else if (this->size_ < s.size_)
    {
      delete [] this->stack_;
      ACE_NEW (this->stack_, T[s.size_]);
    }
  this->top_ = s.top_;

  for (size_t i = 0; i < this->top_; i++)
    this->stack_[i] = s.stack_[i];
}

template<class T>
ACE_Bounded_Stack<T>::~ACE_Bounded_Stack (void) 
{
  ACE_TRACE ("ACE_Bounded_Stack<T>::~ACE_Bounded_Stack");
  delete [] this->stack_;
}

// ----------------------------------------

ACE_ALLOC_HOOK_DEFINE(ACE_Fixed_Stack)

template <class T, size_t SIZE> void
ACE_Fixed_Stack<T, SIZE>::dump (void) const
{
  ACE_TRACE ("ACE_Fixed_Stack<T, SIZE>::dump");
}

template<class T, size_t SIZE>
ACE_Fixed_Stack<T, SIZE>::ACE_Fixed_Stack (void)
  : top_ (0),
    size_ (SIZE)
{
  ACE_TRACE ("ACE_Fixed_Stack<T, SIZE>::ACE_Fixed_Stack");
}

template<class T, size_t SIZE>
ACE_Fixed_Stack<T, SIZE>::ACE_Fixed_Stack (const ACE_Fixed_Stack<T, SIZE> &s)
  : top_ (s.top_),
    size_ (s.size_)
{
  ACE_TRACE ("ACE_Fixed_Stack<T, SIZE>::ACE_Fixed_Stack");
  for (size_t i = 0; i < this->top_; i++)
    this->stack_[i] = s.stack_[i];
}

template<class T, size_t SIZE> void
ACE_Fixed_Stack<T, SIZE>::operator= (const ACE_Fixed_Stack<T, SIZE> &s)
{
  ACE_TRACE ("ACE_Fixed_Stack<T, SIZE>::operator=");
  if (&s == this)
    return;
  this->top_ = s.top_;

  for (size_t i = 0; i < this->top_; i++)
    this->stack_[i] = s.stack_[i];
}

template<class T, size_t SIZE>
ACE_Fixed_Stack<T, SIZE>::~ACE_Fixed_Stack (void) 
{
  ACE_TRACE ("ACE_Fixed_Stack<T, SIZE>::~ACE_Fixed_Stack");
  delete [] this->stack_;
}

//----------------------------------------

template<class T>
class ACE_Stack_Node
{
friend class ACE_Unbounded_Stack<T>;
private:
#if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
  // = Only use a free list if the compiler supports static data
  // members...

  void *operator new (size_t bytes);
  void  operator delete (void *ptr);

  // Returns all dynamic memory on the free list to the free store.
  static void free_all_nodes (void);

  static ACE_Stack_Node<T> *free_list_;
  // Head of the free list of Nodes used to speed up allocation.
#endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */

  ACE_Stack_Node (T i, ACE_Stack_Node<T> *n);
  ACE_Stack_Node (void);

  ACE_Stack_Node<T> *next_;
  T item_;
};

#if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
/* static */
template<class T> ACE_Stack_Node<T> *
ACE_Stack_Node<T>::free_list_ = 0;

template<class T> void *
ACE_Stack_Node<T>::operator new (size_t bytes)
{
  ACE_TRACE ("ACE_Stack_Node<T>::operator new");
  ACE_Stack_Node<T> *temp = ACE_Stack_Node<T>::free_list_;

  if (temp)
    ACE_Stack_Node<T>::free_list_ = ACE_Stack_Node<T>::free_list_->next_;
  else
    temp = (ACE_Stack_Node<T> *) new char[bytes];

  return temp;
}

template<class T> void
ACE_Stack_Node<T>::operator delete (void *ptr)
{
  ACE_TRACE ("ACE_Stack_Node<T>::operator delete");
  ((ACE_Stack_Node<T> *) ptr)->next_ = ACE_Stack_Node<T>::free_list_;
  ACE_Stack_Node<T>::free_list_ = (ACE_Stack_Node<T> *) ptr;
}

template<class T> void 
ACE_Stack_Node<T>::free_all_nodes (void)
{
  ACE_TRACE ("ACE_Stack_Node<T>::free_all_nodes");

  while (ACE_Stack_Node<T>::free_list_)
    {
      ACE_Stack_Node<T> *temp = ACE_Stack_Node<T>::free_list_;
      ACE_Stack_Node<T>::free_list_ = ACE_Stack_Node<T>::free_list_->next_;
      ::delete temp;
    }
}

#endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */

template<class T>
ACE_Stack_Node<T>::ACE_Stack_Node (T i, ACE_Stack_Node<T> *n)
  : next_ (n), 
    item_ (i)
{
  ACE_TRACE ("ACE_Stack_Node<T>::ACE_Stack_Node");
}

template<class T>
ACE_Stack_Node<T>::ACE_Stack_Node (void)
  : next_ (0)
{
  ACE_TRACE ("ACE_Stack_Node<T>::ACE_Stack_Node");
}

ACE_ALLOC_HOOK_DEFINE(ACE_Unbounded_Stack)

template <class T> void
ACE_Unbounded_Stack<T>::dump (void) const
{
  ACE_TRACE ("ACE_Unbounded_Stack<T>::dump");
}

template<class T>
ACE_Unbounded_Stack<T>::ACE_Unbounded_Stack (void)
  : head_ (0)
{
  ACE_NEW (this->last_resort_, ACE_Stack_Node<T>);
  ACE_TRACE ("ACE_Unbounded_Stack<T>::ACE_Unbounded_Stack");
}

template<class T> void
ACE_Unbounded_Stack<T>::delete_all_nodes (void)
{
  ACE_TRACE ("ACE_Unbounded_Stack<T>::delete_all_nodes");
  while (this->head_ != 0)
    {
      ACE_Stack_Node<T> *temp = this->head_;
      this->head_ = this->head_->next_;
      delete temp;
    }

  delete this->last_resort_;
  this->last_resort_ = 0;
} 

template<class T> void
ACE_Unbounded_Stack<T>::copy_all_nodes (const ACE_Unbounded_Stack<T> &s)
{
  ACE_TRACE ("ACE_Unbounded_Stack<T>::copy_all_nodes");
  // Push all of <s>'s nodes onto our stack (this puts them in the
  // reverse order).
  ACE_Stack_Node<T> *temp;

  for (temp = s.head_;
       temp != 0;
       temp = temp->next_)
    {
      if (!this->is_full ())
	this->push (temp->item_);
      else 
	break;
    }

  // Reverse the order of our stack.

  ACE_Stack_Node<T> *prev = 0;

  for (temp = this->head_; temp != 0; )
    {
      ACE_Stack_Node<T> *next = temp->next_;

      temp->next_ = prev;
      prev = temp;
      temp = next;
    }

  this->head_ = prev;
}

template<class T>
ACE_Unbounded_Stack<T>::ACE_Unbounded_Stack (const ACE_Unbounded_Stack<T> &s)
  : head_ (0)
{
  ACE_NEW (this->last_resort_, ACE_Stack_Node<T>);

  ACE_TRACE ("ACE_Unbounded_Stack<T>::ACE_Unbounded_Stack");
  this->copy_all_nodes (s);
}

template<class T> void
ACE_Unbounded_Stack<T>::operator= (const ACE_Unbounded_Stack<T> &s)
{
  ACE_TRACE ("ACE_Unbounded_Stack<T>::operator=");
  if (this == &s)
    return;

  this->delete_all_nodes ();
  this->copy_all_nodes (s);
}

template<class T>
ACE_Unbounded_Stack<T>::~ACE_Unbounded_Stack (void) 
{
  ACE_TRACE ("ACE_Unbounded_Stack<T>::~ACE_Unbounded_Stack");
  this->delete_all_nodes ();
}

template<class T> void 
ACE_Unbounded_Stack<T>::push (const T &new_item)
{
  ACE_TRACE ("ACE_Unbounded_Stack<T>::push");

  ACE_Stack_Node<T> *temp = new ACE_Stack_Node<T> (new_item, this->head_);

  if (temp == 0)
    {
      temp = this->last_resort_;
      this->last_resort_ = 0;
    }

  this->head_ = temp;
}

template<class T> void
ACE_Unbounded_Stack<T>::pop (T &item)
{
  ACE_TRACE ("ACE_Unbounded_Stack<T>::pop");
  item = this->head_->item_;
  ACE_Stack_Node<T> *temp = this->head_;
  this->head_ = this->head_->next_;

  // Restore the node of last resort if necessary.
  if (this->last_resort_ == 0)
    this->last_resort_ = temp;
  else
    delete temp;
}

template<class T> void
ACE_Unbounded_Stack<T>::delete_free_list (void)
{
  ACE_TRACE ("ACE_Unbounded_Stack<T>::delete_free_list");
#if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
  ACE_Stack_Node<T>::free_all_nodes ();
#endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */
}

template<class T>
class ACE_Queue_Node
{
friend class ACE_Unbounded_Queue<T>;
  ACE_Queue_Node (T i, ACE_Queue_Node<T> *n);

  ACE_Queue_Node<T> *next_;
  T item_;
};

template<class T>
ACE_Queue_Node<T>::ACE_Queue_Node (T i, ACE_Queue_Node<T> *n)
  : next_ (n), 
    item_ (i)
{
  ACE_TRACE ("ACE_Queue_Node<T>::ACE_Queue_Node");
}

template <class TYPE>
ACE_Unbounded_Queue<TYPE>::ACE_Unbounded_Queue (void)
  : head_ (0),
    tail_ (0),
    cur_size_ (0)
{
  ACE_TRACE ("ACE_Unbounded_Queue<TYPE>::ACE_Unbounded_Queue (void)");
}

ACE_ALLOC_HOOK_DEFINE(ACE_Unbounded_Queue)

template <class TYPE> void
ACE_Unbounded_Queue<TYPE>::dump (void) const
{
  ACE_TRACE ("ACE_Unbounded_Queue<TYPE>::dump");
}

template <class TYPE>
ACE_Unbounded_Queue<TYPE>::~ACE_Unbounded_Queue (void)
{
  ACE_TRACE ("ACE_Unbounded_Queue<TYPE>::~ACE_Unbounded_Queue (void)");
  ACE_Queue_Node<TYPE> *temp = head_;
  while (temp != 0)
    {
      head_ = head_->next_;
      delete temp;
      temp = head_;
      this->cur_size_--;
    }
}

template <class TYPE> int
ACE_Unbounded_Queue<TYPE>::enqueue (const TYPE &new_item)
{
  ACE_TRACE ("ACE_Unbounded_Queue<TYPE>::enqueue (const TYPE& new_item)");

  ACE_Queue_Node<TYPE> *temp = new ACE_Queue_Node<TYPE> (new_item, 0);

  if (temp == 0)
    return -1;

  if (head_ == 0)
    head_ = tail_ = temp;
  else
    {
      tail_->next_ = temp;
      tail_ = temp;
    }

  ++this->cur_size_;

  return 0;
}

template <class TYPE> int
ACE_Unbounded_Queue<TYPE>::dequeue (TYPE &item)
{
  ACE_TRACE ("ACE_Unbounded_Queue<TYPE>::dequeue (TYPE *&item)");

  if (head_ == 0)
    return -1;

  item = head_->item_;
  ACE_Queue_Node<TYPE> *temp = head_;
  head_ = head_->next_;
  delete temp;
  --this->cur_size_;
  return 0;
}

template <class TYPE> int
ACE_Unbounded_Queue<TYPE>::size (void) const
{
  return this->cur_size_;
}

#endif /* ACE_STACK_C */
