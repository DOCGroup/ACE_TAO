//=============================================================================
/**
 *  @file    Naming_Service_Container.cpp
 *
 *  $Id$
 *
 *  @author Bruce Trask <trask_b@ociweb.com>
 */
//=============================================================================



#ifndef NS_CONTAINER_CPP
#define NS_CONTAINER_CPP

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Log_Macros.h"
#include "ace/Malloc_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Naming/Naming_Service_Container.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_ALLOC_HOOK_DEFINE(ACE_NS_Node)

template <class T>
ACE_NS_Node<T>::~ACE_NS_Node (void)
{
}

template <class T>
ACE_NS_Node<T>::ACE_NS_Node (const T &i, ACE_NS_Node<T> *n)
  : next_ (n),
    item_ (i)
{
  // ACE_TRACE ("ACE_NS_Node<T>::ACE_NS_Node");
}

template <class T>
ACE_NS_Node<T>::ACE_NS_Node (ACE_NS_Node<T> *n, int)
  : next_ (n)
{
  // ACE_TRACE ("ACE_NS_Node<T>::ACE_NS_Node");
}

template <class T>
ACE_NS_Node<T>::ACE_NS_Node (const ACE_NS_Node<T> &s)
  : next_ (s.next_),
    item_ (s.item_)
{
  // ACE_TRACE ("ACE_NS_Node<T>::ACE_NS_Node");
}


ACE_ALLOC_HOOK_DEFINE(ACE_Unbounded_List)

  template <class T> size_t
ACE_Unbounded_List<T>::size (void) const
{
  // ACE_TRACE ("ACE_Unbounded_List<T>::size");
  return this->cur_size_;
}

template <class T> int
ACE_Unbounded_List<T>::insert_tail (const T &item)
{
  ACE_NS_Node<T> *temp;

  // Insert <item> into the old dummy node location.
  this->head_->item_ = item;

  // Create a new dummy node.
  ACE_NEW_MALLOC_RETURN (temp,
                         (ACE_NS_Node<T>*) this->allocator_->malloc (sizeof (ACE_NS_Node<T>)),
                         ACE_NS_Node<T> (this->head_->next_),
                         -1);
  // Link this pointer into the list.
  this->head_->next_ = temp;

  // Point the head to the new dummy node.
  this->head_ = temp;

  this->cur_size_++;
  return 0;
}

template <class T> void
ACE_Unbounded_List<T>::reset (void)
{
  ACE_TRACE ("reset");

  this->delete_nodes ();
}

