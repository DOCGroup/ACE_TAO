// -*- C++ -*-
// $Id$


template <class T> ACE_INLINE
ACE_Vector<T>::~ACE_Vector (void)
{
  ACE_DES_ARRAY_FREE (this->vector_,
                      this->capacity_,
                      this->allocator_->free,
                      T);
}

template <class T> ACE_INLINE T&
ACE_Vector<T>::front (void)
{
  return this->vector_[0];
}

template <class T> ACE_INLINE const T&
ACE_Vector<T>::front (void) const
{
  return this->vector_[0];
}

template <class T> ACE_INLINE T&
ACE_Vector<T>::back (void)
{
  return this->vector_[this->size_ - 1];
}

template <class T> ACE_INLINE const T&
ACE_Vector<T>::back (void) const
{
  return this->vector_[this->size_ - 1];
}

template <class T> ACE_INLINE void
ACE_Vector<T>::push_back (const T& value)
{
  if (this->size_ == this->capacity_)
    this->reserve (this->capacity_ + ACE_DEFAULT_VECTOR_BLOCK_SIZE);

  this->vector_[this->size_] = value;

  (this->size_)++;
}

template <class T> ACE_INLINE void
ACE_Vector<T>::pop_back (void)
{
  // The capacity will remain unchanged.
  (this->size_)--;
}

template <class T> ACE_INLINE ACE_Vector<T>::iterator
ACE_Vector<T>::begin (void)
{
  return this->vector_;
}

template <class T> ACE_INLINE const ACE_Vector<T>::iterator
ACE_Vector<T>::begin (void) const
{
  return this->vector_;
}

template <class T> ACE_INLINE ACE_Vector<T>::iterator
ACE_Vector<T>::end (void)
{
  return this->vector_ + size_;
}

template <class T> ACE_INLINE const ACE_Vector<T>::iterator
ACE_Vector<T>::end (void) const
{
  return this->vector_ + size_;
}

template <class T> ACE_INLINE size_t
ACE_Vector<T>::size (void) const
{
  return this->size_;
}

template <class T> ACE_INLINE size_t
ACE_Vector<T>::max_size (void) const
{
  // @@ Is this correct?
  return size_t (-1) / sizeof (T);
}

template <class T> ACE_INLINE size_t
ACE_Vector<T>::capacity (void) const
{
  return this->capacity_;
}

template <class T> ACE_INLINE int
ACE_Vector<T>::empty (void) const
{
  return this->size_ == 0;
}

template <class T> ACE_INLINE T&
ACE_Vector<T>::operator[] (size_t index)
{
  return this->vector_[index];
}

template <class T> ACE_INLINE const T&
ACE_Vector<T>::operator[] (size_t index) const
{
  return this->vector_[index];
}

template <class T> ACE_INLINE int
ACE_Vector<T>::operator!= (const ACE_Vector<T> &rhs) const
{
  return !(*this == rhs);
}
