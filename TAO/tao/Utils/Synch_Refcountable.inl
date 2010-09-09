// -*- C++ -*-
//
//$Id$

#include "ace/Guard_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE int
TAO_Synch_Refcountable::increment (void)
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->refcount_lock_, 0);
  return ACE_Refcountable_T<ACE_Null_Mutex>::increment ();
}

ACE_INLINE int
TAO_Synch_Refcountable::decrement (void)
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->refcount_lock_, 0);
  return ACE_Refcountable_T<ACE_Null_Mutex>::decrement ();
}

ACE_INLINE int
TAO_Synch_Refcountable::refcount (void) const
{
  return ACE_Refcountable_T<ACE_Null_Mutex>::refcount ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
