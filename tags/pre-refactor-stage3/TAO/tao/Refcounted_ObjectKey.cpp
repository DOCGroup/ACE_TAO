//$Id$
#include "Refcounted_ObjectKey.h"


#if !defined (__ACE_INLINE__)
#include "Refcounted_ObjectKey.inl"
#endif /* defined INLINE */

ACE_RCSID(tao,
          Refcounted_ObjectKey,
          "$Id$")


TAO::Refcounted_ObjectKey::Refcounted_ObjectKey (const TAO::ObjectKey &key)
  : object_key_ (key)
  , ref_count_ (1)
{
}


TAO::Refcounted_ObjectKey::~Refcounted_ObjectKey (void)
{
}

long
TAO::Refcounted_ObjectKey::decr_refcount (void)
{
  if (--this->ref_count_ > 0)
    return this->ref_count_;

  ACE_ASSERT (this->ref_count_ == 0);

  delete this;

  return 0;
}
