/* -*- C++ -*- */
// $Id$

template <class TYPE> ACE_INLINE CORBA::ULong
ACE_CORBA_Sequence<TYPE>::maximum (void) const
{
  return maximum_;
}

template <class TYPE> ACE_INLINE CORBA::ULong
ACE_CORBA_Sequence<TYPE>::length() const
{
  return length_;
}


// ************************************************************

template <class ITEM, size_t SIZE> ACE_INLINE
ACE_ES_Simple_Array<ITEM, SIZE>::ACE_ES_Simple_Array (void) :
  size_ (0)
{
}

template <class ITEM, size_t SIZE> ACE_INLINE int
ACE_ES_Simple_Array<ITEM, SIZE>::insert (const ITEM &copy_me)
{
  if (size_ >= SIZE)
    return -1;

  data_[size_++] = copy_me;
  return 0;
}

template <class ITEM, size_t SIZE> ACE_INLINE size_t
ACE_ES_Simple_Array<ITEM, SIZE>::size (void)
{
  return size_;
}

template <class ITEM, size_t SIZE> ACE_INLINE ITEM *
ACE_ES_Simple_Array<ITEM, SIZE>::data (void)
{
  return data_;
}

// ************************************************************

template <class ITEM> ACE_INLINE
ACE_ES_Array_Iterator<ITEM>::ACE_ES_Array_Iterator (ITEM *data, size_t size) :
  data_ (data),
  size_ (size),
  index_ (0)
{
}

template <class ITEM> ACE_INLINE int
ACE_ES_Array_Iterator<ITEM>::next (ITEM *&next_item)
{
  next_item = &data_[index_];
  return index_ < (size_ - 1);
}

template <class ITEM> ACE_INLINE int
ACE_ES_Array_Iterator<ITEM>::advance (void)
{
  index_++;
  return index_ < size_;
}

template<class ITEM> ACE_INLINE int
ACE_ES_Array_Iterator<ITEM>::done (void) const
{
  return index_ < size_;
}
