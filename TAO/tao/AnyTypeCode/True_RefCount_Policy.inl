// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO::True_RefCount_Policy::True_RefCount_Policy ()
  : refcount_ (1)
{
}

ACE_INLINE void
TAO::True_RefCount_Policy::add_ref ()
{
  ++this->refcount_;
}

ACE_INLINE void
TAO::True_RefCount_Policy::remove_ref ()
{
  const unsigned long new_count = --this->refcount_;

  if (new_count == 0)
    delete this;
}

TAO_END_VERSIONED_NAMESPACE_DECL
