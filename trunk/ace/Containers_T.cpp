// $Id$

#ifndef ACE_CONTAINERS_T_C
#define ACE_CONTAINERS_T_C

#define ACE_BUILD_DLL

#include "ace/Malloc.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Containers.h"

#if !defined (__ACE_INLINE__)
#include "ace/Containers_T.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Containers_T, "$Id$")

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

  if (&s != this)
    {
      if (this->size_ < s.size_)
        {
          delete [] this->stack_;
          ACE_NEW (this->stack_,
                   T[s.size_]);
        }
      this->top_ = s.top_;

      for (size_t i = 0; i < this->top_; i++)
        this->stack_[i] = s.stack_[i];
    }
}

template<class T>
ACE_Bounded_Stack<T>::~ACE_Bounded_Stack (void)
{
  ACE_TRACE ("ACE_Bounded_Stack<T>::~ACE_Bounded_Stack");
  delete [] this->stack_;
}

// ----------------------------------------

ACE_ALLOC_HOOK_DEFINE(ACE_Fixed_Stack)

template <class T, size_t ACE_SIZE> void
ACE_Fixed_Stack<T, ACE_SIZE>::dump (void) const
{
  ACE_TRACE ("ACE_Fixed_Stack<T, ACE_SIZE>::dump");
}

template<class T, size_t ACE_SIZE>
ACE_Fixed_Stack<T, ACE_SIZE>::ACE_Fixed_Stack (void)
  : top_ (0),
    size_ (ACE_SIZE)
{
  ACE_TRACE ("ACE_Fixed_Stack<T, ACE_SIZE>::ACE_Fixed_Stack");
}

template<class T, size_t ACE_SIZE>
ACE_Fixed_Stack<T, ACE_SIZE>::ACE_Fixed_Stack (const ACE_Fixed_Stack<T, ACE_SIZE> &s)
  : top_ (s.top_),
    size_ (s.size_)
{
  ACE_TRACE ("ACE_Fixed_Stack<T, ACE_SIZE>::ACE_Fixed_Stack");
  for (size_t i = 0; i < this->top_; i++)
    this->stack_[i] = s.stack_[i];
}

template<class T, size_t ACE_SIZE> void
ACE_Fixed_Stack<T, ACE_SIZE>::operator= (const ACE_Fixed_Stack<T, ACE_SIZE> &s)
{
  ACE_TRACE ("ACE_Fixed_Stack<T, ACE_SIZE>::operator=");

  if (&s != this)
    {
      this->top_ = s.top_;

      for (size_t i = 0; i < this->top_; i++)
        this->stack_[i] = s.stack_[i];
    }
}

template<class T, size_t ACE_SIZE>
ACE_Fixed_Stack<T, ACE_SIZE>::~ACE_Fixed_Stack (void)
{
  ACE_TRACE ("ACE_Fixed_Stack<T, ACE_SIZE>::~ACE_Fixed_Stack");
}

//----------------------------------------

ACE_ALLOC_HOOK_DEFINE(ACE_Unbounded_Stack)

template <class T> void
ACE_Unbounded_Stack<T>::dump (void) const
{
  //  ACE_TRACE ("ACE_Unbounded_Stack<T>::dump");
}

template<class T>
ACE_Unbounded_Stack<T>::ACE_Unbounded_Stack (ACE_Allocator *alloc)
  : head_ (0),
    cur_size_ (0),
    allocator_ (alloc)
{
  //  ACE_TRACE ("ACE_Unbounded_Stack<T>::ACE_Unbounded_Stack");
  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  ACE_NEW_MALLOC (this->head_,
                  (ACE_Node<T> *) this->allocator_->malloc (sizeof (ACE_Node<T>)),
                  ACE_Node<T>);
  this->head_->next_ = this->head_;
}

template<class T> void
ACE_Unbounded_Stack<T>::delete_all_nodes (void)
{
  //  ACE_TRACE ("ACE_Unbounded_Stack<T>::delete_all_nodes");

  while (this->is_empty () == 0)
    {
      ACE_Node<T> *temp = this->head_->next_;
      this->head_->next_ = temp->next_;
      ACE_DES_FREE_TEMPLATE (temp, this->allocator_->free,
                             ACE_Node, <T>);
    }

  this->cur_size_ = 0;

  ACE_ASSERT (this->head_ == this->head_->next_
              && this->is_empty ());
}

template<class T> void
ACE_Unbounded_Stack<T>::copy_all_nodes (const ACE_Unbounded_Stack<T> &s)
{
  //  ACE_TRACE ("ACE_Unbounded_Stack<T>::copy_all_nodes");

  ACE_ASSERT (this->head_ == this->head_->next_);

  ACE_Node<T> *temp = this->head_;

  for (ACE_Node<T> *s_temp = s.head_->next_;
       s_temp != s.head_;
       s_temp = s_temp->next_)
    {
      ACE_Node<T> *nptr = temp->next_;
      ACE_NEW_MALLOC (temp->next_,
                      (ACE_Node<T> *) this->allocator_->malloc (sizeof (ACE_Node<T>)),
                      ACE_Node<T> (s_temp->item_, nptr));
      temp = temp->next_;
    }
  this->cur_size_ = s.cur_size_;
}

template<class T>
ACE_Unbounded_Stack<T>::ACE_Unbounded_Stack (const ACE_Unbounded_Stack<T> &s)
  : head_ (0),
    cur_size_ (0),
    allocator_ (s.allocator_)
{
  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  ACE_NEW_MALLOC (this->head_,
                  (ACE_Node<T> *) this->allocator_->malloc (sizeof (ACE_Node<T>)),
                  ACE_Node<T>);
  this->head_->next_ = this->head_;

  //  ACE_TRACE ("ACE_Unbounded_Stack<T>::ACE_Unbounded_Stack");
  this->copy_all_nodes (s);
}

template<class T> void
ACE_Unbounded_Stack<T>::operator= (const ACE_Unbounded_Stack<T> &s)
{
  //  ACE_TRACE ("ACE_Unbounded_Stack<T>::operator=");

  if (this != &s)
    {
      this->delete_all_nodes ();
      this->copy_all_nodes (s);
    }
}

template<class T>
ACE_Unbounded_Stack<T>::~ACE_Unbounded_Stack (void)
{
  //  ACE_TRACE ("ACE_Unbounded_Stack<T>::~ACE_Unbounded_Stack");

  this->delete_all_nodes ();
  ACE_DES_FREE_TEMPLATE (head_,
                         this->allocator_->free,
                         ACE_Node,
                         <T>);
}

template<class T> int
ACE_Unbounded_Stack<T>::push (const T &new_item)
{
  //  ACE_TRACE ("ACE_Unbounded_Stack<T>::push");

  ACE_Node<T> *temp;

  ACE_NEW_MALLOC_RETURN (temp,
                         (ACE_Node<T> *) this->allocator_->malloc (sizeof (ACE_Node<T>)),
                         ACE_Node<T> (new_item, this->head_->next_),
                         -1);

  this->head_->next_ = temp;
  this->cur_size_++;
  return 0;
}

template<class T> int
ACE_Unbounded_Stack<T>::pop (T &item)
{
  //  ACE_TRACE ("ACE_Unbounded_Stack<T>::pop");

  if (this->is_empty ())
    return -1;
  else
    {
      ACE_Node<T> *temp = this->head_->next_;
      item = temp->item_;
      this->head_->next_ = temp->next_;

      ACE_DES_FREE_TEMPLATE (temp,
                             this->allocator_->free,
                             ACE_Node,
                             <T>);
      this->cur_size_--;
      return 0;
    }
}

template <class T> int
ACE_Unbounded_Stack<T>::find (const T &item) const
{
// ACE_TRACE ("ACE_Unbounded_Stack<T>::find");
  // Set <item> into the dummy node.
  this->head_->item_ = item;

  ACE_Node<T> *temp = this->head_->next_;

  // Keep looping until we find the item.
  while (!(temp->item_ == item))
    temp = temp->next_;

  // If we found the dummy node then it's not really there, otherwise,
  // it is there.
  return temp == this->head_ ? -1 : 0;
}

template <class T> int
ACE_Unbounded_Stack<T>::insert (const T &item)
{
// ACE_TRACE ("ACE_Unbounded_Stack<T>::insert");

  if (this->find (item) == 0)
    return 1;
  else
    return this->push (item);
}

