/* -*- C++ -*- */
//$Id$

#if !defined (ACE_HAS_BROKEN_EXTENDED_TEMPLATES)

#define ACE_T1 class KEY, class VALUE, class MAP, class ITERATOR_IMPL, class REVERSE_ITERATOR_IMPL, class CACHING_STRATEGY, class ATTRIBUTES
#define ACE_T2 KEY, VALUE, MAP, ITERATOR_IMPL, REVERSE_ITERATOR_IMPL, CACHING_STRATEGY, ATTRIBUTES

#else

#define ACE_T1 class KEY, class VALUE, class MAP, class CACHING_STRATEGY, class ATTRIBUTES
#define ACE_T2 KEY, VALUE, MAP, CACHING_STRATEGY, ATTRIBUTES

#endif /* ACE_HAS_BROKEN_EXTENDED_TEMPLATES */

template <ACE_T1> ACE_INLINE int
ACE_Cache_Map_Manager<ACE_T2>::purge (void)
{
  return this->caching_strategy ().caching_utility ().clear_cache (this->map_,
                                                                   this->caching_strategy ().purge_percent ());
}

template <ACE_T1> ACE_INLINE size_t
ACE_Cache_Map_Manager<ACE_T2>::current_size (void) const
{
  return this->map_.current_size ();
}

template <ACE_T1> ACE_INLINE size_t
ACE_Cache_Map_Manager<ACE_T2>::total_size (void) const
{
  return this->map_.total_size ();
}

template <ACE_T1> ACE_INLINE MAP &
ACE_Cache_Map_Manager<ACE_T2>::map (void)
{
  return this->map_;
}

template <ACE_T1> ACE_INLINE CACHING_STRATEGY &
ACE_Cache_Map_Manager<ACE_T2>::caching_strategy (void)
{
  return this->caching_strategy_;
}

#if !defined (ACE_HAS_BROKEN_EXTENDED_TEMPLATES)

template <ACE_T1> ACE_INLINE ACE_Cache_Map_Iterator<KEY, VALUE, ITERATOR_IMPL, CACHING_STRATEGY, ATTRIBUTES>
ACE_Cache_Map_Manager<ACE_T2>::begin (void)
{
  return ITERATOR (this->map_.begin ());
}

template <ACE_T1> ACE_INLINE ACE_Cache_Map_Iterator<KEY, VALUE, ITERATOR_IMPL, CACHING_STRATEGY, ATTRIBUTES>
ACE_Cache_Map_Manager<ACE_T2>::end (void)
{
  return ITERATOR (this->map_.end ());
}

template <ACE_T1> ACE_INLINE ACE_Cache_Map_Reverse_Iterator<KEY, VALUE, REVERSE_ITERATOR_IMPL, CACHING_STRATEGY, ATTRIBUTES>
ACE_Cache_Map_Manager<ACE_T2>::rbegin (void)
{
  return REVERSE_ITERATOR (this->map_.rbegin ());
}
template <ACE_T1> ACE_INLINE ACE_Cache_Map_Reverse_Iterator<KEY, VALUE, REVERSE_ITERATOR_IMPL, CACHING_STRATEGY, ATTRIBUTES>
ACE_Cache_Map_Manager<ACE_T2>::rend (void)
{
  return REVERSE_ITERATOR (this->map_.rend ());
}

#undef ACE_T1
#undef ACE_T2

////////////////////////////////////////////////////////////////////////////////

#define ACE_T1 class KEY, class VALUE, class IMPLEMENTATION, class CACHING_STRATEGY, class ATTRIBUTES
#define ACE_T2 KEY, VALUE, IMPLEMENTATION, CACHING_STRATEGY, ATTRIBUTES

template <ACE_T1> ACE_INLINE
ACE_Cache_Map_Iterator<ACE_T2>::ACE_Cache_Map_Iterator (const ACE_Cache_Map_Iterator <ACE_T2> &rhs)
  : iterator_implementation_ (rhs.iterator_implementation_)
{
}

