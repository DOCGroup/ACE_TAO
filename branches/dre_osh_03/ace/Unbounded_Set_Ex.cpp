// $Id$

#ifndef ACE_UNBOUNDED_SET_EX_C
#define ACE_UNBOUNDED_SET_EX_C

#include "ace/Unbounded_Set_Ex.h"
#include "ace/Malloc_Base.h"
#include "ace/Log_Msg.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (__ACE_INLINE__)
#include "ace/Unbounded_Set_Ex.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Unbounded_Set_Ex, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_Unbounded_Set_Ex)

template <class T> size_t
ACE_Unbounded_Set_Ex<T>::size (void) const
{
  // ACE_TRACE ("ACE_Unbounded_Set_Ex<T>::size");
  return this->cur_size_;
}

template <class T> int
ACE_Unbounded_Set_Ex<T>::insert_tail (const T &item)
{
  // ACE_TRACE ("ACE_Unbounded_Set_Ex<T>::insert_tail");
  ACE_Node<T> *temp;

  // Insert <item> into the old dummy node location.
  this->head_->item_ = item;

  // Create a new dummy node.
  ACE_NEW_MALLOC_RETURN (temp,
                         ACE_static_cast(ACE_Node<T>*,
                           this->allocator_->malloc (sizeof (ACE_Node<T>))),
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
ACE_Unbounded_Set_Ex<T>::reset (void)
{
  ACE_TRACE ("reset");

  this->delete_nodes ();
}

template <class T> void
ACE_Unbounded_Set_Ex<T>::dump (void) const
{
  ACE_TRACE ("ACE_Unbounded_Set_Ex<T>::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("\nhead_ = %u"), this->head_));
  ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("\nhead_->next_ = %u"), this->head_->next_));
  ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("\ncur_size_ = %d\n"), this->cur_size_));

  T *item = 0;
#if !defined (ACE_NLOGGING)
  size_t count = 1;
#endif /* ! ACE_NLOGGING */

  for (ACE_Unbounded_Set_Ex_Iterator<T> iter (*(ACE_Unbounded_Set_Ex<T> *) this);
       iter.next (item) != 0;
       iter.advance ())
    ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("count = %d\n"), count++));

  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class T> void
ACE_Unbounded_Set_Ex<T>::copy_nodes (const ACE_Unbounded_Set_Ex<T> &us)
{
  for (ACE_Node<T> *curr = us.head_->next_;
       curr != us.head_;
       curr = curr->next_)
    {
      if (!curr->deleted_)
        this->insert_tail (curr->item_);
    }
}

template <class T> void
ACE_Unbounded_Set_Ex<T>::delete_nodes (void)
{
  ACE_Node<T> *curr = this->head_->next_;
  ACE_ASSERT (number_of_iterators_ == 0);
  // Keep looking until we've hit the dummy node.

  while (curr != this->head_)
    {
      ACE_Node<T> *temp = curr;
      curr = curr->next_;
      if (!temp->deleted_)
        this->cur_size_--;
      ACE_DES_FREE_TEMPLATE (temp,
                             this->allocator_->free,
                             ACE_Node,
                             <T>);
    }

  // Reset the list to be a circular list with just a dummy node.
  this->head_->next_ = this->head_;
}

template <class T> void
ACE_Unbounded_Set_Ex<T>::cleanup ()
{
  /// curr is the address of the chaining
  ACE_Node<T> **curr = &(this->head_->next_);
  ACE_ASSERT (number_of_iterators_ == 0);

  // Keep looking until we've hit the dummy node.
  while (*curr != this->head_)
    {
      if ((*curr)->deleted_)
        {
          ACE_Node<T> *temp = *curr;
          *curr = (*curr)->next_; // skip the deleted, curr is still the same
          ACE_DES_FREE_TEMPLATE (temp,
                                 this->allocator_->free,
                                 ACE_Node,
                                 <T>);
        }
      else
        {
          curr = &((*curr)->next_);
        }
    }
}