template <class T> int
ACE_Unbounded_Stack<T>::remove (const T &item)
{
// ACE_TRACE ("ACE_Unbounded_Stack<T>::remove");

  // Insert the item to be founded into the dummy node.
  this->head_->item_ = item;

  ACE_Node<T> *curr = this->head_;

  while (!(curr->next_->item_ == item))
    curr = curr->next_;

  if (curr->next_ == this->head_)
    return -1; // Item was not found.
  else
    {
      ACE_Node<T> *temp = curr->next_;
      // Skip over the node that we're deleting.
      curr->next_ = temp->next_;
      this->cur_size_--;
      ACE_DES_FREE_TEMPLATE (temp,
                             this->allocator_->free,
                             ACE_Node,
                             <T>);
      return 0;
    }
}

template <class T>
ACE_Unbounded_Queue<T>::ACE_Unbounded_Queue (ACE_Allocator *alloc)
  : head_ (0),
    cur_size_ (0),
    allocator_ (alloc)
{
//   ACE_TRACE ("ACE_Unbounded_Queue<T>::ACE_Unbounded_Queue (void)");

  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  ACE_NEW_MALLOC (this->head_,
                  (ACE_Node<T> *) this->allocator_->malloc (sizeof (ACE_Node<T>)),
                  ACE_Node<T>);

  // Make the list circular by pointing it back to itself.
  this->head_->next_ = this->head_;
}

template <class T>
ACE_Unbounded_Queue<T>::ACE_Unbounded_Queue (const ACE_Unbounded_Queue<T> &us)
  : head_ (0),
    cur_size_ (0),
    allocator_ (us.allocator_)
{
//   ACE_TRACE ("ACE_Unbounded_Queue<T>::ACE_Unbounded_Queue");

  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  ACE_NEW_MALLOC (this->head_,
                  (ACE_Node<T> *) this->allocator_->malloc (sizeof (ACE_Node<T>)),
                  ACE_Node<T>);
  this->head_->next_ = this->head_;
  this->copy_nodes (us);
}

template <class T> void
ACE_Unbounded_Queue<T>::operator= (const ACE_Unbounded_Queue<T> &us)
{
//   ACE_TRACE ("ACE_Unbounded_Queue<T>::operator=");

  if (this != &us)
    {
      this->delete_nodes ();
      this->copy_nodes (us);
    }
}

template <class T> ACE_Unbounded_Queue_Iterator<T>
ACE_Unbounded_Queue<T>::begin (void)
{
  // ACE_TRACE ("ACE_Unbounded_Queue<T>::begin");
  return ACE_Unbounded_Queue_Iterator<T> (*this);
}

template <class T> ACE_Unbounded_Queue_Iterator<T>
ACE_Unbounded_Queue<T>::end (void)
{
  // ACE_TRACE ("ACE_Unbounded_Queue<T>::end");
  return ACE_Unbounded_Queue_Iterator<T> (*this, 1);
}


ACE_ALLOC_HOOK_DEFINE(ACE_Unbounded_Queue)

template <class T> void
ACE_Unbounded_Queue<T>::dump (void) const
{
//   ACE_TRACE ("ACE_Unbounded_Queue<T>::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("\nhead_ = %u"), this->head_));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("\nhead_->next_ = %u"), this->head_->next_));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("\ncur_size_ = %d\n"), this->cur_size_));

  T *item = 0;
#if !defined (ACE_NLOGGING)
  size_t count = 1;
#endif /* ! ACE_NLOGGING */

  for (ACE_Unbounded_Queue_Iterator<T> iter (*(ACE_Unbounded_Queue<T> *) this);
       iter.next (item) != 0;
       iter.advance ())
    ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("count = %d\n"), count++));

  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class T> void
ACE_Unbounded_Queue<T>::copy_nodes (const ACE_Unbounded_Queue<T> &us)
{
  for (ACE_Node<T> *curr = us.head_->next_;
       curr != us.head_;
       curr = curr->next_)
    if (this->enqueue_tail (curr->item_) == -1)
      // @@ What's the right thing to do here?
      this->delete_nodes ();
}

template <class T> void
ACE_Unbounded_Queue<T>::delete_nodes (void)
{
  ACE_Node<T> *curr = this->head_->next_;

  // Keep looking until we've hit the dummy node.

  while (curr != this->head_)
    {
      ACE_Node<T> *temp = curr;
      curr = curr->next_;

      ACE_DES_FREE_TEMPLATE (temp,
                             this->allocator_->free,
                             ACE_Node,
                             <T>);
      this->cur_size_--;
    }

  // Reset the list to be a circular list with just a dummy node.
  this->head_->next_ = this->head_;
}

template <class T>
ACE_Unbounded_Queue<T>::~ACE_Unbounded_Queue (void)
{
//   ACE_TRACE ("ACE_Unbounded_Queue<T>::~ACE_Unbounded_Queue (void)");

  this->delete_nodes ();
  ACE_DES_FREE_TEMPLATE (head_,
                         this->allocator_->free,
                         ACE_Node,
                         <T>);
  this->head_ = 0;
}

template <class T> int
ACE_Unbounded_Queue<T>::enqueue_head (const T &new_item)
{
//   ACE_TRACE ("ACE_Unbounded_Queue<T>::enqueue_tail");

  ACE_Node<T> *temp;

  // Create a new node that points to the original head.
  ACE_NEW_MALLOC_RETURN (temp,
                         (ACE_Node<T> *) this->allocator_->malloc (sizeof (ACE_Node<T>)),
                         ACE_Node<T> (new_item, this->head_->next_),
                         -1);

  // Link this pointer into the front of the list.
  this->head_->next_ = temp;

  this->cur_size_++;
  return 0;
}

template <class T> int
ACE_Unbounded_Queue<T>::enqueue_tail (const T &new_item)
{
//   ACE_TRACE ("ACE_Unbounded_Queue<T>::enqueue_head");

  ACE_Node<T> *temp;

  // Insert <item> into the old dummy node location.
  this->head_->item_ = new_item;

  // Create a new dummy node.
  ACE_NEW_MALLOC_RETURN (temp,
                         (ACE_Node<T> *) this->allocator_->malloc (sizeof (ACE_Node<T>)),
                         ACE_Node<T> (this->head_->next_), -1);

  // Link this dummy pointer into the list.
  this->head_->next_ = temp;

  // Point the head to the new dummy node.
  this->head_ = temp;

  this->cur_size_++;
  return 0;
}

template <class T> int
ACE_Unbounded_Queue<T>::dequeue_head (T &item)
{
//   ACE_TRACE ("ACE_Unbounded_Queue<T>::dequeue_head");

  // Check for empty queue.
  if (this->is_empty ())
    return -1;

  ACE_Node<T> *temp = this->head_->next_;

  item = temp->item_;
  this->head_->next_ = temp->next_;
  ACE_DES_FREE_TEMPLATE (temp,
                         this->allocator_->free,
                         ACE_Node,
                         <T>);
  --this->cur_size_;
  return 0;
}

template <class T> void
ACE_Unbounded_Queue<T>::reset (void)
{
  ACE_TRACE ("reset");

  this->delete_nodes ();
}

template <class T> int
ACE_Unbounded_Queue<T>::get (T *&item, size_t index) const
{
//   ACE_TRACE ("ACE_Unbounded_Queue<T>::get");

  ACE_Node<T> *curr = this->head_->next_;

  size_t i;

  for (i = 0; i < this->cur_size_; i++)
    {
      if (i == index)
        break;

      curr = curr->next_;
    }

  if (i < this->cur_size_)
    {
      item = &curr->item_;
      return 0;
    }
  else
    return -1;
}

template <class T> int
ACE_Unbounded_Queue<T>::set (const T &item,
                             size_t index)
{
//   ACE_TRACE ("ACE_Unbounded_Queue<T>::set");

  ACE_Node<T> *curr = this->head_->next_;

  size_t i;

  for (i = 0;
       i < index && i < this->cur_size_;
       i++)
    curr = curr->next_;

  if (i < this->cur_size_)
    {
      // We're in range, so everything's cool.
      curr->item_ = item;
      return 0;
    }
  else
    {
      // We need to expand the list.

      // A common case will be increasing the set size by 1.
      // Therefore, we'll optimize for this case.
      if (i == index)
        {
          // Try to expand the size of the set by 1.
          if (this->enqueue_tail (item) == -1)
            return -1;
          else
            return 0;
        }
      else
        {
          T dummy;

          // We need to expand the list by multiple (dummy) items.
          for (; i < index; i++)
            {
              // This head points to the existing dummy node, which is
              // about to be overwritten when we add the new dummy
              // node.
              curr = this->head_;

              // Try to expand the size of the set by 1, but don't
              // store anything in the dummy node (yet).
              if (this->enqueue_tail (dummy) == -1)
                return -1;
            }

          curr->item_ = item;
          return 0;
        }
    }
}

template <class T> void
ACE_Unbounded_Queue_Iterator<T>::dump (void) const
{
// ACE_TRACE ("ACE_Unbounded_Queue_Iterator<T>::dump");
}

