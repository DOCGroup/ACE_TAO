// -*- C++ -*-
// $Id$
ACE_INLINE long
TAO::Refcounted_ObjectKey::incr_refcount (void)
{
  return ++this->ref_count_;
}

ACE_INLINE long
TAO::Refcounted_ObjectKey::decr_refcount (void)
{
  if (--this->ref_count_ > 0)
    return this->ref_count_;

  ACE_ASSERT (this->ref_count_ == 0);

  delete this;

  return 0;
}

ACE_INLINE const TAO::ObjectKey &
TAO::Refcounted_ObjectKey::object_key (void) const
{
  return this->object_key_;
}
