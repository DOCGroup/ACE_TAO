// -*- C++ -*-
//$Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_Configurable_Refcount::TAO_Configurable_Refcount (
                      unsigned long value)
 : refcount_ (value)
{
}

ACE_INLINE unsigned long
TAO_Configurable_Refcount::increment (void)
{
  return this->refcount_++;
}

ACE_INLINE unsigned long
TAO_Configurable_Refcount::decrement (void)
{
  return --this->refcount_;
}

ACE_INLINE unsigned long
TAO_Configurable_Refcount::value (void) const
{
  return this->refcount_.value ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
