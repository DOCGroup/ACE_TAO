// -*- C++ -*-
//
// $Id$

ACE_INLINE void
TAO_IORInfo::invalidate (void)
{
  this->poa_ = 0;
}

ACE_INLINE void
TAO_IORInfo::components_established (void)
{
  this->components_established_ = 1;
}
