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
  this->grow (); // Only grows when necessary

  this->vector_[this->size_] = value;

  ++(this->size_);
}

template <class T> ACE_INLINE void
ACE_Vector<T>::pop_back (void)
{
  // The capacity will remain unchanged.
  --(this->size_);
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
  return this->vector_ + this->size_;
}

template <class T> ACE_INLINE const ACE_Vector<T>::iterator
ACE_Vector<T>::end (void) const
{
  return this->vector_ + this->size_;
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

template <class T> ACE_INLINE void
ACE_Vector<T>::grow (void)
{
  // ACE_Vector growth will always be proportional to the current
  // capacity.  Unless specified by using ACE_Vector<T>::reserve(),
  // the capacity will automatically grow by:
  //
  //     ACE_DEFAULT_VECTOR_GROWTH_FACTOR * ACE_Vector::capacity()
  //
  // In order to ensure insertion of a series of elements into a
  // vector is a linear time operation growth must be proportionall to 
  // the current capacity.  Otherwise, if growth is not proportional
  // to the current capacity, and instead grows by a constant amount,
  // insertion becomes a quadratic time operation.

  // Only grow when necessary
  if (this->size_ == this->capacity_)
    this->reserve (this->capacity_ * ACE_DEFAULT_VECTOR_GROWTH_FACTOR);
}

template <class T> ACE_INLINE void
ACE_Vector<T>::grow (size_t amount)
{
  // ACE_Vector growth will always be proportional to the current
  // capacity.  Unless specified by using ACE_Vector<T>::reserve(),
  // the capacity will automatically grow by:
  //
  //     ACE_DEFAULT_VECTOR_GROWTH_FACTOR * ACE_Vector::capacity()
  //
  // In order to ensure insertion of a series of elements into a
  // vector is a linear time operation growth must be proportionall to 
  // the current capacity.  Otherwise, if growth is not proportional
  // to the current capacity, and instead grows by a constant amount,
  // insertion becomes a quadratic time operation.

  // Only grow when necessary
  if (this->size_ + amount >= this->capacity_)
      this->reserve (this->size_ + amount);
}