template <class T>
ACE_Unbounded_Queue_Iterator<T>::ACE_Unbounded_Queue_Iterator (ACE_Unbounded_Queue<T> &q, int end)
    : current_ (end == 0 ? q.head_->next_ : q.head_ ),
      queue_ (q)
{
// ACE_TRACE ("ACE_Unbounded_Queue_Iterator<T>::ACE_Unbounded_Queue_Iterator");
}

template <class T> int
ACE_Unbounded_Queue_Iterator<T>::advance (void)
{
// ACE_TRACE ("ACE_Unbounded_Queue_Iterator<T>::advance");
  this->current_ = this->current_->next_;
  return this->current_ != this->queue_.head_;
}

template <class T> int
ACE_Unbounded_Queue_Iterator<T>::first (void)
{
// ACE_TRACE ("ACE_Unbounded_Queue_Iterator<T>::first");
  this->current_ = this->queue_.head_->next_;
  return this->current_ != this->queue_.head_;
}

template <class T> int
ACE_Unbounded_Queue_Iterator<T>::done (void) const
{
  ACE_TRACE ("ACE_Unbounded_Queue_Iterator<T>::done");

  return this->current_ == this->queue_.head_;
}

template <class T> int
ACE_Unbounded_Queue_Iterator<T>::next (T *&item)
{
// ACE_TRACE ("ACE_Unbounded_Queue_Iterator<T>::next");
  if (this->current_ == this->queue_.head_)
    return 0;
  else
    {
      item = &this->current_->item_;
      return 1;
    }
}

//--------------------------------------------------
ACE_ALLOC_HOOK_DEFINE(ACE_Double_Linked_List_Iterator)

template <class T>
ACE_Double_Linked_List_Iterator<T>::ACE_Double_Linked_List_Iterator (ACE_Double_Linked_List<T> &dll)
  : dllist_ (dll)
{
  this->current_ = ACE_static_cast (T*, dll.head_->next_);  // Initialize head ptr.
}

template <class T> T *
ACE_Double_Linked_List_Iterator<T>::not_done (void) const
{
  if (this->current_ != this->dllist_.head_)
    return this->current_;
  else
    return 0;
}

template <class T> T *
ACE_Double_Linked_List_Iterator<T>::do_advance (void)
{
  if (this->not_done ())
    {
      this->current_ = ACE_static_cast (T*, this->current_->next_);
      return this->not_done ();
    }
  else
    return 0;
}

template <class T> T *
ACE_Double_Linked_List_Iterator<T>::next (void) const
{
  return this->not_done ();
}

template <class T> int
ACE_Double_Linked_List_Iterator<T>::advance (void)
{
  return this->do_advance () ? 1 : 0;
}

template <class T> int
ACE_Double_Linked_List_Iterator<T>::first (void)
{
  this->current_ = ACE_static_cast (T*, dllist_.head_->next_);
  return this->not_done () ? 1 : 0;
}

template <class T> int
ACE_Double_Linked_List_Iterator<T>::done (void) const
{
  return this->not_done () ? 0 : 1;
}

template <class T> void
ACE_Double_Linked_List_Iterator<T>::dump (void) const
{
  // Dump the state of an object.
}

ACE_ALLOC_HOOK_DEFINE(ACE_Double_Linked_List)

template <class T>
ACE_Double_Linked_List<T>:: ACE_Double_Linked_List (ACE_Allocator *alloc)
  : size_ (0), allocator_ (alloc)
{
  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  ACE_NEW_MALLOC (this->head_,
                  (T *) this->allocator_->malloc (sizeof (T)),
                  T);

  this->init_head ();
}

template <class T>
ACE_Double_Linked_List<T>::ACE_Double_Linked_List (ACE_Double_Linked_List<T> &cx)
  : allocator_ (cx.allocator_)
{
  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  ACE_NEW_MALLOC (this->head_,
                  (T *) this->allocator_->malloc (sizeof (T)),
                  T);
  this->init_head ();
  this->copy_nodes (cx);
}

template <class T> void
ACE_Double_Linked_List<T>::operator= (ACE_Double_Linked_List<T> &cx)
{
  if (this != &cx)
    {
      this->delete_nodes ();
      this->copy_nodes (cx);
    }
}

template <class T>
ACE_Double_Linked_List<T>::~ACE_Double_Linked_List (void)
{
  this->delete_nodes ();

  ACE_DES_FREE (head_,
                this->allocator_->free,
                T);

  this->head_ = 0;
}

template <class T> int
ACE_Double_Linked_List<T>::is_empty (void) const
{
  return this->size () ? 0 : 1;
}

template <class T> int
ACE_Double_Linked_List<T>::is_full (void) const
{
  return 0;                     // We have no bound.
}

template <class T> T *
ACE_Double_Linked_List<T>::insert_tail (T *new_item)
{
  // Insert it before <head_>, i.e., at tail.
  this->insert_element (new_item, 1);
  return new_item;
}

template <class T> T *
ACE_Double_Linked_List<T>::insert_head (T *new_item)
{
  this->insert_element (new_item); // Insert it after <head_>, i.e., at head.
  return new_item;
}

template <class T> T *
ACE_Double_Linked_List<T>::delete_head (void)
{
  T *temp;

  if (this->is_empty ())
    return 0;

  temp = ACE_static_cast (T *, this->head_->next_);
  this->remove_element (temp);  // Detach it from the list.
  return temp;
}

template <class T> T *
ACE_Double_Linked_List<T>::delete_tail (void)
{
  T *temp;

  if (this->is_empty ())
    return 0;

  temp = ACE_static_cast (T*, this->head_->prev_);
  this->remove_element (temp);  // Detach it from the list.
  return temp;
}

template <class T> void
ACE_Double_Linked_List<T>::reset (void)
{
  this->delete_nodes ();
}

template <class T> int
ACE_Double_Linked_List<T>::get (T *&item, size_t index)
{
  ACE_Double_Linked_List_Iterator<T> iter (*this);

  for (size_t i = 0;
       i < index && !iter.done ();
       i++)
    iter.advance ();

  item = iter.next ();
  return item ? 0 : 1;
}

template <class T> size_t
ACE_Double_Linked_List<T>::size (void) const
{
  return this->size_;
}

template <class T> void
ACE_Double_Linked_List<T>::dump (void) const
{
  // Dump the state of an object.
}

#if 0
template <class T> T *
ACE_Double_Linked_List<T>::find (const T &item)
{
  for (ACE_Double_Linked_List_Iterator<T> iter (*this);
       !iter.done ();
       iter.advance ())
    {
      T *temp = iter.next ();

      if (*temp == item)
        return temp;
    }

  return 0;
}

template <class T> int
ACE_Double_Linked_List<T>::remove (const T &item)
{
  T *temp = this->find (item);

  if (temp != 0)
      return this->remove (temp);
  else
    return -1;
}
#endif /* 0 */

template <class T> int
ACE_Double_Linked_List<T>::remove (T *n)
{
  return this->remove_element (n);
}

template <class T> void
ACE_Double_Linked_List<T>::delete_nodes (void)
{
  while (! this->is_empty ())
    {
      T * temp = ACE_static_cast (T*, this->head_->next_);
      this->remove_element (temp);
      delete temp;
    }
}

template <class T> void
ACE_Double_Linked_List<T>::copy_nodes (ACE_Double_Linked_List<T> &c)
{
  for (ACE_Double_Linked_List_Iterator<T> iter (c);
       !iter.done ();
       iter.advance ())
    this->insert_head (new T (*iter.next ()));
}

template <class T> void
ACE_Double_Linked_List<T>::init_head (void)
{
  this->head_->next_ = this->head_->prev_ = this->head_;
}

template <class T> int
ACE_Double_Linked_List<T>::insert_element (T *new_item,
                                           int before,
                                           T *old_item)
{
  if (old_item == 0)
    old_item = this->head_;

  if (before)
    old_item = ACE_static_cast (T*, old_item->prev_);

  new_item->next_ = old_item->next_;
  new_item->next_->prev_ = new_item;
  new_item->prev_ = old_item;
  old_item->next_ = new_item;
  this->size_++;
  return 0;                     // Well, what will cause errors here?
}

template <class T> int
ACE_Double_Linked_List<T>::remove_element (T *item)
{
  // Notice that you have to ensure that item is an element of this
  // list.  We can't do much checking here.

  if (item == this->head_ || item->next_ == 0
      || item->prev_ == 0 || this->size () == 0)      // Can't remove head
    return -1;

  item->prev_->next_ = item->next_;
  item->next_->prev_ = item->prev_;
  item->next_ = item->prev_ = 0; // reset pointers to prevent double removal.
  this->size_--;
  return 0;
}

