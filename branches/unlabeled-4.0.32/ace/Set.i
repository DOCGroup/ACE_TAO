/* -*- C++ -*- */
// $Id$

// Set.i

template <class T, size_t SIZE> ACE_INLINE size_t
ACE_Fixed_Set<T, SIZE>::size (void) const
{
  return this->cur_size_;
}

template <class T> ACE_INLINE size_t
ACE_Bounded_Set<T>::size (void) const
{
  ACE_TRACE ("ACE_Bounded_Set<T>::size");
  return this->cur_size_;
}

template <class T> ACE_INLINE size_t 
ACE_Unbounded_Set<T>::size (void) const
{
// ACE_TRACE ("ACE_Unbounded_Set<T>::size");
  return this->cur_size_;
}
