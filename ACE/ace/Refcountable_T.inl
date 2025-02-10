// -*- C++ -*-
//
ACE_BEGIN_VERSIONED_NAMESPACE_DECL

template <class ACE_LOCK>
ACE_INLINE
ACE_Refcountable_T<ACE_LOCK>::ACE_Refcountable_T (long refcount)
  : refcount_ (refcount)
{
}

template <class ACE_LOCK>
ACE_INLINE long
ACE_Refcountable_T<ACE_LOCK>::increment ()
{
  return ++this->refcount_;
}

template <class ACE_LOCK>
ACE_INLINE long
ACE_Refcountable_T<ACE_LOCK>::decrement ()
{
  return --this->refcount_;
}

template <class ACE_LOCK>
ACE_INLINE long
ACE_Refcountable_T<ACE_LOCK>::refcount () const
{
  return this->refcount_.value ();
}

ACE_END_VERSIONED_NAMESPACE_DECL
