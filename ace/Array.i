/* -*- C++ -*- */

template <class T> ACE_INLINE size_t 
ACE_Array<T>::size (void) const
{
  return this->cur_size_;
}

template <class T> ACE_INLINE int
ACE_Array<T>::in_range (size_t index) const
{
  return index < this->cur_size_;
}

template <class T> ACE_INLINE T &
ACE_Array<T>::operator[] (size_t index)
{
  return this->array_[index];
}
