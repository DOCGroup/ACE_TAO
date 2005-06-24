// -*- C++ -*-
//
// $Id$

template<typename Key, typename Value>
ACE_INLINE
ACE_Array_Map<Key, Value>::ACE_Array_Map (
  typename ACE_Array_Map<Key, Value>::size_type s)
  : size_ (0)
  , capacity_ (s)
  , nodes_ (s == 0 ? 0 : new value_type[s])
{
}

template<typename Key, typename Value>
ACE_INLINE ACE_Array_Map<Key, Value> &
ACE_Array_Map<Key, Value>::operator= (ACE_Array_Map<Key, Value> const & map)
{
  // Strongly exception-safe assignment.

  ACE_Array_Map<Key, Value> temp (map);
  this->swap (temp);
  return *this;
}

template<typename Key, typename Value>
ACE_INLINE typename ACE_Array_Map<Key, Value>::iterator
ACE_Array_Map<Key, Value>::begin (void)
{
  return this->nodes_;
}

template<typename Key, typename Value>
ACE_INLINE typename ACE_Array_Map<Key, Value>::iterator
ACE_Array_Map<Key, Value>::end (void)
{
  return this->nodes_ + this->size_;
}

template<typename Key, typename Value>
ACE_INLINE typename ACE_Array_Map<Key, Value>::const_iterator
ACE_Array_Map<Key, Value>::begin (void) const
{
  return this->nodes_;
}

template<typename Key, typename Value>
ACE_INLINE typename ACE_Array_Map<Key, Value>::const_iterator
ACE_Array_Map<Key, Value>::end (void) const
{
  return this->nodes_ + this->size_;
}

template<typename Key, typename Value>
ACE_INLINE typename ACE_Array_Map<Key, Value>::reverse_iterator
ACE_Array_Map<Key, Value>::rbegin (void)
{
  return reverse_iterator (this->end ());
}

template<typename Key, typename Value>
ACE_INLINE typename ACE_Array_Map<Key, Value>::reverse_iterator
ACE_Array_Map<Key, Value>::rend (void)
{
  return reverse_iterator (this->begin ());
}

template<typename Key, typename Value>
ACE_INLINE typename ACE_Array_Map<Key, Value>::const_reverse_iterator
ACE_Array_Map<Key, Value>::rbegin (void) const
{
  return const_reverse_iterator (this->end ());
}

template<typename Key, typename Value>
ACE_INLINE typename ACE_Array_Map<Key, Value>::const_reverse_iterator
ACE_Array_Map<Key, Value>::rend (void) const
{
  return const_reverse_iterator (this->begin ());
}

template<typename Key, typename Value>
ACE_INLINE typename ACE_Array_Map<Key, Value>::size_type
ACE_Array_Map<Key, Value>::size (void) const
{
  return this->size_;
}

template<typename Key, typename Value>
ACE_INLINE typename ACE_Array_Map<Key, Value>::size_type
ACE_Array_Map<Key, Value>::max_size (void) const
{
  return size_type (-1) / sizeof (value_type);
}

template<typename Key, typename Value>
ACE_INLINE bool
ACE_Array_Map<Key, Value>::empty (void) const
{
  return this->size_ == 0;
}

template<typename Key, typename Value>
ACE_INLINE typename ACE_Array_Map<Key, Value>::size_type
ACE_Array_Map<Key, Value>::count (
  typename ACE_Array_Map<Key, Value>::key_type const & k)
{
  return
    (this->find (k) == this->end () ? 0 : 1); // Only one datum per key.
}

template<typename Key, typename Value>
ACE_INLINE typename ACE_Array_Map<Key, Value>::data_type &
ACE_Array_Map<Key, Value>::operator[] (
  typename ACE_Array_Map<Key, Value>::key_type const & k)
{
  return (*((this->insert (value_type (k, data_type ()))).first)).second;
}