template <class T>
ACE_Unbounded_Set_Ex<T>::~ACE_Unbounded_Set_Ex (void)
{
  // ACE_TRACE ("ACE_Unbounded_Set_Ex<T>::~ACE_Unbounded_Set_Ex");

  this->delete_nodes ();

  // Delete the dummy node.
  ACE_DES_FREE_TEMPLATE (head_,
                         this->allocator_->free,
                         ACE_Node,
                         <T>);
  this->head_ = 0;
}

template <class T>
ACE_Unbounded_Set_Ex<T>::ACE_Unbounded_Set_Ex (ACE_Allocator *alloc)
  : head_ (0),
    cur_size_ (0),
    allocator_ (alloc),
    number_of_iterators_ (0)
{
  // ACE_TRACE ("ACE_Unbounded_Set_Ex<T>::ACE_Unbounded_Set_Ex");

  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  ACE_NEW_MALLOC (this->head_,
                  (ACE_Node<T>*) this->allocator_->malloc (sizeof (ACE_Node<T>)),
                  ACE_Node<T>);
  // Make the list circular by pointing it back to itself.
  this->head_->next_ = this->head_;
}

template <class T>
ACE_Unbounded_Set_Ex<T>::ACE_Unbounded_Set_Ex (const ACE_Unbounded_Set_Ex<T> &us)
  : head_ (0),
    cur_size_ (0),
    allocator_ (us.allocator_),
    number_of_iterators_ (0)
{
  ACE_TRACE ("ACE_Unbounded_Set_Ex<T>::ACE_Unbounded_Set_Ex");

  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  ACE_NEW_MALLOC (this->head_,
                  (ACE_Node<T>*) this->allocator_->malloc (sizeof (ACE_Node<T>)),
                  ACE_Node<T>);
  this->head_->next_ = this->head_;
  this->copy_nodes (us);
}

template <class T> void
ACE_Unbounded_Set_Ex<T>::operator= (const ACE_Unbounded_Set_Ex<T> &us)
{
  ACE_TRACE ("ACE_Unbounded_Set_Ex<T>::operator=");

  if (this != &us)
    {
      this->delete_nodes ();
      this->copy_nodes (us);
    }
}

template <class T> int
ACE_Unbounded_Set_Ex<T>::find (const T &item) const
{
  // ACE_TRACE ("ACE_Unbounded_Set_Ex<T>::find");
  // Set <item> into the dummy node.
  this->head_->item_ = item;

  ACE_Node<T> *temp = this->head_->next_;

  // Keep looping until we find the item.
  while (!(temp->item_ == item && !temp->deleted_))
    temp = temp->next_;

  // If we found the dummy node then it's not really there, otherwise,
  // it is there.
  return temp == this->head_ ? -1 : 0;
}

template <class T> int
ACE_Unbounded_Set_Ex<T>::insert (const T &item)
{
  // ACE_TRACE ("ACE_Unbounded_Set_Ex<T>::insert");
  if (this->find (item) == 0)
    return 1;
  else
    return this->insert_tail (item);
}

template <class T> int
ACE_Unbounded_Set_Ex<T>::remove (const T &item)
{
  // ACE_TRACE ("ACE_Unbounded_Set_Ex<T>::remove");

  // Insert the item to be founded into the dummy node.
  this->head_->item_ = item;
  this->head_->deleted_ = false;

  ACE_Node<T> *curr = this->head_;

  while (!(curr->next_->item_ == item) || curr->next_->deleted_)
    curr = curr->next_;

  if (curr->next_ == this->head_)
    return -1; // Item was not found.
  else
    {
      this->cur_size_--;
      ACE_Node<T> *temp = curr->next_;
      if (number_of_iterators_>0)
        {
          temp->deleted_=true;
        }
      else
        {
          // Skip over the node that we're deleting.
          curr->next_ = temp->next_;
          ACE_DES_FREE_TEMPLATE (temp,
                                 this->allocator_->free,
                                 ACE_Node,
                                 <T>);
        }
      return 0;
    }
}

template <class T> ACE_Unbounded_Set_Ex_Iterator<T>
ACE_Unbounded_Set_Ex<T>::begin (void)
{
  // ACE_TRACE ("ACE_Unbounded_Set_Ex<T>::begin");
  return ACE_Unbounded_Set_Ex_Iterator<T> (*this);
}

