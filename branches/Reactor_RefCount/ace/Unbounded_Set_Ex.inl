/* -*- C++ -*- */
// $Id$

template <class T> ACE_INLINE int
ACE_Unbounded_Set_Ex<T>::is_empty (void) const
{
  ACE_TRACE ("ACE_Unbounded_Set_Ex<T>::is_empty");
  return this->head_ == this->head_->next_;
}

template <class T> ACE_INLINE int
ACE_Unbounded_Set_Ex<T>::is_full (void) const
{
  ACE_TRACE ("ACE_Unbounded_Set_Ex<T>::is_full");
  return 0; // We should implement a "node of last resort for this..."
}