//--------------------------------------------------

ACE_ALLOC_HOOK_DEFINE(ACE_Fixed_Set)

template <class T, size_t ACE_SIZE> size_t
ACE_Fixed_Set<T, ACE_SIZE>::size (void) const
{
  return this->cur_size_;
}

template <class T> size_t
ACE_Bounded_Set<T>::size (void) const
{
  ACE_TRACE ("ACE_Bounded_Set<T>::size");
  return this->cur_size_;
}

template <class T, size_t ACE_SIZE> void
ACE_Fixed_Set<T, ACE_SIZE>::dump (void) const
{
  ACE_TRACE ("ACE_Fixed_Set<T, ACE_SIZE>::dump");
}

template <class T, size_t ACE_SIZE>
ACE_Fixed_Set<T, ACE_SIZE>::~ACE_Fixed_Set (void)
{
  ACE_TRACE ("ACE_Fixed_Set<T, ACE_SIZE>::~ACE_Fixed_Set");
  this->cur_size_ = 0;
}

template <class T, size_t ACE_SIZE>
ACE_Fixed_Set<T, ACE_SIZE>::ACE_Fixed_Set (const ACE_Fixed_Set<T, ACE_SIZE> &fs)
  : cur_size_ (fs.cur_size_)
{
  ACE_TRACE ("ACE_Fixed_Set<T>::ACE_Fixed_Set");

  for (size_t i = 0; i < this->cur_size_; i++)
    this->search_structure_[i] = fs.search_structure_[i];
}

template <class T, size_t ACE_SIZE> void
ACE_Fixed_Set<T, ACE_SIZE>::operator= (const ACE_Fixed_Set<T, ACE_SIZE> &fs)
{
  ACE_TRACE ("ACE_Fixed_Set<T>::operator=");

  if (this != &fs)
    {
      this->cur_size_ = fs.cur_size_;

      for (size_t i = 0; i < this->cur_size_; i++)
        this->search_structure_[i] = fs.search_structure_[i];
    }
}

template <class T, size_t ACE_SIZE>
ACE_Fixed_Set<T, ACE_SIZE>::ACE_Fixed_Set (void)
  : cur_size_ (0),
    max_size_ (ACE_SIZE)
{
  ACE_TRACE ("ACE_Fixed_Set<T, ACE_SIZE>::ACE_Fixed_Set");
  for (size_t i = 0; i < this->max_size_; i++)
    this->search_structure_[i].is_free_ = 1;
}

template <class T, size_t ACE_SIZE> int
ACE_Fixed_Set<T, ACE_SIZE>::find (const T &item) const
{
  ACE_TRACE ("ACE_Fixed_Set<T, ACE_SIZE>::find");

  for (size_t i = 0; i < this->cur_size_; i++)
    if (this->search_structure_[i].item_ == item
        && this->search_structure_[i].is_free_ == 0)
      return 0;

  return -1;
}

template <class T, size_t ACE_SIZE> int
ACE_Fixed_Set<T, ACE_SIZE>::insert (const T &item)
{
  ACE_TRACE ("ACE_Fixed_Set<T, ACE_SIZE>::insert");
  int first_free = -1;   // Keep track of first free slot.
  size_t i;

  for (i = 0; i < this->cur_size_; i++)
    // First, make sure we don't allow duplicates.

    if (this->search_structure_[i].item_ == item
        && this->search_structure_[i].is_free_ == 0)
      return 1;
    else if (this->search_structure_[i].is_free_
             && first_free == -1)
      first_free = i;

  // If we found a free spot let's reuse it.
  if (first_free > -1)
    {
      this->search_structure_[first_free].item_ = item;
      this->search_structure_[first_free].is_free_ = 0;
      return 0;
    }
    // Insert at the end of the active portion.
  else if (i < this->max_size_)
    {
      this->search_structure_[i].item_ = item;
      this->search_structure_[i].is_free_ = 0;
      this->cur_size_++;
      return 0;
    }
  else /* No more room! */
    {
      errno = ENOMEM;
      return -1;
    }
}

template <class T, size_t ACE_SIZE> int
ACE_Fixed_Set<T, ACE_SIZE>::remove (const T &item)
{
  ACE_TRACE ("ACE_Fixed_Set<T, ACE_SIZE>::remove");

  for (size_t i = 0; i < this->cur_size_; i++)
    if (this->search_structure_[i].item_ == item)
      {
        // Mark this entry as being free.
        this->search_structure_[i].is_free_ = 1;

        // If we just unbound the highest entry, then we need to
        // figure out where the next highest active entry is.
        if (i + 1 == this->cur_size_)
          {
            while (i > 0
                   && this->search_structure_[--i].is_free_)
              continue;

            if (i == 0
                && this->search_structure_[i].is_free_)
              this->cur_size_ = 0;
            else
              this->cur_size_ = i + 1;
          }
        return 0;
      }

  return -1;
}

ACE_ALLOC_HOOK_DEFINE(ACE_Fixed_Set_Iterator)

template <class T, size_t ACE_SIZE> void
ACE_Fixed_Set_Iterator<T, ACE_SIZE>::dump (void) const
{
  ACE_TRACE ("ACE_Fixed_Set_Iterator<T, ACE_SIZE>::dump");
}

template <class T, size_t ACE_SIZE>
ACE_Fixed_Set_Iterator<T, ACE_SIZE>::ACE_Fixed_Set_Iterator (ACE_Fixed_Set<T, ACE_SIZE> &s)
  : s_ (s),
    next_ (-1)
{
  ACE_TRACE ("ACE_Fixed_Set_Iterator<T, ACE_SIZE>::ACE_Fixed_Set_Iterator");
  this->advance ();
}

template <class T, size_t ACE_SIZE> int
ACE_Fixed_Set_Iterator<T, ACE_SIZE>::advance (void)
{
  ACE_TRACE ("ACE_Fixed_Set_Iterator<T, ACE_SIZE>::advance");

  for (++this->next_;
       ACE_static_cast(size_t, this->next_) < this->s_.cur_size_
       && this->s_.search_structure_[this->next_].is_free_;
       ++this->next_)
    continue;

  return ACE_static_cast(size_t, this->next_) < this->s_.cur_size_;
}

template <class T, size_t ACE_SIZE> int
ACE_Fixed_Set_Iterator<T, ACE_SIZE>::first (void)
{
  ACE_TRACE ("ACE_Fixed_Set_Iterator<T, ACE_SIZE>::first");

  next_ = -1;
  return this->advance ();
}

template <class T, size_t ACE_SIZE> int
ACE_Fixed_Set_Iterator<T, ACE_SIZE>::done (void) const
{
  ACE_TRACE ("ACE_Fixed_Set_Iterator<T, ACE_SIZE>::done");

  return ACE_static_cast (ACE_CAST_CONST size_t, this->next_) >=
    this->s_.cur_size_;
}

template <class T, size_t ACE_SIZE> int
ACE_Fixed_Set_Iterator<T, ACE_SIZE>::next (T *&item)
{
  ACE_TRACE ("ACE_Fixed_Set_Iterator<T, ACE_SIZE>::next");
  if (ACE_static_cast (size_t, this->next_) < this->s_.cur_size_)
    {
      item = &this->s_.search_structure_[this->next_].item_;
      return 1;
    }
  else
    return 0;
}

ACE_ALLOC_HOOK_DEFINE(ACE_Bounded_Set)

template <class T> void
ACE_Bounded_Set<T>::dump (void) const
{
  ACE_TRACE ("ACE_Bounded_Set<T>::dump");
}

template <class T>
ACE_Bounded_Set<T>::~ACE_Bounded_Set (void)
{
  ACE_TRACE ("ACE_Bounded_Set<T>::~ACE_Bounded_Set");
  delete [] this->search_structure_;
}

template <class T>
ACE_Bounded_Set<T>::ACE_Bounded_Set (void)
  : cur_size_ (0),
    max_size_ (ACE_static_cast(size_t, ACE_Bounded_Set<T>::DEFAULT_SIZE))
{
  ACE_TRACE ("ACE_Bounded_Set<T>::ACE_Bounded_Set");

  ACE_NEW (this->search_structure_,
           ACE_Bounded_Set<T>::Search_Structure[this->max_size_]);

  for (size_t i = 0; i < this->max_size_; i++)
    this->search_structure_[i].is_free_ = 1;
}

template <class T>
ACE_Bounded_Set<T>::ACE_Bounded_Set (const ACE_Bounded_Set<T> &bs)
  : cur_size_ (bs.cur_size_),
    max_size_ (bs.max_size_)
{
  ACE_TRACE ("ACE_Bounded_Set<T>::ACE_Bounded_Set");

  ACE_NEW (this->search_structure_,
           ACE_Bounded_Set<T>::Search_Structure[this->max_size_]);

  for (size_t i = 0; i < this->cur_size_; i++)
    this->search_structure_[i] = bs.search_structure_[i];
}

