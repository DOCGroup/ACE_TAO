// -*- C++ -*-
//
// $Id$

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

template<class T> ACE_INLINE int
ACE_Intrusive_List<T>::is_empty (void) const
{
  return this->head_ == 0;
}

template<class T> ACE_INLINE int
ACE_Intrusive_List<T>::empty (void) const
{
  return this->is_empty ();
}

template<class T> ACE_INLINE T *
ACE_Intrusive_List<T>::head (void) const
{
  return this->head_;
}

template<class T> ACE_INLINE T *
ACE_Intrusive_List<T>::tail (void) const
{
  return this->tail_;
}

ACE_END_VERSIONED_NAMESPACE_DECL
