// -*- C++ -*-
ACE_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename Key, typename Value, class EqualTo, class Alloc>
ACE_INLINE
ACE_Array_Map<Key, Value, EqualTo, Alloc>::ACE_Array_Map (
  typename ACE_Array_Map<Key, Value, EqualTo, Alloc>::size_type s)
  : size_ (0)
  , capacity_ (s)
  , nodes_ (s == 0 ? 0 : this->alloc_.allocate (s))
{
  std::uninitialized_fill_n (this->nodes_, s, value_type ());
}

template<typename Key, typename Value, class EqualTo, class Alloc>
ACE_INLINE ACE_Array_Map<Key, Value, EqualTo, Alloc> &
ACE_Array_Map<Key, Value, EqualTo, Alloc>::operator= (
  ACE_Array_Map<Key, Value, EqualTo, Alloc> const & map)
{
  // Strongly exception-safe assignment.

  ACE_Array_Map<Key, Value, EqualTo, Alloc> temp (map);
  this->swap (temp);
  return *this;
}

template<typename Key, typename Value, class EqualTo, class Alloc>
ACE_INLINE typename ACE_Array_Map<Key, Value, EqualTo, Alloc>::iterator
ACE_Array_Map<Key, Value, EqualTo, Alloc>::begin ()
{
  return this->nodes_;
}

template<typename Key, typename Value, class EqualTo, class Alloc>
ACE_INLINE typename ACE_Array_Map<Key, Value, EqualTo, Alloc>::iterator
ACE_Array_Map<Key, Value, EqualTo, Alloc>::end ()
{
  return this->nodes_ + this->size_;
}

template<typename Key, typename Value, class EqualTo, class Alloc>
ACE_INLINE typename ACE_Array_Map<Key, Value, EqualTo, Alloc>::const_iterator
ACE_Array_Map<Key, Value, EqualTo, Alloc>::begin () const
{
  return this->nodes_;
}

template<typename Key, typename Value, class EqualTo, class Alloc>
ACE_INLINE typename ACE_Array_Map<Key, Value, EqualTo, Alloc>::const_iterator
ACE_Array_Map<Key, Value, EqualTo, Alloc>::end () const
{
  return this->nodes_ + this->size_;
}

template<typename Key, typename Value, class EqualTo, class Alloc>
ACE_INLINE typename ACE_Array_Map<Key, Value, EqualTo, Alloc>::reverse_iterator
ACE_Array_Map<Key, Value, EqualTo, Alloc>::rbegin ()
{
  return reverse_iterator (this->end ());
}

template<typename Key, typename Value, class EqualTo, class Alloc>
ACE_INLINE typename ACE_Array_Map<Key, Value, EqualTo, Alloc>::reverse_iterator
ACE_Array_Map<Key, Value, EqualTo, Alloc>::rend ()
{
  return reverse_iterator (this->begin ());
}

template<typename Key, typename Value, class EqualTo, class Alloc>
ACE_INLINE typename ACE_Array_Map<Key, Value, EqualTo, Alloc>::const_reverse_iterator
ACE_Array_Map<Key, Value, EqualTo, Alloc>::rbegin () const
{
  return const_reverse_iterator (this->end ());
}

template<typename Key, typename Value, class EqualTo, class Alloc>
ACE_INLINE typename ACE_Array_Map<Key, Value, EqualTo, Alloc>::const_reverse_iterator
ACE_Array_Map<Key, Value, EqualTo, Alloc>::rend () const
{
  return const_reverse_iterator (this->begin ());
}

template<typename Key, typename Value, class EqualTo, class Alloc>
ACE_INLINE typename ACE_Array_Map<Key, Value, EqualTo, Alloc>::size_type
ACE_Array_Map<Key, Value, EqualTo, Alloc>::size () const
{
  return this->size_;
}

template<typename Key, typename Value, class EqualTo, class Alloc>
ACE_INLINE typename ACE_Array_Map<Key, Value, EqualTo, Alloc>::size_type
ACE_Array_Map<Key, Value, EqualTo, Alloc>::max_size () const
{
  return size_type (-1) / sizeof (value_type);
}

template<typename Key, typename Value, class EqualTo, class Alloc>
ACE_INLINE bool
ACE_Array_Map<Key, Value, EqualTo, Alloc>::is_empty () const
{
  return this->size_ == 0;
}

// The following method is deprecated.

template<typename Key, typename Value, class EqualTo, class Alloc>
ACE_INLINE bool
ACE_Array_Map<Key, Value, EqualTo, Alloc>::empty () const
{
  return this->is_empty ();
}

template<typename Key, typename Value, class EqualTo, class Alloc>
ACE_INLINE typename ACE_Array_Map<Key, Value, EqualTo, Alloc>::size_type
ACE_Array_Map<Key, Value, EqualTo, Alloc>::count (
  typename ACE_Array_Map<Key, Value, EqualTo, Alloc>::key_type const & k)
{
  return
    (this->find (k) == this->end () ? 0 : 1); // Only one datum per key.
}

template<typename Key, typename Value, class EqualTo, class Alloc>
ACE_INLINE typename ACE_Array_Map<Key, Value, EqualTo, Alloc>::mapped_type &
ACE_Array_Map<Key, Value, EqualTo, Alloc>::operator[] (
  typename ACE_Array_Map<Key, Value, EqualTo, Alloc>::key_type const & k)
{
  iterator i = (this->insert (value_type (k, mapped_type ()))).first;
  return (*i).second;
}

ACE_END_VERSIONED_NAMESPACE_DECL