template <class T> void
ACE_Bounded_Set<T>::operator= (const ACE_Bounded_Set<T> &bs)
{
  ACE_TRACE ("ACE_Bounded_Set<T>::operator=");

  if (this != &bs)
    {
      if (this->max_size_ < bs.cur_size_)
        {
          delete [] this->search_structure_;
          ACE_NEW (this->search_structure_,
                   ACE_Bounded_Set<T>::Search_Structure[bs.cur_size_]);
          this->max_size_ = bs.cur_size_;
        }

      this->cur_size_ = bs.cur_size_;

      for (size_t i = 0; i < this->cur_size_; i++)
        this->search_structure_[i] = bs.search_structure_[i];
    }
}

template <class T>
ACE_Bounded_Set<T>::ACE_Bounded_Set (size_t size)
  : cur_size_ (0),
    max_size_ (size)
{
  ACE_TRACE ("ACE_Bounded_Set<T>::ACE_Bounded_Set");
  ACE_NEW (this->search_structure_,
           ACE_Bounded_Set<T>::Search_Structure[size]);

  for (size_t i = 0; i < this->max_size_; i++)
    this->search_structure_[i].is_free_ = 1;
}

template <class T> int
ACE_Bounded_Set<T>::find (const T &item) const
{
  ACE_TRACE ("ACE_Bounded_Set<T>::find");

  for (size_t i = 0; i < this->cur_size_; i++)
    if (this->search_structure_[i].item_ == item
        && this->search_structure_[i].is_free_ == 0)
      return 0;

  return -1;
}

template <class T> int
ACE_Bounded_Set<T>::insert (const T &item)
{
  ACE_TRACE ("ACE_Bounded_Set<T>::insert");
  int first_free = -1;   // Keep track of first free slot.
  size_t i;

  for (i = 0; i < this->cur_size_; i++)
    // First, make sure we don't allow duplicates.

    if (this->search_structure_[i].item_ == item
        && this->search_structure_[i].is_free_ == 0)
      return 1;
    else if (this->search_structure_[i].is_free_ && first_free == -1)
      first_free = i;

  if (first_free > -1)   // If we found a free spot let's reuse it.
    {
      this->search_structure_[first_free].item_ = item;
      this->search_structure_[first_free].is_free_ = 0;
      return 0;
    }
  else if (i < this->max_size_) // Insert at the end of the active portion.
    {
      this->search_structure_[i].item_ = item;
      this->search_structure_[i].is_free_ = 0;
      this->cur_size_++;
      return 0;
    }
  else /* No more room! */
    {
      errno = ENOMEM;
      return -1;
    }
}

template <class T> int
ACE_Bounded_Set<T>::remove (const T &item)
{
  ACE_TRACE ("ACE_Bounded_Set<T>::remove");
  for (size_t i = 0; i < this->cur_size_; i++)
    if (this->search_structure_[i].item_ == item)
      {
        // Mark this entry as being free.
        this->search_structure_[i].is_free_ = 1;

        // If we just unbound the highest entry, then we need to
        // figure out where the next highest active entry is.
        if (i + 1 == this->cur_size_)
          {
            while (i > 0 && this->search_structure_[--i].is_free_)
              continue;

            if (i == 0 && this->search_structure_[i].is_free_)
              this->cur_size_ = 0;
            else
              this->cur_size_ = i + 1;
          }
        return 0;
      }

  return -1;
}

#if defined (__Lynx__)
 // LynxOS 3.0.0 native g++ compiler raises internal error with this inline.
template <class T> int
ACE_Bounded_Set<T>::is_full (void) const
{
  ACE_TRACE ("ACE_Bounded_Set<T>::is_full");
  return this->cur_size_ == this->max_size_;
}
#endif /* __Lynx__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Bounded_Set_Iterator)

template <class T> void
ACE_Bounded_Set_Iterator<T>::dump (void) const
{
  ACE_TRACE ("ACE_Bounded_Set_Iterator<T>::dump");
}

template <class T>
ACE_Bounded_Set_Iterator<T>::ACE_Bounded_Set_Iterator (ACE_Bounded_Set<T> &s)
  : s_ (s),
    next_ (-1)
{
  ACE_TRACE ("ACE_Bounded_Set_Iterator<T>::ACE_Bounded_Set_Iterator");
  this->advance ();
}

template <class T> int
ACE_Bounded_Set_Iterator<T>::advance (void)
{
  ACE_TRACE ("ACE_Bounded_Set_Iterator<T>::advance");

  for (++this->next_;
       ACE_static_cast(size_t, this->next_) < this->s_.cur_size_
       && this->s_.search_structure_[this->next_].is_free_;
       ++this->next_)
    continue;

  return ACE_static_cast(size_t, this->next_) < this->s_.cur_size_;
}

template <class T> T*
ACE_Double_Linked_List_Iterator<T>::advance_and_remove (int dont_remove)
{
  T* item = 0;
  if (dont_remove)
      this->do_advance ();
  else
    {
      item = this->next ();
      this->do_advance ();
      this->dllist_.remove_element (item);
    }
  return item;
}

template <class T> int
ACE_Bounded_Set_Iterator<T>::first (void)
{
  ACE_TRACE ("ACE_Bounded_Set_Iterator<T>::first");

  next_ = -1;
  return this->advance ();
}

template <class T> int
ACE_Bounded_Set_Iterator<T>::done (void) const
{
  ACE_TRACE ("ACE_Bounded_Set_Iterator<T>::done");

  return ACE_static_cast (ACE_CAST_CONST size_t, this->next_) >=
    this->s_.cur_size_;
}

template <class T> int
ACE_Bounded_Set_Iterator<T>::next (T *&item)
{
  ACE_TRACE ("ACE_Bounded_Set_Iterator<T>::next");
  if (ACE_static_cast(size_t, this->next_) < this->s_.cur_size_)
    {
      item = &this->s_.search_structure_[this->next_].item_;
      return 1;
    }
  else
    return 0;
}

ACE_ALLOC_HOOK_DEFINE(ACE_Node)

# if ! defined (ACE_HAS_BROKEN_NOOP_DTORS)
template <class T>
ACE_Node<T>::~ACE_Node (void)
{
}
# endif /* ! defined (ACE_HAS_BROKEN_NOOP_DTORS) */

template <class T>
ACE_Node<T>::ACE_Node (const T &i, ACE_Node<T> *n)
  : next_ (n),
    item_ (i)
{
// ACE_TRACE ("ACE_Node<T>::ACE_Node");
}

template <class T>
ACE_Node<T>::ACE_Node (ACE_Node<T> *n, int)
  : next_ (n)
{
// ACE_TRACE ("ACE_Node<T>::ACE_Node");
}

template <class T>
ACE_Node<T>::ACE_Node (const ACE_Node<T> &s)
  : next_ (s.next_),
    item_ (s.item_)
{
// ACE_TRACE ("ACE_Node<T>::ACE_Node");
}

ACE_ALLOC_HOOK_DEFINE(ACE_DNode)

template <class T>
ACE_DNode<T>::ACE_DNode (const T &i, ACE_DNode<T> *n, ACE_DNode<T> *p)
  : next_ (n), prev_ (p), item_ (i)
{
}

# if ! defined (ACE_HAS_BROKEN_NOOP_DTORS)
template <class T>
ACE_DNode<T>::~ACE_DNode (void)
{
}
# endif /* ! defined (ACE_HAS_BROKEN_NOOP_DTORS) */

ACE_ALLOC_HOOK_DEFINE(ACE_Unbounded_Set)

template <class T> size_t
ACE_Unbounded_Set<T>::size (void) const
{
// ACE_TRACE ("ACE_Unbounded_Set<T>::size");
  return this->cur_size_;
}

template <class T> int
ACE_Unbounded_Set<T>::insert_tail (const T &item)
{
// ACE_TRACE ("ACE_Unbounded_Queue<T>::insert_tail");
  ACE_Node<T> *temp;

  // Insert <item> into the old dummy node location.
  this->head_->item_ = item;

  // Create a new dummy node.
  ACE_NEW_MALLOC_RETURN (temp,
                         (ACE_Node<T>*) this->allocator_->malloc (sizeof (ACE_Node<T>)),
                         ACE_Node<T> (this->head_->next_),
                         -1);

  // Link this pointer into the list.
  this->head_->next_ = temp;

  // Point the head to the new dummy node.
  this->head_ = temp;

  this->cur_size_++;
  return 0;
}

