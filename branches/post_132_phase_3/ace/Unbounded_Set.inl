/* -*- C++ -*- */
// $Id$

template <class T> ACE_INLINE int
ACE_Unbounded_Set<T>::is_empty (void) const
{
  ACE_TRACE ("ACE_Unbounded_Set<T>::is_empty");
  // Does not work if deleted elements are in the list:
  // return this->head_ == this->head_->next_;
  return size() == 0;
}

template <class T> ACE_INLINE int
ACE_Unbounded_Set<T>::is_full (void) const
{
  ACE_TRACE ("ACE_Unbounded_Set<T>::is_full");
  return 0; // We should implement a "node of last resort for this..."
}
