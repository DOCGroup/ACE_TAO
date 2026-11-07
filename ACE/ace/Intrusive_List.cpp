#ifndef ACE_INTRUSIVE_LIST_CPP
#define ACE_INTRUSIVE_LIST_CPP

#include "ace/Intrusive_List.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (__ACE_INLINE__)
#include "ace/Intrusive_List.inl"
#endif /* __ACE_INLINE__ */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

template<class T> void
ACE_Intrusive_List<T>::push_back (T *node)
{
  if (this->tail_ == nullptr)
    {
      this->tail_ = node;
      this->head_ = node;
      node->next (nullptr);
      node->prev (nullptr);
    }
  else
    {
      this->tail_->next (node);
      node->prev (this->tail_);
      node->next (nullptr);
      this->tail_ = node;
    }
}

template<class T> void
ACE_Intrusive_List<T>::push_front (T *node)
{
  if (this->head_ == nullptr)
    {
      this->tail_ = node;
      this->head_ = node;
      node->next (nullptr);
      node->prev (nullptr);
    }
  else
    {
      this->head_->prev (node);
      node->next (this->head_);
      node->prev (nullptr);
      this->head_ = node;
    }
}

template<class T> T *
ACE_Intrusive_List<T>::pop_front ()
{
  T * const node = this->head_;
  if (node != nullptr)
    {
      this->unsafe_remove (node);
    }
  return node;
}

template<class T> T *
ACE_Intrusive_List<T>::pop_back ()
{
  T * const node = this->tail_;
  if (node != nullptr)
    {
      this->unsafe_remove (node);
    }
  return node;
}

template<class T> void
ACE_Intrusive_List<T>::remove (T *node)
{
  for (T *i = this->head_; i != nullptr; i = i->next ())
    {
      if (node == i)
        {
          this->unsafe_remove (node);
          return;
        }
    }
}

template<class T> void
ACE_Intrusive_List<T>::unsafe_remove (T *node)
{
  if (node->prev () != nullptr)
    node->prev ()->next (node->next ());
  else
    this->head_ = node->next ();

  if (node->next () != nullptr)
    node->next ()->prev (node->prev ());
  else
    this->tail_ = node->prev ();

  node->next (nullptr);
  node->prev (nullptr);
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_INTRUSIVE_LIST_CPP */
