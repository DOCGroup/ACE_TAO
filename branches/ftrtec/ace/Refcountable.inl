/* -*- C++ -*- */
//$Id$
ACE_INLINE
ACE_Refcountable::ACE_Refcountable (int refcount)
  : refcount_ (refcount)
{
}

ACE_INLINE
ACE_Refcountable::~ACE_Refcountable (void)
{
}

ACE_INLINE int
ACE_Refcountable::increment (void)
{
  return ++this->refcount_;
}

ACE_INLINE int
ACE_Refcountable::decrement (void)
{
  return --this->refcount_;
}

ACE_INLINE int
ACE_Refcountable::refcount (void) const
{
  return this->refcount_;
}