template <class T> void
ACE_Unbounded_Set<T>::reset (void)
{
  ACE_TRACE ("reset");

  this->delete_nodes ();
}

template <class T> void
ACE_Unbounded_Set<T>::dump (void) const
{
  ACE_TRACE ("ACE_Unbounded_Set<T>::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("\nhead_ = %u"), this->head_));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("\nhead_->next_ = %u"), this->head_->next_));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("\ncur_size_ = %d\n"), this->cur_size_));

  T *item = 0;
#if !defined (ACE_NLOGGING)
  size_t count = 1;
#endif /* ! ACE_NLOGGING */

  for (ACE_Unbounded_Set_Iterator<T> iter (*(ACE_Unbounded_Set<T> *) this);
       iter.next (item) != 0;
       iter.advance ())
    ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("count = %d\n"), count++));

  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class T> void
ACE_Unbounded_Set<T>::copy_nodes (const ACE_Unbounded_Set<T> &us)
{
  for (ACE_Node<T> *curr = us.head_->next_;
       curr != us.head_;
       curr = curr->next_)
    this->insert_tail (curr->item_);
}

template <class T> void
ACE_Unbounded_Set<T>::delete_nodes (void)
{
  ACE_Node<T> *curr = this->head_->next_;

  // Keep looking until we've hit the dummy node.

  while (curr != this->head_)
    {
      ACE_Node<T> *temp = curr;
      curr = curr->next_;
      ACE_DES_FREE_TEMPLATE (temp,
                             this->allocator_->free,
                             ACE_Node,
                             <T>);
      this->cur_size_--;
    }

  // Reset the list to be a circular list with just a dummy node.
  this->head_->next_ = this->head_;
}

template <class T>
ACE_Unbounded_Set<T>::~ACE_Unbounded_Set (void)
{
// ACE_TRACE ("ACE_Unbounded_Set<T>::~ACE_Unbounded_Set");

  this->delete_nodes ();

  // Delete the dummy node.
  ACE_DES_FREE_TEMPLATE (head_,
                         this->allocator_->free,
                         ACE_Node,
                         <T>);
  this->head_ = 0;
}

template <class T>
ACE_Unbounded_Set<T>::ACE_Unbounded_Set (ACE_Allocator *alloc)
  : head_ (0),
    cur_size_ (0),
    allocator_ (alloc)
{
// ACE_TRACE ("ACE_Unbounded_Set<T>::ACE_Unbounded_Set");

  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  ACE_NEW_MALLOC (this->head_,
                  (ACE_Node<T>*) this->allocator_->malloc (sizeof (ACE_Node<T>)),
                  ACE_Node<T>);

  // Make the list circular by pointing it back to itself.
  this->head_->next_ = this->head_;
}

template <class T>
ACE_Unbounded_Set<T>::ACE_Unbounded_Set (const ACE_Unbounded_Set<T> &us)
  : head_ (0),
    cur_size_ (0),
    allocator_ (us.allocator_)
{
  ACE_TRACE ("ACE_Unbounded_Set<T>::ACE_Unbounded_Set");

  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  ACE_NEW_MALLOC (this->head_,
                  (ACE_Node<T>*) this->allocator_->malloc (sizeof (ACE_Node<T>)),
                  ACE_Node<T>);
  this->head_->next_ = this->head_;
  this->copy_nodes (us);
}

template <class T> void
ACE_Unbounded_Set<T>::operator= (const ACE_Unbounded_Set<T> &us)
{
  ACE_TRACE ("ACE_Unbounded_Set<T>::operator=");

  if (this != &us)
    {
      this->delete_nodes ();
      this->copy_nodes (us);
    }
}

template <class T> int
ACE_Unbounded_Set<T>::find (const T &item) const
{
// ACE_TRACE ("ACE_Unbounded_Stack<T>::find");
  // Set <item> into the dummy node.
  this->head_->item_ = item;

  ACE_Node<T> *temp = this->head_->next_;

  // Keep looping until we find the item.
  while (!(temp->item_ == item))
    temp = temp->next_;

  // If we found the dummy node then it's not really there, otherwise,
  // it is there.
  return temp == this->head_ ? -1 : 0;
}

template <class T> int
ACE_Unbounded_Set<T>::insert (const T &item)
{
// ACE_TRACE ("ACE_Unbounded_Set<T>::insert");
  if (this->find (item) == 0)
    return 1;
  else
    return this->insert_tail (item);
}

template <class T> int
ACE_Unbounded_Set<T>::remove (const T &item)
{
// ACE_TRACE ("ACE_Unbounded_Set<T>::remove");

  // Insert the item to be founded into the dummy node.
  this->head_->item_ = item;

  ACE_Node<T> *curr = this->head_;

  while (!(curr->next_->item_ == item))
    curr = curr->next_;

  if (curr->next_ == this->head_)
    return -1; // Item was not found.
  else
    {
      ACE_Node<T> *temp = curr->next_;
      // Skip over the node that we're deleting.
      curr->next_ = temp->next_;
      this->cur_size_--;
      ACE_DES_FREE_TEMPLATE (temp,
                             this->allocator_->free,
                             ACE_Node,
                             <T>);
      return 0;
    }
}

template <class T> ACE_Unbounded_Set_Iterator<T>
ACE_Unbounded_Set<T>::begin (void)
{
  // ACE_TRACE ("ACE_Unbounded_Set<T>::begin");
  return ACE_Unbounded_Set_Iterator<T> (*this);
}

template <class T> ACE_Unbounded_Set_Iterator<T>
ACE_Unbounded_Set<T>::end (void)
{
  // ACE_TRACE ("ACE_Unbounded_Set<T>::end");
  return ACE_Unbounded_Set_Iterator<T> (*this, 1);
}


ACE_ALLOC_HOOK_DEFINE(ACE_Unbounded_Set_Iterator)

template <class T> void
ACE_Unbounded_Set_Iterator<T>::dump (void) const
{
// ACE_TRACE ("ACE_Unbounded_Set_Iterator<T>::dump");
}

template <class T>
ACE_Unbounded_Set_Iterator<T>::ACE_Unbounded_Set_Iterator (ACE_Unbounded_Set<T> &s, int end)
    : current_ (end == 0 ? s.head_->next_ : s.head_ ),
      set_ (&s)
{
// ACE_TRACE ("ACE_Unbounded_Set_Iterator<T>::ACE_Unbounded_Set_Iterator");
}

template <class T> int
ACE_Unbounded_Set_Iterator<T>::advance (void)
{
// ACE_TRACE ("ACE_Unbounded_Set_Iterator<T>::advance");
  this->current_ = this->current_->next_;
  return this->current_ != this->set_->head_;
}

template <class T> int
ACE_Unbounded_Set_Iterator<T>::first (void)
{
// ACE_TRACE ("ACE_Unbounded_Set_Iterator<T>::first");
  this->current_ = this->set_->head_->next_;
  return this->current_ != this->set_->head_;
}

template <class T> int
ACE_Unbounded_Set_Iterator<T>::done (void) const
{
  ACE_TRACE ("ACE_Unbounded_Set_Iterator<T>::done");

  return this->current_ == this->set_->head_;
}

template <class T> int
ACE_Unbounded_Set_Iterator<T>::next (T *&item)
{
// ACE_TRACE ("ACE_Unbounded_Set_Iterator<T>::next");
  if (this->current_ == this->set_->head_)
    return 0;
  else
    {
      item = &this->current_->item_;
      return 1;
    }
}

template <class T> ACE_Unbounded_Set_Iterator<T>
ACE_Unbounded_Set_Iterator<T>::operator++ (int)
{
  //ACE_TRACE ("ACE_Unbounded_Set_Iterator<T>::operator++ (int)");
  ACE_Unbounded_Set_Iterator<T> retv (*this);

  // postfix operator

  this->advance ();
  return retv;
}

template <class T> ACE_Unbounded_Set_Iterator<T>&
ACE_Unbounded_Set_Iterator<T>::operator++ (void)
{
  // ACE_TRACE ("ACE_Unbounded_Set_Iterator<T>::operator++ (void)");

  // prefix operator

  this->advance ();
  return *this;
}

template <class T> T&
ACE_Unbounded_Set_Iterator<T>::operator* (void)
{
  //ACE_TRACE ("ACE_Unbounded_Set_Iterator<T>::operator*");
  T *retv = 0;

  int result = this->next (retv);
  ACE_ASSERT (result != 0);
  ACE_UNUSED_ARG (result);

  return *retv;
}

template <class T> int
ACE_Unbounded_Set_Iterator<T>::operator== (const ACE_Unbounded_Set_Iterator<T> &rhs) const
{
  //ACE_TRACE ("ACE_Unbounded_Set_Iterator<T>::operator==");
  return (this->set_ == rhs.set_ && this->current_ == rhs.current_);
}

