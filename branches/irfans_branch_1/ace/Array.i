/* -*- C++ -*- */

// Clean up the array (e.g., delete dynamically allocated memory).

template <class T>  ACE_INLINE
ACE_Array<T>::~ACE_Array (void)
{
   delete [] this->array_;
}

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

template <class T> ACE_INLINE const T &
ACE_Array<T>::operator[] (size_t index) const
{
  return this->array_[index];
}

// Compare this array with <s> for inequality.

template <class T> ACE_INLINE int
ACE_Array<T>::operator!= (const ACE_Array<T> &s) const
{
  return !(*this == s);
}

template <class T> ACE_INLINE void
ACE_Array_Iterator<T>::dump (void) const
{
  // ACE_TRACE ("ACE_Array_Iterator<T>::dump");
}

template <class T> ACE_INLINE
ACE_Array_Iterator<T>::ACE_Array_Iterator (ACE_Array<T> &a)
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
