// -*- C++ -*-
//$Id$
ACE_INLINE int
TAO_Synch_Refcountable::increment (void)
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->refcount_lock_, 0);
  return ACE_Refcountable::increment ();
}

ACE_INLINE int
TAO_Synch_Refcountable::decrement (void)
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->refcount_lock_, 0);
  return ACE_Refcountable::decrement ();
}

ACE_INLINE int
TAO_Synch_Refcountable::refcount (void) const
{
  return ACE_Refcountable::refcount ();
}