template <class T> void
ACE_Unbounded_List<T>::dump (void) const
{
  ACE_TRACE ("ACE_Unbounded_List<T>::dump");

  ORBSVCS_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ORBSVCS_DEBUG ((LM_DEBUG,  ACE_TEXT ("\nhead_ = %u"), this->head_));
  ORBSVCS_DEBUG ((LM_DEBUG,  ACE_TEXT ("\nhead_->next_ = %u"), this->head_->next_));
  ORBSVCS_DEBUG ((LM_DEBUG,  ACE_TEXT ("\ncur_size_ = %d\n"), this->cur_size_));

  T *item = 0;
#if !defined (ACE_NLOGGING)
  size_t count = 1;
#endif /* ! ACE_NLOGGING */

  for (ACE_Unbounded_List_Iterator<T> iter (*(ACE_Unbounded_List<T> *) this);
       iter.next (item) != 0;
       iter.advance ())
    ORBSVCS_DEBUG ((LM_DEBUG,  ACE_TEXT ("count = %d\n"), count++));

  ORBSVCS_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class T> void
ACE_Unbounded_List<T>::copy_nodes (const ACE_Unbounded_List<T> &us)
{
  for (ACE_NS_Node<T> *curr = us.head_->next_;
       curr != us.head_;
       curr = curr->next_)
    this->insert_tail (curr->item_);
}

template <class T> void
ACE_Unbounded_List<T>::delete_nodes (void)
{
  ACE_NS_Node<T> *curr = this->head_->next_;

  // Keep looking until we've hit the dummy node.

  while (curr != this->head_)
    {
      ACE_NS_Node<T> *temp = curr;
      curr = curr->next_;
      ACE_DES_FREE_TEMPLATE (temp,
                             this->allocator_->free,
                             ACE_NS_Node,
                             <T>);
      this->cur_size_--;
    }

  // Reset the list to be a circular list with just a dummy node.
  this->head_->next_ = this->head_;
}

template <class T>
ACE_Unbounded_List<T>::~ACE_Unbounded_List (void)
{
  // ACE_TRACE ("ACE_Unbounded_List<T>::~ACE_Unbounded_List");

  this->delete_nodes ();

  // Delete the dummy node.
  ACE_DES_FREE_TEMPLATE (head_,
                         this->allocator_->free,
                         ACE_NS_Node,
                         <T>);
  this->head_ = 0;
}

template <class T>
ACE_Unbounded_List<T>::ACE_Unbounded_List (ACE_Allocator *alloc)
  : head_ (0),
    cur_size_ (0),
    allocator_ (alloc)
{
  // ACE_TRACE ("ACE_Unbounded_List<T>::ACE_Unbounded_List");

  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  ACE_NEW_MALLOC (this->head_,
                  (ACE_NS_Node<T>*) this->allocator_->malloc (sizeof (ACE_NS_Node<T>)),
                  ACE_NS_Node<T>);
  // Make the list circular by pointing it back to itself.
  this->head_->next_ = this->head_;
}

template <class T>
ACE_Unbounded_List<T>::ACE_Unbounded_List (const ACE_Unbounded_List<T> &us)
  : head_ (0),
    cur_size_ (0),
    allocator_ (us.allocator_)
{
  ACE_TRACE ("ACE_Unbounded_List<T>::ACE_Unbounded_List");

  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  ACE_NEW_MALLOC (this->head_,
                  (ACE_NS_Node<T>*) this->allocator_->malloc (sizeof (ACE_NS_Node<T>)),
                  ACE_NS_Node<T>);
  this->head_->next_ = this->head_;
  this->copy_nodes (us);
}

template <class T> void
ACE_Unbounded_List<T>::operator= (const ACE_Unbounded_List<T> &us)
{
  ACE_TRACE ("ACE_Unbounded_List<T>::operator=");

  if (this != &us)
    {
      this->delete_nodes ();
      this->copy_nodes (us);
    }
}

template <class T> int
ACE_Unbounded_List<T>::insert (const T &item)
{
  ACE_TRACE ("ACE_Unbounded_List<T>::insert");
  return this->insert_tail (item);
}

template <class T> int
ACE_Unbounded_List<T>::remove (const T &item)
{
  // ACE_TRACE ("ACE_Unbounded_List<T>::remove");

  // Insert the item to be founded into the dummy node.
  this->head_->item_ = item;

  ACE_NS_Node<T> *curr = this->head_;

  while (!(curr->next_->item_ == item))
    curr = curr->next_;

  if (curr->next_ == this->head_)
    return -1; // Item was not found.
  else
    {
      ACE_NS_Node<T> *temp = curr->next_;
      // Skip over the node that we're deleting.
      curr->next_ = temp->next_;
      this->cur_size_--;
      ACE_DES_FREE_TEMPLATE (temp,
                             this->allocator_->free,
                             ACE_NS_Node,
                             <T>);
      return 0;
    }
}

template <class T> ACE_Unbounded_List_Iterator<T>
ACE_Unbounded_List<T>::begin (void)
{
  // ACE_TRACE ("ACE_Unbounded_List<T>::begin");
  return ACE_Unbounded_List_Iterator<T> (*this);
}

template <class T> ACE_Unbounded_List_Iterator<T>
ACE_Unbounded_List<T>::end (void)
{
  // ACE_TRACE ("ACE_Unbounded_List<T>::end");
  return ACE_Unbounded_List_Iterator<T> (*this, 1);
}


ACE_ALLOC_HOOK_DEFINE(ACE_Unbounded_List_Iterator)

  template <class T> void
ACE_Unbounded_List_Iterator<T>::dump (void) const
{
  // ACE_TRACE ("ACE_Unbounded_List_Iterator<T>::dump");
}

template <class T>
ACE_Unbounded_List_Iterator<T>::ACE_Unbounded_List_Iterator (ACE_Unbounded_List<T> &s, int end)
  : current_ (end == 0 ? s.head_->next_ : s.head_ ),
    set_ (&s)
{
  // ACE_TRACE ("ACE_Unbounded_List_Iterator<T>::ACE_Unbounded_List_Iterator");
}

template <class T> int
ACE_Unbounded_List_Iterator<T>::advance (void)
{
  // ACE_TRACE ("ACE_Unbounded_List_Iterator<T>::advance");
  this->current_ = this->current_->next_;
  return this->current_ != this->set_->head_;
}

template <class T> int
ACE_Unbounded_List_Iterator<T>::first (void)
{
  // ACE_TRACE ("ACE_Unbounded_List_Iterator<T>::first");
  this->current_ = this->set_->head_->next_;
  return this->current_ != this->set_->head_;
}

template <class T> int
ACE_Unbounded_List_Iterator<T>::done (void) const
{
  ACE_TRACE ("ACE_Unbounded_List_Iterator<T>::done");

  return this->current_ == this->set_->head_;
}

template <class T> int
ACE_Unbounded_List_Iterator<T>::next (T *&item)
{
  // ACE_TRACE ("ACE_Unbounded_List_Iterator<T>::next");
  if (this->current_ == this->set_->head_)
    return 0;
  else
    {
      item = &this->current_->item_;
      return 1;
    }
}

template <class T> ACE_Unbounded_List_Iterator<T>
ACE_Unbounded_List_Iterator<T>::operator++ (int)
{
  //ACE_TRACE ("ACE_Unbounded_List_Iterator<T>::operator++ (int)");
  ACE_Unbounded_List_Iterator<T> retv (*this);

  // postfix operator

  this->advance ();
  return retv;
}

template <class T> ACE_Unbounded_List_Iterator<T>&
ACE_Unbounded_List_Iterator<T>::operator++ (void)
{
  // ACE_TRACE ("ACE_Unbounded_List_Iterator<T>::operator++ (void)");

  // prefix operator

  this->advance ();
  return *this;
}

template <class T> T&
ACE_Unbounded_List_Iterator<T>::operator* (void)
{
  //ACE_TRACE ("ACE_Unbounded_List_Iterator<T>::operator*");
  T *retv = 0;

  int result = this->next (retv);
  ACE_ASSERT (result != 0);
  ACE_UNUSED_ARG (result);

  return *retv;
}

template <class T> bool
ACE_Unbounded_List_Iterator<T>::operator== (const ACE_Unbounded_List_Iterator<T> &rhs) const
{
  //ACE_TRACE ("ACE_Unbounded_List_Iterator<T>::operator==");
  return (this->set_ == rhs.set_ && this->current_ == rhs.current_);
}

template <class T> bool
ACE_Unbounded_List_Iterator<T>::operator!= (const ACE_Unbounded_List_Iterator<T> &rhs) const
{
  //ACE_TRACE ("ACE_Unbounded_List_Iterator<T>::operator!=");
  return (this->set_ != rhs.set_ || this->current_ != rhs.current_);
}

// ---

template <class T> int
ACE_Unbounded_List<T>::is_empty (void) const
{
  ACE_TRACE ("ACE_Unbounded_List<T>::is_empty");
  return this->head_ == this->head_->next_;
}

template <class T>  int
ACE_Unbounded_List<T>::is_full (void) const
{
  ACE_TRACE ("ACE_Unbounded_List<T>::is_full");
  return 0; // We should implement a "node of last resort for this..."
}

TAO_END_VERSIONED_NAMESPACE_DECL


#endif /* NS_CONTAINERS_T_CPP */