template <class T> ACE_Unbounded_Set_Ex_Iterator<T>
ACE_Unbounded_Set_Ex<T>::end (void)
{
  // ACE_TRACE ("ACE_Unbounded_Set_Ex<T>::end");
  return ACE_Unbounded_Set_Ex_Iterator<T> (*this, 1);
}

template <class T> void
ACE_Unbounded_Set_Ex<T>::iterator_add (void) const
{
  number_of_iterators_++;
}

template <class T> void
ACE_Unbounded_Set_Ex<T>::iterator_leave (void)
{
  ACE_ASSERT (number_of_iterators_ > 0);
  number_of_iterators_--;
  if (number_of_iterators_ == 0)
    cleanup ();
}

template <class T> void
ACE_Unbounded_Set_Ex<T>::const_iterator_leave (void) const
{
  ACE_ASSERT (number_of_iterators_ > 0);
  number_of_iterators_--;
}

ACE_ALLOC_HOOK_DEFINE(ACE_Unbounded_Set_Ex_Iterator)

template <class T> void
ACE_Unbounded_Set_Ex_Iterator<T>::dump (void) const
{
  // ACE_TRACE ("ACE_Unbounded_Set_Ex_Iterator<T>::dump");
}

template <class T>
ACE_Unbounded_Set_Ex_Iterator<T>::ACE_Unbounded_Set_Ex_Iterator (ACE_Unbounded_Set_Ex<T> &s, int end)
  : current_ (end == 0 ? s.head_->next_ : s.head_ ), set_ (&s)
{
  // ACE_TRACE ("ACE_Unbounded_Set_Ex_Iterator<T>::ACE_Unbounded_Set_Ex_Iterator");
  // the first one may be deleted
  while (this->current_->deleted_ && this->current_ != this->set_->head_)
    this->current_ = this->current_->next_;
  registered_in_set_ = (!end && this->current_ != this->set_->head_);
  if (registered_in_set_)
    set_->iterator_add ();
}

template <class T>
ACE_Unbounded_Set_Ex_Iterator<T>::ACE_Unbounded_Set_Ex_Iterator (const ACE_Unbounded_Set_Ex_Iterator<T> &o)
  : current_ (o.current_), set_ (o.set_),
    registered_in_set_ (o.registered_in_set_)
{
  if (registered_in_set_)
    set_->iterator_add ();
}

template <class T> void
ACE_Unbounded_Set_Ex_Iterator<T>::operator= (const ACE_Unbounded_Set_Ex_Iterator &o)
{
  if (this == &o)
    return;
  if (registered_in_set_)
    set_->iterator_leave ();
  this->set_ = o.set_;
  this->current_ = o.current_;
  this->registered_in_set_ = o.registered_in_set_;
  if (registered_in_set_)
    set_->iterator_add ();
}


template <class T>
ACE_Unbounded_Set_Ex_Iterator<T>::~ACE_Unbounded_Set_Ex_Iterator ()
{
  if (registered_in_set_)
    set_->iterator_leave ();
}

template <class T> int
ACE_Unbounded_Set_Ex_Iterator<T>::advance (void)
{
  // ACE_TRACE ("ACE_Unbounded_Set_Ex_Iterator<T>::advance");
  this->current_ = this->current_->next_;
  while (this->current_->deleted_ && this->current_ != this->set_->head_)
    this->current_ = this->current_->next_;
  int completed = (this->current_ == this->set_->head_);
  if (completed && registered_in_set_)
    {
      set_->iterator_leave ();
      registered_in_set_ = false;
    }
  return !completed;
}

template <class T> int
ACE_Unbounded_Set_Ex_Iterator<T>::first (void)
{
  // ACE_TRACE ("ACE_Unbounded_Set_Ex_Iterator<T>::first");
  this->current_ = this->set_->head_->next_;
  while (this->current_->deleted_ && this->current_ != this->set_->head_)
    this->current_ = this->current_->next_;
  int non_empty = (this->current_ != this->set_->head_);
  if (non_empty && !registered_in_set_)
    {
      registered_in_set_ = true;
      set_->iterator_add ();
    }
  return non_empty;
}

