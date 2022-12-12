// -*- C++ -*-

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE void
TAO_IORInfo::invalidate ()
{
  this->poa_ = 0;
}

ACE_INLINE void
TAO_IORInfo::components_established ()
{
  this->components_established_ = true;
}

TAO_END_VERSIONED_NAMESPACE_DECL
