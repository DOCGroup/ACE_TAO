/* -*- C++ -*- */
//$Id$

#include "Optimal_Cache_Map_Manager_T.h"

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE
ACE_Optimal_Cache_Map_Manager<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::ACE_Optimal_Cache_Map_Manager (ACE_Allocator *alloc,
                                                                                                                    ATTRIBUTES attributes,
                                                                                                                    int purge_percent)
  :map_ (alloc),
   caching_strategy_ (map_, attributes, purge_percent)
{
}


template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE
ACE_Optimal_Cache_Map_Manager<KEY, VALUE,  HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::ACE_Optimal_Cache_Map_Manager (size_t size,
                                                                                                                     ACE_Allocator *alloc,
                                                                                                                     ATTRIBUTES attributes,
                                                                                                                     int purge_percent)
  :map_ (size, 
         alloc),
   caching_strategy_ (map_, attributes, purge_percent)
{
}
                       
template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE
ACE_Optimal_Cache_Map_Manager<KEY, VALUE,  HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::~ACE_Optimal_Cache_Map_Manager (void)
{
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE int
ACE_Optimal_Cache_Map_Manager<KEY, VALUE,  HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::open (size_t length,
                                                                                            ACE_Allocator *alloc)  
{
  return this->map_.open (length,
                          alloc);
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE  int
ACE_Optimal_Cache_Map_Manager<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::purge (void)
{
  ACE_DEBUG ((LM_DEBUG, "ACE_Optimal_Cache_Map_Manager::PURGE\n"));
  return this->caching_strategy ().clear_cache (this->map ());
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE  size_t
ACE_Optimal_Cache_Map_Manager<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::current_size (void)
{
  return this->map_.current_size ();
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE  size_t
ACE_Optimal_Cache_Map_Manager<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::total_size (void)
{
  return this->map_.total_size ();
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE int
ACE_Optimal_Cache_Map_Manager<KEY, VALUE,  HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::close (void)  
{
  return this->map_.close ();
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE ACE_Optimal_Cache_Map_Manager<KEY, VALUE,  HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::MAP &
ACE_Optimal_Cache_Map_Manager<KEY, VALUE,  HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::map (void) 
{
  return this->map_;
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE CACHING_STRATEGY &
ACE_Optimal_Cache_Map_Manager<KEY, VALUE,  HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::caching_strategy (void) 
{
  return this->caching_strategy_;
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE ACE_Optimal_Cache_Map_Manager<KEY, VALUE,  HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::ITERATOR
ACE_Optimal_Cache_Map_Manager<KEY, VALUE,  HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::begin (void)
{
  return ACE_Optimal_Cache_Map_Manager<KEY, VALUE,  HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::ITERATOR (this->map_.begin ());
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE  ACE_Optimal_Cache_Map_Manager<KEY, VALUE,  HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::ITERATOR
ACE_Optimal_Cache_Map_Manager<KEY, VALUE,  HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::end (void)
{
  return ACE_Optimal_Cache_Map_Manager<KEY, VALUE,  HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::ITERATOR (this->map_.end ());
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE  ACE_Optimal_Cache_Map_Manager<KEY, VALUE,  HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::REVERSE_ITERATOR
ACE_Optimal_Cache_Map_Manager<KEY, VALUE,  HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::rbegin (void)
{
  return ACE_Optimal_Cache_Map_Manager<KEY, VALUE,  HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::REVERSE_ITERATOR (this->map_.rbegin ());
}
template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE ACE_Optimal_Cache_Map_Manager<KEY, VALUE,  HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::REVERSE_ITERATOR
ACE_Optimal_Cache_Map_Manager<KEY, VALUE,  HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::rend (void)
{
  return ACE_Optimal_Cache_Map_Manager<KEY, VALUE,  HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::REVERSE_ITERATOR (this->map_.rend ());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE 
ACE_Optimal_Cache_Map_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::ACE_Optimal_Cache_Map_Iterator (const ACE_Optimal_Cache_Map_Iterator <KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY> &rhs)
 : iterator_implementation_ (rhs.iterator_implementation ())
{
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE 
ACE_Optimal_Cache_Map_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::~ACE_Optimal_Cache_Map_Iterator (void)
{
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE ACE_Optimal_Cache_Map_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY> &
ACE_Optimal_Cache_Map_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::operator= (const ACE_Optimal_Cache_Map_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY> &rhs)
{
  this->iterator_implementation_ = rhs.iterator_implementation ();
  return *this;
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE int
ACE_Optimal_Cache_Map_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::operator== (const ACE_Optimal_Cache_Map_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY> &rhs)
{
  return this->iterator_implementation_ == rhs.iterator_implementation ();
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE int
ACE_Optimal_Cache_Map_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::operator!= (const ACE_Optimal_Cache_Map_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY> &rhs)
{
  return this->iterator_implementation_ != rhs.iterator_implementation ();
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE ACE_Reference_Pair<KEY, VALUE> 
ACE_Optimal_Cache_Map_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::operator* (void) const
{
  value_type retv ((*this->iterator_implementation_).ext_id_, (*this->iterator_implementation_).int_id_.first ());
  return retv;
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE 
ACE_Optimal_Cache_Map_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY> &
ACE_Optimal_Cache_Map_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::operator++ (void)
{
  this->iterator_implementation_.operator++ ();
  return *this;
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE 
ACE_Optimal_Cache_Map_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY> 
ACE_Optimal_Cache_Map_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::operator++ (int)
{
  this->iterator_implementation_.operator++ (1);
  return *this;
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE 
ACE_Optimal_Cache_Map_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY> &
ACE_Optimal_Cache_Map_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::operator-- (void)
{
  this->iterator_implementation_.operator-- ();
  return *this;
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE 
ACE_Optimal_Cache_Map_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY> 
ACE_Optimal_Cache_Map_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::operator-- (int)
{
  this->iterator_implementation_.operator-- (1);
  return *this;
}


template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE void
ACE_Optimal_Cache_Map_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::dump (void) const
{
  return this->iterator_implementation_.dump ();
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE
ACE_Optimal_Cache_Map_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::ACE_Optimal_Cache_Map_Iterator (const ACE_Optimal_Cache_Map_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::IMPLEMENTATION &iterator_impl)
 : iterator_implementation_(iterator_impl) 
{
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE ACE_Optimal_Cache_Map_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::IMPLEMENTATION
ACE_Optimal_Cache_Map_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::iterator_implementation (void) const
{
  return this->iterator_implementation_;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE 
ACE_Optimal_Cache_Map_Reverse_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::ACE_Optimal_Cache_Map_Reverse_Iterator (const ACE_Optimal_Cache_Map_Reverse_Iterator <KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY> &rhs)
 : reverse_iterator_implementation_ (rhs.iterator_implementation ())
{
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE 
ACE_Optimal_Cache_Map_Reverse_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::~ACE_Optimal_Cache_Map_Reverse_Iterator (void)
{
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE ACE_Optimal_Cache_Map_Reverse_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY> &
ACE_Optimal_Cache_Map_Reverse_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::operator= (const ACE_Optimal_Cache_Map_Reverse_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY> &rhs)
{
  this->reverse_iterator_implementation_ = rhs.iterator_implementation ();
  return *this;
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE int
ACE_Optimal_Cache_Map_Reverse_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::operator== (const ACE_Optimal_Cache_Map_Reverse_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY> &rhs)
{
  return this->reverse_iterator_implementation_ == rhs.iterator_implementation ();
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE int
ACE_Optimal_Cache_Map_Reverse_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::operator!= (const ACE_Optimal_Cache_Map_Reverse_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY> &rhs)
{
  return this->reverse_iterator_implementation_ != rhs.iterator_implementation ();
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE ACE_Reference_Pair<KEY, VALUE> 
ACE_Optimal_Cache_Map_Reverse_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::operator* (void) const
{
  value_type retv ((*this->reverse_iterator_implementation_).ext_id_, (*this->reverse_iterator_implementation_).int_id_.first ());
  return retv;
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE 
ACE_Optimal_Cache_Map_Reverse_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY> &
ACE_Optimal_Cache_Map_Reverse_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::operator++ (void)
{
  this->reverse_iterator_implementation_.operator++ ();
  return *this;
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE 
ACE_Optimal_Cache_Map_Reverse_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY> 
ACE_Optimal_Cache_Map_Reverse_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::operator++ (int)
{
  this->reverse_iterator_implementation_.operator++ (1);
  return *this;
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE 
ACE_Optimal_Cache_Map_Reverse_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY> &
ACE_Optimal_Cache_Map_Reverse_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::operator-- (void)
{
  this->reverse_iterator_implementation_.operator-- ();
  return *this;
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE 
ACE_Optimal_Cache_Map_Reverse_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY> 
ACE_Optimal_Cache_Map_Reverse_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::operator-- (int)
{
  this->reverse_iterator_implementation_.operator-- (1);
  return *this;
}


template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE void
ACE_Optimal_Cache_Map_Reverse_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::dump (void) const
{
  return this->reverse_iterator_implementation_.dump ();
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE
ACE_Optimal_Cache_Map_Reverse_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::ACE_Optimal_Cache_Map_Reverse_Iterator (const ACE_Optimal_Cache_Map_Reverse_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::REVERSE_IMPLEMENTATION &iterator_impl)
 : reverse_iterator_implementation_(iterator_impl) 
{
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> ACE_INLINE ACE_Optimal_Cache_Map_Reverse_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::REVERSE_IMPLEMENTATION
ACE_Optimal_Cache_Map_Reverse_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::iterator_implementation (void) const
{
  return this->reverse_iterator_implementation_;
}

