/* -*- C++ -*- */
// $Id$

template <class T, size_t DEFAULT_SIZE> ACE_INLINE
ACE_Vector<T, DEFAULT_SIZE>::ACE_Vector (const size_t init_size,
					 ACE_Allocator* alloc)
  : ACE_Array<T> (init_size == 0 ? DEFAULT_SIZE : init_size, alloc)
{
  length_ = 0;
  curr_max_size_ = this->max_size ();
}

template <class T, size_t DEFAULT_SIZE> ACE_INLINE
ACE_Vector<T, DEFAULT_SIZE>::~ACE_Vector ()
{
}

template <class T, size_t DEFAULT_SIZE> ACE_INLINE
size_t ACE_Vector<T, DEFAULT_SIZE>::capacity (void) const
{
  return curr_max_size_;
}

template <class T, size_t DEFAULT_SIZE> ACE_INLINE
size_t ACE_Vector<T, DEFAULT_SIZE>::size (void) const
{
  return length_;
}

template <class T, size_t DEFAULT_SIZE> ACE_INLINE
void ACE_Vector<T, DEFAULT_SIZE>::clear (void)
{
  length_ = 0;
}

template <class T, size_t DEFAULT_SIZE> ACE_INLINE
void ACE_Vector<T, DEFAULT_SIZE>::pop_back (void)
{
  if (length_ > 0)
    --length_;
}