template <ACE_T1> ACE_INLINE
ACE_Cache_Map_Iterator<ACE_T2>::~ACE_Cache_Map_Iterator (void)
{
}

template <ACE_T1> ACE_INLINE ACE_Cache_Map_Iterator<ACE_T2> &
ACE_Cache_Map_Iterator<ACE_T2>::operator= (const ACE_Cache_Map_Iterator<ACE_T2> &rhs)
{
  this->iterator_implementation_ = rhs.iterator_implementation_;
  return *this;
}

template <ACE_T1> ACE_INLINE int
ACE_Cache_Map_Iterator<ACE_T2>::operator== (const ACE_Cache_Map_Iterator<ACE_T2> &rhs) const
{
  return this->iterator_implementation_ == rhs.iterator_implementation_;
}

template <ACE_T1> ACE_INLINE int
ACE_Cache_Map_Iterator<ACE_T2>::operator!= (const ACE_Cache_Map_Iterator<ACE_T2> &rhs) const
{
  return this->iterator_implementation_ != rhs.iterator_implementation_;
}

template <ACE_T1> ACE_INLINE ACE_Reference_Pair<KEY, VALUE>
ACE_Cache_Map_Iterator<ACE_T2>::operator* (void) const
{
  value_type retn ((*this->iterator_implementation_).ext_id_,
                   (*this->iterator_implementation_).int_id_.first ());
  return retn;
}

template <ACE_T1> ACE_INLINE
ACE_Cache_Map_Iterator<ACE_T2> &
ACE_Cache_Map_Iterator<ACE_T2>::operator++ (void)
{
  ++this->iterator_implementation_;
  return *this;
}

template <ACE_T1> ACE_INLINE
ACE_Cache_Map_Iterator<ACE_T2>
ACE_Cache_Map_Iterator<ACE_T2>::operator++ (int)
{
  ACE_Cache_Map_Iterator<ACE_T2> retn = *this;
  ++this->iterator_implementation_;
  return retn;
}

template <ACE_T1> ACE_INLINE
ACE_Cache_Map_Iterator<ACE_T2> &
ACE_Cache_Map_Iterator<ACE_T2>::operator-- (void)
{
  --this->iterator_implementation_;
  return *this;
}

template <ACE_T1> ACE_INLINE
ACE_Cache_Map_Iterator<ACE_T2>
ACE_Cache_Map_Iterator<ACE_T2>::operator-- (int)
{
  ACE_Cache_Map_Iterator<ACE_T2> retn = *this;
  --this->iterator_implementation_;
  return retn;
}


template <ACE_T1> ACE_INLINE void
ACE_Cache_Map_Iterator<ACE_T2>::dump (void) const
{
  this->iterator_implementation_.dump ();
}

template <ACE_T1> ACE_INLINE
ACE_Cache_Map_Iterator<ACE_T2>::ACE_Cache_Map_Iterator (const IMPLEMENTATION &iterator_impl)
  : iterator_implementation_ (iterator_impl)
{
}

template <ACE_T1> ACE_INLINE IMPLEMENTATION &
ACE_Cache_Map_Iterator<ACE_T2>::iterator_implementation (void)
{
  return this->iterator_implementation_;
}

#undef ACE_T1
#undef ACE_T2

////////////////////////////////////////////////////////////////////////////////

#define ACE_T1 class KEY, class VALUE, class REVERSE_IMPLEMENTATION, class CACHING_STRATEGY, class ATTRIBUTES
#define ACE_T2 KEY, VALUE, REVERSE_IMPLEMENTATION, CACHING_STRATEGY, ATTRIBUTES

template <ACE_T1> ACE_INLINE
ACE_Cache_Map_Reverse_Iterator<ACE_T2>::ACE_Cache_Map_Reverse_Iterator (const ACE_Cache_Map_Reverse_Iterator <ACE_T2> &rhs)
  : reverse_iterator_implementation_ (rhs.reverse_iterator_implementation_)
{
}

