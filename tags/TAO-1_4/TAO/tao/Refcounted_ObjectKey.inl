// -*- C++ -*-
// $Id$
ACE_INLINE long
TAO::Refcounted_ObjectKey::incr_refcount (void)
{
  return ++this->ref_count_;
}



ACE_INLINE const TAO::ObjectKey &
TAO::Refcounted_ObjectKey::object_key (void) const
{
  return this->object_key_;
}