template <class T> int
ACE_Unbounded_Set_Iterator<T>::operator!= (const ACE_Unbounded_Set_Iterator<T> &rhs) const
{
  //ACE_TRACE ("ACE_Unbounded_Set_Iterator<T>::operator!=");
  return (this->set_ != rhs.set_ || this->current_ != rhs.current_);
}

template <class T> void
ACE_Unbounded_Stack_Iterator<T>::dump (void) const
{
// ACE_TRACE ("ACE_Unbounded_Stack_Iterator<T>::dump");
}

template <class T>
ACE_Unbounded_Stack_Iterator<T>::ACE_Unbounded_Stack_Iterator (ACE_Unbounded_Stack<T> &q)
    : current_ (q.head_->next_),
      stack_ (q)
{
// ACE_TRACE ("ACE_Unbounded_Stack_Iterator<T>::ACE_Unbounded_Stack_Iterator");
}

template <class T> int
ACE_Unbounded_Stack_Iterator<T>::advance (void)
{
// ACE_TRACE ("ACE_Unbounded_Stack_Iterator<T>::advance");
  this->current_ = this->current_->next_;
  return this->current_ != this->stack_.head_;
}

template <class T> int
ACE_Unbounded_Stack_Iterator<T>::first (void)
{
// ACE_TRACE ("ACE_Unbounded_Stack_Iterator<T>::first");
  this->current_ = this->stack_.head_->next_;
  return this->current_ != this->stack_.head_;
}

template <class T> int
ACE_Unbounded_Stack_Iterator<T>::done (void) const
{
  ACE_TRACE ("ACE_Unbounded_Stack_Iterator<T>::done");

  return this->current_ == this->stack_.head_;
}

template <class T> int
ACE_Unbounded_Stack_Iterator<T>::next (T *&item)
{
// ACE_TRACE ("ACE_Unbounded_Stack_Iterator<T>::next");
  if (this->current_ == this->stack_.head_)
    return 0;
  else
    {
      item = &this->current_->item_;
      return 1;
    }
}


ACE_ALLOC_HOOK_DEFINE(ACE_Ordered_MultiSet)


template <class T>
ACE_Ordered_MultiSet<T>::ACE_Ordered_MultiSet (ACE_Allocator *alloc)
  : head_ (0)
  , tail_ (0)
  , cur_size_ (0)
  , allocator_ (alloc)
{
// ACE_TRACE ("ACE_Ordered_MultiSet<T>::ACE_Ordered_MultiSet");

  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();
}

template <class T>
ACE_Ordered_MultiSet<T>::ACE_Ordered_MultiSet (const ACE_Ordered_MultiSet<T> &us)
  : head_ (0)
  , tail_ (0)
  , cur_size_ (0)
  , allocator_ (us.allocator_)
{
  ACE_TRACE ("ACE_Ordered_MultiSet<T>::ACE_Ordered_MultiSet");

  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  this->copy_nodes (us);
}

template <class T>
ACE_Ordered_MultiSet<T>::~ACE_Ordered_MultiSet (void)
{
// ACE_TRACE ("ACE_Ordered_MultiSet<T>::~ACE_Ordered_MultiSet");

  this->delete_nodes ();
}


template <class T> void
ACE_Ordered_MultiSet<T>::operator= (const ACE_Ordered_MultiSet<T> &us)
{
  ACE_TRACE ("ACE_Ordered_MultiSet<T>::operator=");

  if (this != &us)
    {
      this->delete_nodes ();
      this->copy_nodes (us);
    }
}


template <class T> int
ACE_Ordered_MultiSet<T>::insert (const T &item)
{
// ACE_TRACE ("ACE_Ordered_MultiSet<T>::insert");

  return  this->insert_from (item, this->head_, 0);
}

template <class T> int
ACE_Ordered_MultiSet<T>::insert (const T &item,
                                 ACE_Ordered_MultiSet_Iterator<T> &iter)
{
// ACE_TRACE ("ACE_Ordered_MultiSet<T>::insert using iterator");

  return  this->insert_from (item, iter.current_, &iter.current_);
}

template <class T> int
ACE_Ordered_MultiSet<T>::remove (const T &item)
{
// ACE_TRACE ("ACE_Ordered_MultiSet<T>::remove");

  ACE_DNode<T> *node = 0;

  int result = locate (item, 0, node);

  // if we found the node, remove from list and free it
  if (node && (result == 0))
  {
    if (node->prev_)
    {
      node->prev_->next_ = node->next_;
    }
    else
    {
      head_ = node->next_;
    }

    if (node->next_)
    {
      node->next_->prev_ = node->prev_;
    }
    else
    {
      tail_ = node->prev_;
    }

    this->cur_size_--;

    ACE_DES_FREE_TEMPLATE (node,
                           this->allocator_->free,
                           ACE_DNode,
                           <T>);
    return 0;
  }

  return -1;
}

template <class T> int
ACE_Ordered_MultiSet<T>::find (const T &item,
                               ACE_Ordered_MultiSet_Iterator<T> &iter) const
{
  // search an occurance of item, using iterator's current position as a hint
  ACE_DNode<T> *node = iter.current_;
  int result = locate (item, node, node);

  // if we found the node, update the iterator and indicate success
  if (node && (result == 0))
  {
    iter.current_ = node;
    return 0;
  }

  return -1;
}



template <class T> void
ACE_Ordered_MultiSet<T>::reset (void)
{
  ACE_TRACE ("reset");

  this->delete_nodes ();
}