template <class T> int
ACE_Unbounded_Set_Ex_Iterator<T>::done (void) const
{
  ACE_TRACE ("ACE_Unbounded_Set_Ex_Iterator<T>::done");

  return this->current_ == this->set_->head_;
}

template <class T> int
ACE_Unbounded_Set_Ex_Iterator<T>::next (T *&item)
{
  // ACE_TRACE ("ACE_Unbounded_Set_Ex_Iterator<T>::next");
  int completed = (this->current_ == this->set_->head_);
  if (completed)
    {
      if (registered_in_set_)
        {
          set_->iterator_leave ();
          registered_in_set_ = false;
        }
      return 0;
    }
  item = &this->current_->item_;
  return 1;
}

template <class T> ACE_Unbounded_Set_Ex_Iterator<T>
ACE_Unbounded_Set_Ex_Iterator<T>::operator++ (int)
{
  //ACE_TRACE ("ACE_Unbounded_Set_Ex_Iterator<T>::operator++ (int)");
  ACE_Unbounded_Set_Ex_Iterator<T> retv (*this);

  // postfix operator

  this->advance ();
  return retv;
}

template <class T> ACE_Unbounded_Set_Ex_Iterator<T>&
ACE_Unbounded_Set_Ex_Iterator<T>::operator++ (void)
{
  // ACE_TRACE ("ACE_Unbounded_Set_Ex_Iterator<T>::operator++ (void)");

  // prefix operator

  this->advance ();
  return *this;
}

template <class T> T&
ACE_Unbounded_Set_Ex_Iterator<T>::operator* (void)
{
  //ACE_TRACE ("ACE_Unbounded_Set_Ex_Iterator<T>::operator*");
  T *retv = 0;

  int result = this->next (retv);
  ACE_ASSERT (result != 0);
  ACE_UNUSED_ARG (result);

  return *retv;
}

template <class T> int
ACE_Unbounded_Set_Ex_Iterator<T>::operator== (const ACE_Unbounded_Set_Ex_Iterator<T> &rhs) const
{
  //ACE_TRACE ("ACE_Unbounded_Set_Ex_Iterator<T>::operator==");
  return (this->set_ == rhs.set_ && this->current_ == rhs.current_);
}

template <class T> int
ACE_Unbounded_Set_Ex_Iterator<T>::operator!= (const ACE_Unbounded_Set_Ex_Iterator<T> &rhs) const
{
  //ACE_TRACE ("ACE_Unbounded_Set_Ex_Iterator<T>::operator!=");
  return (this->set_ != rhs.set_ || this->current_ != rhs.current_);
}

ACE_ALLOC_HOOK_DEFINE(ACE_Unbounded_Set_Ex_Const_Iterator)

template <class T> void
ACE_Unbounded_Set_Ex_Const_Iterator<T>::dump (void) const
{
  // ACE_TRACE ("ACE_Unbounded_Set_Ex_Const_Iterator<T>::dump");
}

template <class T>
ACE_Unbounded_Set_Ex_Const_Iterator<T>::ACE_Unbounded_Set_Ex_Const_Iterator (const ACE_Unbounded_Set_Ex<T> &s, int end)
  : current_ (end == 0 ? s.head_->next_ : s.head_ ), set_ (&s)
{
  // ACE_TRACE ("ACE_Unbounded_Set_Ex_Const_Iterator<T>::ACE_Unbounded_Set_Ex_Const_Iterator");
  // the first one may be deleted
  while (this->current_->deleted_ && this->current_ != this->set_->head_)
    this->current_ = this->current_->next_;
  registered_in_set_ = (!end && this->current_ != this->set_->head_);
  if (registered_in_set_)
    set_->iterator_add ();
}

template <class T>
ACE_Unbounded_Set_Ex_Const_Iterator<T>::ACE_Unbounded_Set_Ex_Const_Iterator
     (const ACE_Unbounded_Set_Ex_Const_Iterator<T> &o)
     : current_ (o.current_),
       set_ (o.set_),
       registered_in_set_ (o.registered_in_set_)
{
  if (registered_in_set_)
    set_->iterator_add ();
}