template <ACE_T1> ACE_INLINE
ACE_Cache_Map_Reverse_Iterator<ACE_T2>::~ACE_Cache_Map_Reverse_Iterator (void)
{
}

template <ACE_T1> ACE_INLINE ACE_Cache_Map_Reverse_Iterator<ACE_T2> &
ACE_Cache_Map_Reverse_Iterator<ACE_T2>::operator= (const ACE_Cache_Map_Reverse_Iterator<ACE_T2> &rhs)
{
  this->reverse_iterator_implementation_ = rhs.reverse_iterator_implementation_;
  return *this;
}

template <ACE_T1> ACE_INLINE int
ACE_Cache_Map_Reverse_Iterator<ACE_T2>::operator== (const ACE_Cache_Map_Reverse_Iterator<ACE_T2> &rhs) const
{
  return this->reverse_iterator_implementation_ == rhs.reverse_iterator_implementation_;
}

template <ACE_T1> ACE_INLINE int
ACE_Cache_Map_Reverse_Iterator<ACE_T2>::operator!= (const ACE_Cache_Map_Reverse_Iterator<ACE_T2> &rhs) const
{
  return this->reverse_iterator_implementation_ != rhs.reverse_iterator_implementation_;
}

template <ACE_T1> ACE_INLINE ACE_Reference_Pair<KEY, VALUE>
ACE_Cache_Map_Reverse_Iterator<ACE_T2>::operator* (void) const
{
  value_type retv ((*this->reverse_iterator_implementation_).ext_id_,
                   (*this->reverse_iterator_implementation_).int_id_.first ());
  return retv;
}

template <ACE_T1> ACE_INLINE
ACE_Cache_Map_Reverse_Iterator<ACE_T2> &
ACE_Cache_Map_Reverse_Iterator<ACE_T2>::operator++ (void)
{
  ++this->reverse_iterator_implementation_;
  return *this;
}

template <ACE_T1> ACE_INLINE
ACE_Cache_Map_Reverse_Iterator<ACE_T2>
ACE_Cache_Map_Reverse_Iterator<ACE_T2>::operator++ (int)
{
  ACE_Cache_Map_Reverse_Iterator<ACE_T2> retn = *this;
  ++this->reverse_iterator_implementation_;
  return retn;
}

template <ACE_T1> ACE_INLINE
ACE_Cache_Map_Reverse_Iterator<ACE_T2> &
ACE_Cache_Map_Reverse_Iterator<ACE_T2>::operator-- (void)
{
  --this->reverse_iterator_implementation_;
  return *this;
}

template <ACE_T1> ACE_INLINE
ACE_Cache_Map_Reverse_Iterator<ACE_T2>
ACE_Cache_Map_Reverse_Iterator<ACE_T2>::operator-- (int)
{
  ACE_Cache_Map_Reverse_Iterator<ACE_T2> retn = *this;
  --this->reverse_iterator_implementation_;
  return retn;
}


template <ACE_T1> ACE_INLINE void
ACE_Cache_Map_Reverse_Iterator<ACE_T2>::dump (void) const
{
  this->reverse_iterator_implementation_.dump ();
}

template <ACE_T1> ACE_INLINE
ACE_Cache_Map_Reverse_Iterator<ACE_T2>::ACE_Cache_Map_Reverse_Iterator (const REVERSE_IMPLEMENTATION &iterator_impl)
  : reverse_iterator_implementation_(iterator_impl)
{
}

template <ACE_T1> ACE_INLINE REVERSE_IMPLEMENTATION &
ACE_Cache_Map_Reverse_Iterator<ACE_T2>::iterator_implementation (void)
{
  return this->reverse_iterator_implementation_;
}

#endif /* ACE_HAS_BROKEN_EXTENDED_TEMPLATES */

#undef ACE_T1
#undef ACE_T2
