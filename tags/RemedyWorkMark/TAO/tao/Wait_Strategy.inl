//$Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE bool
TAO_Wait_Strategy::is_registered (void) const
{
  return this->is_registered_;
}

ACE_INLINE void
TAO_Wait_Strategy::is_registered (bool flag)
{
  this->is_registered_ = flag;
}

TAO_END_VERSIONED_NAMESPACE_DECL