template <class T>
void ACE_Unbounded_Set_Ex_Const_Iterator<T>::operator=
     (const ACE_Unbounded_Set_Ex_Const_Iterator& o)
{
  if (this == &o)
    return;
  if (registered_in_set_)
    set_->const_iterator_leave ();
  this->set_ = o.set_;
  this->current_ = o.current_;
  this->registered_in_set_ = o.registered_in_set_;
  if (registered_in_set_)
    set_->iterator_add ();
}

template <class T>
ACE_Unbounded_Set_Ex_Const_Iterator<T>::~ACE_Unbounded_Set_Ex_Const_Iterator ()
{
  if (registered_in_set_)
    set_->const_iterator_leave ();
}

template <class T> int
ACE_Unbounded_Set_Ex_Const_Iterator<T>::advance (void)
{
  // ACE_TRACE ("ACE_Unbounded_Set_Ex_Const_Iterator<T>::advance");
  this->current_ = this->current_->next_;
  while (this->current_->deleted_ && this->current_ != this->set_->head_)
    this->current_ = this->current_->next_;
  int completed = (this->current_ == this->set_->head_);
  if (completed && registered_in_set_)
    {
      set_->const_iterator_leave ();
      registered_in_set_ = false;
    }
  return !completed;
}

template <class T> int
ACE_Unbounded_Set_Ex_Const_Iterator<T>::first (void)
{
  // ACE_TRACE ("ACE_Unbounded_Set_Ex_Const_Iterator<T>::first");
  this->current_ = this->set_->head_->next_;
  while (this->current_->deleted_ && this->current_ != this->set_->head_)
    this->current_ = this->current_->next_;
  int non_empty = (this->current_ != this->set_->head_);
  if (non_empty && !registered_in_set_)
    {
      registered_in_set_ = true;
      set_->iterator_add ();
    }
  return non_empty;
}

template <class T> int
ACE_Unbounded_Set_Ex_Const_Iterator<T>::done (void) const
{
  ACE_TRACE ("ACE_Unbounded_Set_Ex_Const_Iterator<T>::done");

  return this->current_ == this->set_->head_;
}

template <class T> int
ACE_Unbounded_Set_Ex_Const_Iterator<T>::next (T *&item)
{
  // ACE_TRACE ("ACE_Unbounded_Set_Ex_Const_Iterator<T>::next");
  int completed = (this->current_ == this->set_->head_);
  if (completed)
    {
      if (registered_in_set_)
        {
          set_->const_iterator_leave ();
          registered_in_set_ = false;
        }
      return 0;
    }
  item = &this->current_->item_;
  return 1;
}

template <class T> ACE_Unbounded_Set_Ex_Const_Iterator<T>
ACE_Unbounded_Set_Ex_Const_Iterator<T>::operator++ (int)
{
  //ACE_TRACE ("ACE_Unbounded_Set_Ex_Const_Iterator<T>::operator++ (int)");
  ACE_Unbounded_Set_Ex_Const_Iterator<T> retv (*this);

  // postfix operator

  this->advance ();
  return retv;
}

template <class T> ACE_Unbounded_Set_Ex_Const_Iterator<T>&
ACE_Unbounded_Set_Ex_Const_Iterator<T>::operator++ (void)
{
  // ACE_TRACE ("ACE_Unbounded_Set_Ex_Const_Iterator<T>::operator++ (void)");

  // prefix operator

  this->advance ();
  return *this;
}

template <class T> T&
ACE_Unbounded_Set_Ex_Const_Iterator<T>::operator* (void)
{
  //ACE_TRACE ("ACE_Unbounded_Set_Ex_Const_Iterator<T>::operator*");
  T *retv = 0;

  int result = this->next (retv);
  ACE_ASSERT (result != 0);
  ACE_UNUSED_ARG (result);

  return *retv;
}

#endif /* ACE_UNBOUNDED_SET_EX_C */
