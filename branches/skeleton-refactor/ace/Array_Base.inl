/* -*- C++ -*- */
// $Id$

// Clean up the array (e.g., delete dynamically allocated memory).

template <class T> ACE_INLINE
ACE_Array_Base<T>::~ACE_Array_Base (void)
{
  ACE_DES_ARRAY_FREE (this->array_,
                      this->max_size_,
                      this->allocator_->free,
                      T);
}

template <class T> ACE_INLINE size_t
ACE_Array_Base<T>::size (void) const
{
  return this->cur_size_;
}

template <class T> ACE_INLINE size_t
ACE_Array_Base<T>::max_size (void) const
{
  return this->max_size_;
}

template <class T> ACE_INLINE int
ACE_Array_Base<T>::in_range (size_t index) const
{
  return index < this->cur_size_;
}

template <class T> ACE_INLINE T &
ACE_Array_Base<T>::operator[] (size_t index)
{
  return this->array_[index];
}

template <class T> ACE_INLINE const T &
ACE_Array_Base<T>::operator[] (size_t index) const
{
  return this->array_[index];
}

// ****************************************************************

template <class T> ACE_INLINE void
ACE_Array_Iterator<T>::dump (void) const
{
#if defined (ACE_HAS_DUMP)
  // ACE_TRACE ("ACE_Array_Iterator<T>::dump");
#endif /* ACE_HAS_DUMP */
}

template <class T> ACE_INLINE
ACE_Array_Iterator<T>::ACE_Array_Iterator (ACE_Array_Base<T> &a)
    : current_ (0),
      array_ (a)
{
  // ACE_TRACE ("ACE_Array_Iterator<T>::ACE_Array_Iterator");
}

template <class T> ACE_INLINE int
ACE_Array_Iterator<T>::advance (void)
{
  // ACE_TRACE ("ACE_Array_Iterator<T>::advance");

  if (this->current_ < array_.size ())
    {
      ++this->current_;
      return 1;
    }
  else
    {
      // Already finished iterating.
      return 0;
    }
}

template <class T> ACE_INLINE int
ACE_Array_Iterator<T>::done (void) const
{
  ACE_TRACE ("ACE_Array_Iterator<T>::done");

  return this->current_ >= array_.size ();
}
