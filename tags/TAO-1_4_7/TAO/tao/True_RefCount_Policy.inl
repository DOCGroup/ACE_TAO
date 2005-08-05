// -*- C++ -*-
//
// $Id$


ACE_INLINE
TAO::True_RefCount_Policy::True_RefCount_Policy (void)
  : refcount_ (1)
{
}

ACE_INLINE void
TAO::True_RefCount_Policy::add_ref (void)
{
  ++this->refcount_;
}

ACE_INLINE void
TAO::True_RefCount_Policy::remove_ref (void)
{
  const unsigned long new_count = --this->refcount_;

  if (new_count == 0)
    delete this;
}