template <class T> void
ACE_Ordered_MultiSet<T>::dump (void) const
{
//  ACE_TRACE ("ACE_Ordered_MultiSet<T>::dump");
//
//  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
//  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("\nhead_ = %u"), this->head_));
//  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("\nhead_->next_ = %u"), this->head_->next_));
//  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("\ncur_size_ = %d\n"), this->cur_size_));
//
//  T *item = 0;
//  size_t count = 1;
//
//  for (ACE_Ordered_MultiSet_Iterator<T> iter (*(ACE_Ordered_MultiSet<T> *) this);
//       iter.next (item) != 0;
//       iter.advance ())
//    ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("count = %d\n"), count++));
//
//  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class T> int
ACE_Ordered_MultiSet<T>::insert_from (const T &item, ACE_DNode<T> *position,
                                      ACE_DNode<T> **new_position)
{
// ACE_TRACE ("ACE_Unbounded_Queue<T>::insert_tail");

    // create a new node
  ACE_DNode<T> *temp;
  ACE_NEW_MALLOC_RETURN (temp,
                         (ACE_DNode<T>*) this->allocator_->malloc (sizeof (ACE_DNode<T>)),
                         ACE_DNode<T> (item),
                         -1);

  // obtain approximate location of the node
  int result = locate (item, position, position);

  // if there are nodes in the multiset
  if (position)
  {
    switch (result)
    {
      // insert after the approximate position
      case -1:

        // if there is a following node
        if (position->next_)
        {
          // link up with the following node
          position->next_->prev_ = temp;
          temp->next_ = position->next_;
        }
        else
        {
          // appending to the end of the set
          tail_ = temp;
        }

        // link up with the preceeding node
        temp->prev_ = position;
        position->next_ = temp;

        break;

      // insert before the position
      case  0:
      case  1:

        // if there is a preceeding node
        if (position->prev_)
        {
          // link up with the preceeding node
          position->prev_->next_ = temp;
          temp->prev_ = position->prev_;
        }
        else
        {
          // prepending to the start of the set
          head_ = temp;
        }

        // link up with the preceeding node
        temp->next_ = position;
        position->prev_ = temp;

        break;

      default:
        return -1;
    }
  }
  else
  {
    // point the head and tail to the new node.
    this->head_ = temp;
    this->tail_ = temp;
  }

  this->cur_size_++;
  if (new_position)
  {
    *new_position = temp;
  }

  return 0;
}

template <class T> int
ACE_Ordered_MultiSet<T>::locate (const T &item, ACE_DNode<T> *start_position,
                                 ACE_DNode<T> *&new_position) const
{
  if (! start_position)
  {
    start_position = this->head_;
  }

  // if starting before the item, move forward
  // until at or just before item
  while (start_position && start_position->item_ < item &&
         start_position->next_)
  {
    start_position = start_position->next_;
  }

  // if starting after the item, move back
  // until at or just after item
  while (start_position && item < start_position->item_ &&
         start_position->prev_)
  {
    start_position = start_position->prev_;
  }

  // save the (approximate) location in the passed pointer
  new_position = start_position;

  // show the location is after (1), before (-1) , or at (0) the item
  if (! new_position )
  {
    return 1;
  }
  else if (item < new_position->item_)
  {
    return 1;
  }
  else if (new_position->item_ < item)
  {
    return -1;
  }
  else
  {
    return 0;
  }
}
  // looks for first occurance of <item> in the ordered set, using the
  // passed starting position as a hint: if there is such an instance, it
  // updates the new_position pointer to point to one such node and returns 0;
  // if there is no such node, then if there is a node before where the
  // item would have been, it updates the new_position pointer to point
  // to this node and returns -1; if there is no such node, then if there
  // is a node after where the item would have been, it updates the
  // new_position pointer to point to this node (or 0 if there is no such
  // node) and returns 1;

template <class T> void
ACE_Ordered_MultiSet<T>::copy_nodes (const ACE_Ordered_MultiSet<T> &us)
{
  ACE_DNode<T> *insertion_point = this->head_;

  for (ACE_DNode<T> *curr = us.head_;
       curr;
       curr = curr->next_)
  {
    this->insert_from (curr->item_, insertion_point, &insertion_point);
  }
}

template <class T> void
ACE_Ordered_MultiSet<T>::delete_nodes (void)
{
  // iterate through list, deleting nodes
  for (ACE_DNode<T> *curr = this->head_;
       curr != 0;
       )
    {
      ACE_DNode<T> *temp = curr;
      curr = curr->next_;
      ACE_DES_FREE_TEMPLATE (temp,
                             this->allocator_->free,
                             ACE_DNode,
                             <T>);
    }

  this->head_ = 0;
  this->tail_ = 0;
  this->cur_size_ = 0;
}

ACE_ALLOC_HOOK_DEFINE(ACE_Ordered_MultiSet_Iterator)

template <class T>
ACE_Ordered_MultiSet_Iterator<T>::ACE_Ordered_MultiSet_Iterator (ACE_Ordered_MultiSet<T> &s)
    : current_ (s.head_),
      set_ (s)
{
// ACE_TRACE ("ACE_Ordered_MultiSet_Iterator<T>::ACE_Ordered_MultiSet_Iterator");
}

 template <class T> int
ACE_Ordered_MultiSet_Iterator<T>::next (T *&item) const
{
// ACE_TRACE ("ACE_Ordered_MultiSet_Iterator<T>::next");
  if (this->current_)
  {
    item = &this->current_->item_;
    return 1;
  }

  return 0;
}


ACE_ALLOC_HOOK_DEFINE (ACE_DLList_Node)

template <class T> T *
ACE_DLList<T>::insert_tail (T *new_item)
{
  ACE_DLList_Node *temp1, *temp2;
  ACE_NEW_MALLOC_RETURN (temp1,
                         (ACE_DLList_Node *) this->allocator_->malloc (sizeof (ACE_DLList_Node)),
                         ACE_DLList_Node ((void *&)new_item),
                         0);

  temp2 = ACE_DLList_Base::insert_tail (temp1);
  return (T *) (temp2 ? temp2->item_ : 0);
}

template <class T> T *
ACE_DLList<T>::insert_head (T *new_item)
{
  ACE_DLList_Node *temp1, *temp2;
  ACE_NEW_MALLOC_RETURN (
    temp1,
    (ACE_DLList_Node *) this->allocator_->malloc (sizeof (ACE_DLList_Node)),
    ACE_DLList_Node ((void *&)new_item), 0);

  temp2 = ACE_DLList_Base::insert_head (temp1);
  return (T *) (temp2 ? temp2->item_ : 0);
}

template <class T> T *
ACE_DLList<T>::delete_head (void)
{
  ACE_DLList_Node *temp1 = ACE_DLList_Base::delete_head ();
  T *temp2 = (T *) (temp1 ? temp1->item_ : 0);
  ACE_DES_FREE (temp1,
                this->allocator_->free,
                ACE_DLList_Node);

  return temp2;
}

template <class T> T *
ACE_DLList<T>::delete_tail (void)
{
  ACE_DLList_Node *temp1 = ACE_DLList_Base::delete_tail ();
  T *temp2 = (T *) (temp1 ? temp1->item_ : 0);
  ACE_DES_FREE (temp1,
                this->allocator_->free,
                ACE_DLList_Node);
  return temp2;
}

// ****************************************************************

// Dynamically initialize an array.

template <class T>
ACE_Array_Base<T>::ACE_Array_Base (size_t size,
                                   ACE_Allocator *alloc)
  : max_size_ (size),
    cur_size_ (size),
    allocator_ (alloc)
{
  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  if (size != 0)
    {
      ACE_NEW_MALLOC (this->array_,
                      (T *) this->allocator_->malloc (size * sizeof (T)),
                      T);
      for (size_t i = 0; i < size; ++i)
        new (&array_[i]) T;
    }
  else
    this->array_ = 0;
}

template <class T>
ACE_Array_Base<T>::ACE_Array_Base (size_t size,
                                   const T &default_value,
                                   ACE_Allocator *alloc)
  : max_size_ (size),
    cur_size_ (size),
    allocator_ (alloc)
{
  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  if (size != 0)
    {
      ACE_NEW_MALLOC (this->array_,
                      (T *) this->allocator_->malloc (size * sizeof (T)),
                      T);

      for (size_t i = 0; i < size; ++i)
        new (&array_[i]) T (default_value);
    }
  else
    this->array_ = 0;
}

// The copy constructor (performs initialization).

template <class T>
ACE_Array_Base<T>::ACE_Array_Base (const ACE_Array_Base<T> &s)
   : max_size_ (s.size ()),
     cur_size_ (s.size ()),
     allocator_ (s.allocator_)
{
  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  ACE_NEW_MALLOC (this->array_,
                  (T *) this->allocator_->malloc (s.size () * sizeof (T)),
                  T);

  for (size_t i = 0; i < this->size (); i++)
    new (&this->array_[i]) T (s.array_[i]);
}

// Assignment operator (performs assignment).

template <class T> void
ACE_Array_Base<T>::operator= (const ACE_Array_Base<T> &s)
{
  // Check for "self-assignment".

  if (this != &s)
    {
      if (this->max_size_ < s.size ())
        {
          ACE_DES_FREE (this->array_,
                        this->allocator_->free,
                        T);

          ACE_NEW_MALLOC (this->array_,
                          (T *) this->allocator_->malloc (s.size () * sizeof (T)),
                          T);
          this->max_size_ = s.size ();
        }

      this->cur_size_ = s.size ();

      for (size_t i = 0; i < this->size (); i++)
        new (&this->array_[i]) T (s.array_[i]);
    }
}

// Set an item in the array at location index.

template <class T> int
ACE_Array_Base<T>::set (const T &new_item, size_t index)
{
  if (this->in_range (index))
    {
      this->array_[index] = new_item;
      return 0;
    }
  else
    return -1;
}

// Get an item in the array at location index.

template <class T> int
ACE_Array_Base<T>::get (T &item, size_t index) const
{
   if (this->in_range (index))
     {
       // Copies the item.  If you don't want to copy, use operator []
       // instead (but then you'll be responsible for range checking).
       item = this->array_[index];
       return 0;
     }
   else
     return -1;
}

template<class T> int
ACE_Array_Base<T>::max_size (size_t new_size)
{
  if (new_size > this->max_size_)
    {
      T *tmp;

      ACE_NEW_MALLOC_RETURN (tmp,
                             (T *) this->allocator_->malloc (new_size * sizeof (T)),
                             T,
                             -1);
      for (size_t i = 0; i < cur_size_; ++i)
         new (&tmp[i]) T (this->array_[i]);

      ACE_DES_FREE (this->array_,
                    this->allocator_->free,
                    T);
      this->array_ = tmp;
      this->max_size_ = new_size;
    }

  return 0;
}

template<class T> int
ACE_Array_Base<T>::size (size_t new_size)
{
  int r = this->max_size (new_size);
  if (r != 0)
    return r;
  this->cur_size_ = new_size;
  return 0;
}

// ****************************************************************

// Compare this array with <s> for equality.

template <class T> int
ACE_Array<T>::operator== (const ACE_Array<T> &s) const
{
  if (this == &s)
    return 1;
  else if (this->size () != s.size ())
    return 0;

  for (size_t index = 0; index < s.size (); index++)
    if ((*this)[index] != s[index])
      return 0;

  return 1;
}

// ****************************************************************


template <class T> int
ACE_Array_Iterator<T>::next (T *&item)
{
  // ACE_TRACE ("ACE_Array_Iterator<T>::next");

  if (this->done ())
    {
      item = 0;
      return 0;
    }
  else
    {
      item = &array_[current_];
      return 1;
    }
}

#endif /* ACE_CONTAINERS_T_C */
